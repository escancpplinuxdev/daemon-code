// ============================================================
//  FILE  : diffBetween_Thread_And_pthread.cpp
//  TOPIC : std::thread vs pthread — what each one is, how they differ,
//          and when to use which (Linux / glibc / libstdc++)
//  LEVEL : Concept -> senior interview depth (C++ Linux programmer)
//  STD   : C++20 (jthread, stop_token)
//  PLATFORM: Linux + glibc + libstdc++. Several demos use GNU extensions
//          (pthread_setname_np, pthread_getattr_np, pthread_setattr_default_np).
//  VERIFIED: compiled with -Wall -Wextra -Wshadow -Wsign-conversion (zero
//            warnings) and run under -fsanitize=address,undefined and
//            -fsanitize=thread (zero reports). ThreadSanitizer aborts on a
//            thread that joins itself, so that one demo (Section 6) is
//            skipped automatically in the TSan build.
//            Evidence quoted in comments (nm, strace) is real output from
//            Ubuntu 24.04, glibc 2.39, GCC 13.3.
// ============================================================

#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <stop_token>
#include <string>
#include <system_error>
#include <thread>
#include <vector>
using namespace std;
using namespace std::chrono_literals;

// Aborts with a message if a condition is false. Used instead of
// assert() so the checks still run when NDEBUG is defined.
static void check(bool ok, const char* what)
{
    if (!ok)
    {
        cerr << "CHECK FAILED: " << what << "\n";
        abort();
    }
}

static void banner(const char* title)
{
    cout << "\n--- " << title << " ---\n";
}

// ============================================================
// SECTION 1 : WHAT EACH ONE IS
// ============================================================
//
//  pthread  = POSIX Threads.
//             A C API (<pthread.h>) standardized by POSIX. Available on Linux,
//             macOS, BSD; not native on Windows. On Linux it is implemented by
//             NPTL inside glibc: every pthread is one kernel task (1:1 model)
//             created with the clone3 system call. Since glibc 2.34 the
//             pthread code lives in libc itself, but keep compiling with
//             -pthread for portability and older systems.
//
//  std::thread = the C++11 class in <thread>.
//             A portable abstraction. It does not create threads itself: it
//             calls the platform's native thread API. On Linux with libstdc++
//             that is pthread_create / pthread_join / pthread_detach. With the
//             MSVC STL it is Win32 threads.
//
//   your C++ code
//        |
//   std::thread          C++11, portable, type-safe        (the ABSTRACTION)
//        |   libstdc++ calls pthread_create / pthread_join / pthread_detach
//   pthread_*            POSIX C API, NPTL inside glibc    (the MECHANISM)
//        |   glibc calls the clone3 system call
//   Linux kernel         a thread is a task sharing the process address space
//
//  So on Linux this is not "two competing thread systems". std::thread is a
//  convenience layer over pthreads; the real questions are what the layer
//  adds and what it hides.

// ============================================================
// SECTION 2 : PROOF — std::thread IS pthread ON LINUX
// ============================================================
//
//  Evidence 1 — libstdc++ imports the pthread functions (real output):
//     $ nm -D --undefined-only /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep pthread
//                      U pthread_create@GLIBC_2.34
//                      U pthread_detach@GLIBC_2.34
//                      U pthread_join@GLIBC_2.34
//
//  Evidence 2 — both create the same kernel thread (real strace output):
//     raw pthread_create:
//       clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|
//               CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID,
//               exit_signal=0, stack_size=0x7fff80, ...})
//     std::thread:
//       clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|
//               CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID,
//               exit_signal=0, stack_size=0x7fff80, ...})
//     Identical flags, identical stack size (just under 8 MB).
//
//  Evidence 3 — the demo below: the type and value of the native handle.
//
//  THREE DIFFERENT IDENTIFIERS (a classic interview trap):
//     pthread_t        user-space handle (on glibc, the address of the thread's
//                      control block); opaque, compare with pthread_equal
//     std::thread::id  C++ wrapper around the native handle; on libstdc++ it
//                      prints as the same number as pthread_self()
//                      (an implementation detail, not guaranteed by the standard)
//     kernel TID       gettid(): what the kernel and `top -H` show
//     getpid()         the same for every thread of the process

struct Ids
{
    pthread_t self;
    string stdId;
    long kernelTid;
    pid_t pid;
};

void section2_stdthread_is_pthread()
{
    static_assert(is_same_v<thread::native_handle_type, pthread_t>,
                  "with libstdc++ on Linux, std::thread's native handle IS a pthread_t");

    Ids ids{};
    thread t([&ids]()
    {
        ids.self = pthread_self();
        ostringstream oss;
        oss << this_thread::get_id();
        ids.stdId = oss.str();
        ids.kernelTid = syscall(SYS_gettid);
        ids.pid = getpid();
    });

    pthread_t handle = t.native_handle();         // read BEFORE join
    t.join();

    bool same = pthread_equal(handle, ids.self) != 0;
    cout << "native_handle() == pthread_self() inside the thread : " << (same ? "yes" : "NO") << "\n";
    cout << "std::thread::id printed as       : " << ids.stdId << "\n";
    cout << "pthread_self() printed as number : " << static_cast<unsigned long>(ids.self) << "\n";
    cout << "kernel TID (gettid)              : " << ids.kernelTid << "\n";
    cout << "process ID (same for all threads): " << ids.pid << "\n";
    check(same, "std::thread's native handle must equal pthread_self() inside the thread");
}

// ============================================================
// SECTION 3 : THE SAME JOB, TWO WAYS
// ============================================================
//
//  Task: sum a vector with two worker threads.
//
//  pthread version
//    - the thread function must be  void* (*)(void*)
//    - ONE void* argument: pass several values through a struct
//    - the "return value" is a void*; the usual workaround is an output
//      field in the argument struct
//    - every call returns an int error code that you must check
//    - the argument struct must outlive the thread's use of it
//
//  std::thread version
//    - any callable (lambda, functor, member function) with typed arguments
//    - the result travels through std::promise / std::future (or std::async)
//    - failures throw std::system_error

struct SumArgs
{
    const vector<int>* data;
    size_t begin;
    size_t end;
    long long result;                             // output field = the "return value"
};

static void* sumWorker(void* raw)
{
    SumArgs* a = static_cast<SumArgs*>(raw);
    long long s = 0;
    for (size_t i = a->begin; i < a->end; ++i)
    {
        s += (*a->data)[i];
    }
    a->result = s;
    return nullptr;
}

static long long sumWithPthreads(const vector<int>& v)
{
    size_t mid = v.size() / 2;
    SumArgs args[2] =
    {
        {&v, 0, mid, 0},
        {&v, mid, v.size(), 0}
    };                                            // lives until after both joins
    pthread_t th[2];

    for (int i = 0; i < 2; ++i)
    {
        int rc = pthread_create(&th[i], nullptr, sumWorker, &args[i]);
        check(rc == 0, "pthread_create");
    }
    for (int i = 0; i < 2; ++i)
    {
        int rc = pthread_join(th[i], nullptr);
        check(rc == 0, "pthread_join");
    }
    return args[0].result + args[1].result;
}

static long long sumWithStdThreads(const vector<int>& v)
{
    size_t mid = v.size() / 2;
    auto partial = [&v](size_t b, size_t e)
    {
        return accumulate(v.begin() + static_cast<ptrdiff_t>(b),
                          v.begin() + static_cast<ptrdiff_t>(e), 0LL);
    };

    promise<long long> p1;
    promise<long long> p2;
    future<long long> f1 = p1.get_future();
    future<long long> f2 = p2.get_future();

    thread t1([&]()
    {
        p1.set_value(partial(0, mid));
    });
    thread t2([&]()
    {
        p2.set_value(partial(mid, v.size()));
    });

    long long total = f1.get() + f2.get();
    t1.join();
    t2.join();
    return total;
}

void section3_same_job_two_ways()
{
    vector<int> v(1000);
    iota(v.begin(), v.end(), 1);                  // 1..1000, sum = 500500

    long long a = sumWithPthreads(v);
    long long b = sumWithStdThreads(v);
    cout << "pthread version     : " << a << "\n";
    cout << "std::thread version : " << b << "\n";
    check(a == 500500 && b == 500500, "both versions must give 500500");
}

// ============================================================
// SECTION 4 : PASSING ARGUMENTS
// ============================================================
//
//  pthread: ONE void* argument.
//    BUG:  for (...) { int i = ...; pthread_create(&t, 0, fn, &i); }
//          &i dies at the end of every iteration, the thread reads garbage.
//    OK :  an array that lives until after the joins, or a heap block the
//          thread owns and frees.
//
//  std::thread: every argument is COPIED (decay-copy) or MOVED into storage
//  owned by the thread before the function runs.
//    - no dangling by default
//    - a function that takes `int&` needs std::ref(x), otherwise it does
//      not compile (the thread would be modifying a hidden copy)
//    - std::ref opts back in to sharing, and the lifetime rule returns
//    - move-only types (unique_ptr) work by passing std::move(x)
//    - member functions: thread(&Class::method, &object, args...)

static void addOne(int& x)
{
    ++x;
}

static void consumeOwnership(unique_ptr<int> p)
{
    *p += 1;
}

struct Counter
{
    int value = 0;
    void add(int n)
    {
        value += n;
    }
};

void section4_arguments()
{
    int counter = 0;
    thread t1(addOne, ref(counter));              // without ref(): compile error
    t1.join();

    Counter c;
    thread t2(&Counter::add, &c, 10);             // member function + object + argument
    t2.join();

    unique_ptr<int> owned = make_unique<int>(41);
    thread t3(consumeOwnership, move(owned));     // ownership moves into the thread
    t3.join();

    cout << "counter after thread(addOne, ref(counter))       : " << counter << "\n";
    cout << "c.value after thread(&Counter::add, &c, 10)      : " << c.value << "\n";
    cout << "owned is null after being moved into the thread  : "
         << (owned == nullptr ? "yes" : "no") << "\n";
    check(counter == 1 && c.value == 10 && owned == nullptr, "argument passing results");
}

// ============================================================
// SECTION 5 : RETURN VALUES AND EXCEPTIONS
// ============================================================
//
//  pthread: the start routine returns a void* that pthread_join hands back.
//    Anything richer needs a hand-made result struct. An exception must NOT
//    escape a start routine (it would terminate the process), so the thread
//    catches it and records the failure itself.
//
//  std::thread: the return value of the callable is DISCARDED. Get results with
//    std::promise / std::future, std::packaged_task or std::async. A future
//    carries the value OR the exception, and future.get() rethrows it in the
//    calling thread. An exception that escapes a bare std::thread function
//    calls std::terminate.

static int mayThrow(bool doThrow)
{
    if (doThrow)
    {
        throw runtime_error("boom from the worker");
    }
    return 42;
}

struct PthreadResult
{
    bool failed = false;
    int value = 0;
    string message;
};

static void* pthreadWorker(void* raw)
{
    PthreadResult* r = static_cast<PthreadResult*>(raw);
    try
    {
        r->value = mayThrow(true);
    }
    catch (const exception& e)
    {
        r->failed = true;
        r->message = e.what();
    }
    return nullptr;
}

void section5_return_values_and_exceptions()
{
    future<int> okFuture = async(launch::async, mayThrow, false);
    future<int> badFuture = async(launch::async, mayThrow, true);

    int value = okFuture.get();
    string caught;
    try
    {
        badFuture.get();                          // rethrows the worker's exception HERE
    }
    catch (const runtime_error& e)
    {
        caught = e.what();
    }
    cout << "std::async value                 : " << value << "\n";
    cout << "std::async exception, rethrown   : " << caught << "\n";

    PthreadResult r;
    pthread_t th;
    check(pthread_create(&th, nullptr, pthreadWorker, &r) == 0, "pthread_create");
    check(pthread_join(th, nullptr) == 0, "pthread_join");
    cout << "pthread result struct            : failed=" << (r.failed ? "true" : "false")
         << ", message=" << r.message << "\n";

    check(value == 42 && caught == "boom from the worker", "std::async results");
    check(r.failed && r.message == "boom from the worker", "pthread result struct");
}

// ============================================================
// SECTION 6 : ERROR REPORTING — RETURN CODES vs EXCEPTIONS
// ============================================================
//
//  pthread functions return an error NUMBER (0 = success). They do not set
//  errno. Ignoring the return value silently ignores the failure.
//  std::thread reports failures by throwing std::system_error.
//
//  The demo uses one trigger for both: a thread trying to join ITSELF,
//  which would deadlock. Both layers detect it:
//     pthread_join(pthread_self(), ...)  -> returns EDEADLK
//     t.join() called from t's own body  -> throws system_error(EDEADLK)
//
//  Other real failures: pthread_create returns EAGAIN when the system cannot
//  create another thread (resource limit, no memory for the stack);
//  std::thread's constructor then throws
//  system_error(errc::resource_unavailable_try_again).

static void* selfJoinWorker(void* raw)
{
    int* rc = static_cast<int*>(raw);
    *rc = pthread_join(pthread_self(), nullptr);
    return nullptr;
}

void section6_error_reporting()
{
#if defined(__SANITIZE_THREAD__)
    // ThreadSanitizer's pthread_join interceptor aborts when a thread joins
    // itself (a TSan limitation, not a bug in the program), so this demo is
    // skipped in the -fsanitize=thread build.
    cout << "skipped under ThreadSanitizer (its pthread_join interceptor cannot handle a self-join)\n";
    return;
#endif
    int pthreadRc = 0;
    pthread_t th;
    check(pthread_create(&th, nullptr, selfJoinWorker, &pthreadRc) == 0, "pthread_create");
    check(pthread_join(th, nullptr) == 0, "pthread_join");
    cout << "pthread_join(self) returned : " << pthreadRc << " (" << strerror(pthreadRc) << ")\n";
    check(pthreadRc == EDEADLK, "pthread_join(self) must return EDEADLK");

    promise<void> gate;
    shared_future<void> gateFuture = gate.get_future().share();
    bool threw = false;
    int codeValue = 0;
    string message;

    thread t;
    t = thread([&]()
    {
        gateFuture.wait();                        // wait until main finished assigning `t`
        try
        {
            t.join();                             // a thread joining itself
        }
        catch (const system_error& e)
        {
            threw = true;
            codeValue = e.code().value();
            message = e.code().message();
        }
    });
    gate.set_value();
    t.join();

    cout << "std::thread::join() from itself threw std::system_error: code=" << codeValue
         << " (" << message << ")\n";
    check(threw && codeValue == EDEADLK, "std::thread::join from itself must throw system_error(EDEADLK)");
}

// ============================================================
// SECTION 7 : LIFETIME AND RAII
// ============================================================
//
//  std::thread must be joined or detached before its destructor runs.
//  Destroying a JOINABLE std::thread calls std::terminate. (The committee
//  chose that over silently joining, which could hang, or silently
//  detaching, which could leave a thread using dead objects.)
//
//  pthread has no destructor at all: a joinable pthread you never join is a
//  leak (its resources stay until pthread_join/pthread_detach), but nothing
//  terminates.
//
//  std::jthread (C++20) joins in its destructor (after request_stop()),
//  which makes it the RAII thread you usually wanted.
//
//  std::thread is MOVE-ONLY. pthread_t is a plain copyable handle, compared
//  with pthread_equal, never with ==.

void section7_lifetime()
{
    thread t([]()
    {
    });
    cout << "joinable before join : " << (t.joinable() ? "true" : "false") << "\n";
    t.join();
    cout << "joinable after join  : " << (t.joinable() ? "true" : "false") << "\n";

    atomic<bool> finished{false};
    {
        jthread j([&finished]()
        {
            this_thread::sleep_for(50ms);
            finished = true;
        });
    }                                             // ~jthread: request_stop() + join()
    cout << "jthread joined by its destructor, work finished: " << (finished.load() ? "yes" : "no") << "\n";
    check(finished.load(), "~jthread must join");
}

// ============================================================
// SECTION 8 : THINGS ONLY pthread CAN DO (reach them via native_handle())
// ============================================================
//
//  std::thread deliberately exposes a small, portable subset. Anything
//  platform-specific is reached through t.native_handle(), which is a
//  pthread_t on Linux. Below: the five that matter most in practice.

// ---------------- 8a. Stack size ----------------
//
//  std::thread has NO way to pass thread attributes at creation.
//  Options for a custom stack size:
//    - call pthread_create directly with a pthread_attr_t
//    - pthread_setattr_default_np(): change the default attributes for
//      EVERY thread created afterwards, including std::thread (GNU extension)
//    - setrlimit(RLIMIT_STACK) before the process starts (glibc reads it at
//      startup for the default thread stack size)

static size_t stackSizeOfCallingThread()
{
    pthread_attr_t attr;
    check(pthread_getattr_np(pthread_self(), &attr) == 0, "pthread_getattr_np");
    void* addr = nullptr;
    size_t size = 0;
    pthread_attr_getstack(&attr, &addr, &size);
    pthread_attr_destroy(&attr);
    return size;
}

static void* reportStackSize(void* raw)
{
    *static_cast<size_t*>(raw) = stackSizeOfCallingThread();
    return nullptr;
}

void section8a_stack_size()
{
    size_t stdDefault = 0;
    thread t1([&stdDefault]()
    {
        stdDefault = stackSizeOfCallingThread();
    });
    t1.join();

    size_t pthreadCustom = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    check(pthread_attr_setstacksize(&attr, 1024 * 1024) == 0, "pthread_attr_setstacksize");
    pthread_t th;
    check(pthread_create(&th, &attr, reportStackSize, &pthreadCustom) == 0, "pthread_create");
    pthread_attr_destroy(&attr);
    check(pthread_join(th, nullptr) == 0, "pthread_join");

    pthread_attr_t original;
    check(pthread_getattr_default_np(&original) == 0, "pthread_getattr_default_np");
    pthread_attr_t tuned;
    pthread_attr_init(&tuned);
    pthread_attr_setstacksize(&tuned, 1024 * 1024);
    check(pthread_setattr_default_np(&tuned) == 0, "pthread_setattr_default_np");

    size_t stdTuned = 0;
    thread t2([&stdTuned]()
    {
        stdTuned = stackSizeOfCallingThread();
    });
    t2.join();

    check(pthread_setattr_default_np(&original) == 0, "restore default attributes");
    pthread_attr_destroy(&tuned);
    pthread_attr_destroy(&original);

    cout << "std::thread, default attributes                  : " << stdDefault << " bytes\n";
    cout << "pthread_create with pthread_attr_setstacksize 1MB: " << pthreadCustom << " bytes\n";
    cout << "std::thread after pthread_setattr_default_np 1MB : " << stdTuned << " bytes\n";
    check(pthreadCustom < stdDefault, "the 1 MB pthread must have a smaller stack than the default");
    check(stdTuned < stdDefault, "std::thread must follow the new default attributes");
}

// ---------------- 8b. Name and CPU affinity ----------------
//
//  The thread name shows up in `top -H`, gdb and /proc/<pid>/task/<tid>/comm
//  (15 characters maximum). Affinity pins the thread to chosen CPUs.
//  Both are set from OUTSIDE the thread through its native handle.

void section8b_name_and_affinity()
{
    promise<void> gate;
    shared_future<void> gateFuture = gate.get_future().share();

    char nameInside[32] = {};
    int cpuInside = -1;

    thread t([&]()
    {
        gateFuture.wait();                        // wait until main has tuned this thread
        pthread_getname_np(pthread_self(), nameInside, sizeof(nameInside));
        cpuInside = sched_getcpu();
    });

    int renameRc = pthread_setname_np(t.native_handle(), "pc-worker");

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    int pinRc = pthread_setaffinity_np(t.native_handle(), sizeof(set), &set);

    gate.set_value();
    t.join();

    cout << "thread name read back inside the thread : " << nameInside << "\n";
    cout << "pthread_setaffinity_np returned          : " << pinRc << "\n";
    cout << "sched_getcpu() inside the pinned thread  : " << cpuInside << "\n";
    check(renameRc == 0 && strcmp(nameInside, "pc-worker") == 0, "thread name must be set");
    if (pinRc == 0)
    {
        check(cpuInside == 0, "a thread pinned to CPU 0 must run on CPU 0");
    }
}

// ---------------- 8c. Cancellation ----------------
//
//  pthread_cancel asks another thread to stop at its next CANCELLATION POINT
//  (sleep, read, write, pthread_cond_wait, ...). On glibc the target is
//  stopped by a forced stack unwind, so C++ destructors DO run. Join returns
//  PTHREAD_CANCELED.
//
//  HAZARD: catch (...) that does not rethrow swallows the forced unwind and
//  glibc aborts the process ("FATAL: exception not rethrown").
//
//  The C++ standard has no pthread_cancel. The portable replacement is
//  COOPERATIVE: std::stop_token (Section 9). It cannot interrupt a blocking
//  call that does not check the token, which is the price of safety.

struct DtorFlag
{
    atomic<bool>* flag;
    ~DtorFlag()
    {
        flag->store(true);
    }
};

static void* cancellableWorker(void* raw)
{
    DtorFlag guard{static_cast<atomic<bool>*>(raw)};
    sleep(30);                                    // a cancellation point
    return nullptr;                               // never reached when cancelled
}

void section8c_cancellation()
{
    atomic<bool> destructorRan{false};
    pthread_t th;
    check(pthread_create(&th, nullptr, cancellableWorker, &destructorRan) == 0, "pthread_create");
    check(pthread_cancel(th) == 0, "pthread_cancel");

    void* ret = nullptr;
    check(pthread_join(th, &ret) == 0, "pthread_join");

    cout << "join returned PTHREAD_CANCELED : " << (ret == PTHREAD_CANCELED ? "yes" : "no") << "\n";
    cout << "destructor ran during the unwind: " << (destructorRan.load() ? "yes" : "no") << "\n";
    check(ret == PTHREAD_CANCELED && destructorRan.load(), "cancellation must unwind and run destructors");
}

// ---------------- 8d. Robust mutex ----------------
//
//  If a thread (or, with PTHREAD_PROCESS_SHARED, a whole process) dies while
//  holding a normal mutex, the mutex stays locked forever and every waiter
//  hangs. A ROBUST mutex tells the next locker: lock returns EOWNERDEAD, the
//  new owner repairs the data, calls pthread_mutex_consistent, then unlocks.
//  std::mutex has no equivalent. This is why shared-memory IPC code still
//  uses pthread mutexes directly.

struct RobustCtx
{
    pthread_mutex_t mtx;
};

static void* lockAndExit(void* raw)
{
    RobustCtx* c = static_cast<RobustCtx*>(raw);
    pthread_mutex_lock(&c->mtx);
    return nullptr;                               // the thread ends still HOLDING the lock
}

void section8d_robust_mutex()
{
    RobustCtx ctx;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    check(pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST) == 0, "setrobust");
    check(pthread_mutex_init(&ctx.mtx, &attr) == 0, "pthread_mutex_init");
    pthread_mutexattr_destroy(&attr);

    pthread_t th;
    check(pthread_create(&th, nullptr, lockAndExit, &ctx) == 0, "pthread_create");
    check(pthread_join(th, nullptr) == 0, "pthread_join");

    int rc = pthread_mutex_lock(&ctx.mtx);        // the owner died holding it
    cout << "pthread_mutex_lock after the owner died returned: "
         << (rc == EOWNERDEAD ? "EOWNERDEAD (we now own it and must repair)" : "something else") << "\n";
    check(rc == EOWNERDEAD, "a robust mutex must report EOWNERDEAD");
    check(pthread_mutex_consistent(&ctx.mtx) == 0, "pthread_mutex_consistent");
    check(pthread_mutex_unlock(&ctx.mtx) == 0, "pthread_mutex_unlock");
    pthread_mutex_destroy(&ctx.mtx);
}

// ---------------- 8e. Thread-specific data ----------------
//
//  pthread_key_create / pthread_setspecific: a per-thread void* slot with an
//  optional destructor callback that runs at thread exit. Manual casts, a
//  limited number of keys (PTHREAD_KEYS_MAX).
//  thread_local (C++11 keyword): per-thread objects with real constructors
//  and destructors, no casts, no key bookkeeping. Prefer it.
//  Both work on a std::thread; the demo shows both destructors running.

static atomic<int> gKeyDtorRuns{0};
static atomic<int> gTlsDtorRuns{0};

static void keyDestructor(void* p)
{
    delete static_cast<int*>(p);
    gKeyDtorRuns.fetch_add(1);
}

struct TlsObject
{
    int value = 0;
    ~TlsObject()
    {
        gTlsDtorRuns.fetch_add(1);
    }
};

static thread_local TlsObject tTls;

void section8e_thread_specific_data()
{
    pthread_key_t key;
    check(pthread_key_create(&key, keyDestructor) == 0, "pthread_key_create");

    vector<thread> threads;
    for (int i = 1; i <= 3; ++i)
    {
        threads.emplace_back([key, i]()
        {
            pthread_setspecific(key, new int(i));     // per-thread slot, manual
            tTls.value = i * 10;                       // per-thread object, automatic
        });
    }
    for (thread& t : threads)
    {
        t.join();
    }
    pthread_key_delete(key);

    cout << "pthread_key destructors run at thread exit : " << gKeyDtorRuns.load() << " of 3\n";
    cout << "thread_local destructors run at thread exit: " << gTlsDtorRuns.load() << " of 3\n";
    check(gKeyDtorRuns.load() == 3 && gTlsDtorRuns.load() == 3, "both per-thread mechanisms must clean up");
}

// ---------------- 8f. More pthread-only features (not demonstrated) ----------------
//
//  - Scheduling: pthread_attr_setschedpolicy (SCHED_FIFO / SCHED_RR) and
//    priorities; usually needs privileges or CAP_SYS_NICE
//  - PTHREAD_PROCESS_SHARED mutexes and condition variables living in shared
//    memory, usable across processes
//  - Signals: pthread_sigmask (per-thread mask), pthread_kill (signal one
//    thread). The C++ standard has no thread-level signal API
//  - pthread_atfork: handlers around fork() in a multithreaded program
//  - Spinlocks (pthread_spin_lock) and pthread_barrier (C++20 has std::barrier,
//    still no std spinlock)
//  - pthread_getcpuclockid: per-thread CPU clock

// ============================================================
// SECTION 9 : THINGS std::thread / C++ GIVE YOU THAT pthread DOES NOT
// ============================================================
//
//  - Type-safe callables and arguments (lambdas, member functions, move-only)
//  - std::future / promise / async / packaged_task: values AND exceptions
//  - RAII locking: lock_guard, unique_lock, scoped_lock
//  - condition_variable with predicate overloads and std::chrono timeouts
//  - std::atomic and a real memory model: a data race is undefined behavior
//    and atomics take memory orders. POSIX specifies synchronization through
//    particular functions (mutex lock/unlock, ...) but not a language model
//  - thread_local with constructors and destructors
//  - jthread + stop_token: RAII join and cooperative cancellation
//  - hardware_concurrency(), this_thread::sleep_for / yield
//  - PORTABILITY: the same code builds on Linux, macOS and Windows
//
//  The demo is the portable counterpart of Section 8c: cancel a worker
//  COOPERATIVELY with a stop_token. Destructors run through normal control
//  flow, with no forced unwind and no catch(...) hazard.

void section9_cooperative_cancellation()
{
    cout << "hardware_concurrency() : " << thread::hardware_concurrency() << "\n";

    atomic<bool> destructorRan{false};
    {
        jthread worker([&destructorRan](stop_token st)
        {
            DtorFlag guard{&destructorRan};
            while (!st.stop_requested())
            {
                this_thread::sleep_for(1ms);
            }
        });
        this_thread::sleep_for(20ms);
        worker.request_stop();
    }                                             // ~jthread joins

    cout << "worker stopped by request_stop(), destructor ran: " << (destructorRan.load() ? "yes" : "no") << "\n";
    check(destructorRan.load(), "cooperative cancellation must run destructors");
}

// ============================================================
// SECTION 10 : COST — pthread_create vs std::thread
// ============================================================
//
//  Both pay for the same clone3 call and the same stack mapping. std::thread
//  adds one small heap allocation (the state object that holds the callable
//  and its copied arguments) and a trampoline call. Thread creation cost is
//  dominated by the kernel, so the two are close.
//
//  Measured with -O2 on a 1-core sandbox (create + join an empty thread,
//  average of 2000, three separate runs):
//      pthread_create + pthread_join : 6.5, 7.4, 6.9 us per thread
//      std::thread + join            : 11.4, 7.8, 6.9 us per thread
//  The std::thread numbers are noisier and slightly higher on average, but
//  the gap is within run-to-run noise: do not read it as a real difference.
//  Run it on your machine; absolute numbers depend on the kernel and CPU.

static void* emptyFn(void*)
{
    return nullptr;
}

void section10_creation_cost()
{
    constexpr int N = 2000;

    auto t0 = chrono::steady_clock::now();
    for (int i = 0; i < N; ++i)
    {
        pthread_t th;
        check(pthread_create(&th, nullptr, emptyFn, nullptr) == 0, "pthread_create");
        check(pthread_join(th, nullptr) == 0, "pthread_join");
    }
    auto t1 = chrono::steady_clock::now();

    for (int i = 0; i < N; ++i)
    {
        thread t([]()
        {
        });
        t.join();
    }
    auto t2 = chrono::steady_clock::now();

    double pthreadUs = chrono::duration<double, micro>(t1 - t0).count() / N;
    double stdUs = chrono::duration<double, micro>(t2 - t1).count() / N;

    cout << fixed << setprecision(1);
    cout << "pthread_create + pthread_join : " << pthreadUs << " us per thread\n";
    cout << "std::thread + join            : " << stdUs << " us per thread\n";
    cout << defaultfloat;
}

// ============================================================
// SECTION 11 : MIXING THE TWO — RULES
// ============================================================
//
//  The common, healthy pattern: create with std::thread, tune with the
//  native handle (name, affinity, priority). Rules:
//   - never pthread_join / pthread_detach a thread that a std::thread object
//     also manages: a double join is undefined behavior
//   - std::mutex on libstdc++ is a pthread_mutex_t (native_handle()), but the
//     standard does not promise it. Use std::condition_variable with
//     std::mutex, or pthread_cond_t with pthread_mutex_t, not a mix
//   - an exception must not escape either kind of start routine
//   - pthread_exit() in C++ code is a forced unwind: avoid it, just return
//   - compile and link with -pthread. On glibc older than 2.34, using
//     std::thread without it fails at run time with
//     "Enable multithreading to use std::thread: Operation not permitted"
//   - fork() in a multithreaded process: only the calling thread exists in
//     the child, and locks held by other threads stay locked forever.
//     pthread_atfork helps; std has no equivalent

// ============================================================
// SECTION 12 : DECISION GUIDE
// ============================================================
//
//  Use std::thread (or jthread) when:
//    - you want portable, type-safe, exception-aware code
//    - you need results or errors back: promise / future / async
//    - you want RAII locks, condition_variable with predicates, atomics
//    - this is ordinary application code
//
//  Use pthread directly (or std::thread + native_handle()) when:
//    - you need thread attributes at creation (stack size, detach state, scheduling)
//    - you need affinity, names, real-time priority
//    - you need cancellation, robust mutexes, process-shared synchronization
//    - you need thread-level signal handling (pthread_sigmask, pthread_kill)
//    - you call a C library or framework that hands you pthread_t or expects pthread functions
//    - the target has no usable C++ standard library threading
//
//  Rule of thumb: write std::thread first, drop to native_handle() for the
//  one platform feature you need, and only use raw pthread_create when you
//  need attributes at creation time.

// ============================================================
// SECTION 13 : COMMON BUGS
// ============================================================
//
//   1. Destroying a joinable std::thread (std::terminate)
//   2. Passing &local to pthread_create and letting local die
//   3. Expecting std::thread to copy a reference argument: no std::ref means
//      a compile error, std::ref means YOU own the lifetime
//   4. Ignoring pthread return codes (they do not set errno)
//   5. Letting an exception escape a thread function
//   6. Joining the same thread twice (pthread_join twice, or both APIs)
//   7. catch (...) that swallows pthread_cancel's forced unwind (abort)
//   8. Comparing pthread_t with == instead of pthread_equal
//   9. Mixing pthread_cond_t with std::mutex (or the reverse)
//  10. Thread names longer than 15 characters: pthread_setname_np fails with ERANGE
//  11. Forgetting -pthread on older glibc
//  12. Detaching a thread that still uses objects owned by the creator

// ============================================================
// SECTION 14 : 5-YEAR EXPERIENCE INTERVIEW QUESTION BANK
// ============================================================
//
//  Q1. Is std::thread a replacement for pthread?
//  A1. No. It is a portable abstraction over the platform's native threads.
//      On Linux with libstdc++ it IS pthreads underneath (Section 2).
//
//  Q2. How can you prove std::thread uses pthread on Linux?
//  A2. thread::native_handle_type is pthread_t; libstdc++.so imports
//      pthread_create/join/detach; strace shows the same clone3 flags and
//      stack size for both.
//
//  Q3. How do you set the stack size of a std::thread?
//  A3. You cannot at creation. Use pthread_create with an attribute, or
//      pthread_setattr_default_np before creating the std::thread (GNU
//      extension, Section 8a), or RLIMIT_STACK at process start.
//
//  Q4. What happens if a joinable std::thread is destroyed?
//  A4. std::terminate. jthread joins instead.
//
//  Q5. How do you get a return value out of a thread?
//  A5. pthread: the void* from pthread_join, or an output field.
//      std::thread: promise/future, packaged_task or async; the future also
//      carries exceptions.
//
//  Q6. Why does std::thread need std::ref for reference parameters?
//  A6. Arguments are copied into thread-owned storage first. A reference
//      parameter would bind to that hidden copy, so the compiler rejects it.
//      std::ref opts in to sharing the original.
//
//  Q7. Difference between pthread_t, std::thread::id and gettid()?
//  A7. pthread_t: user-space handle. std::thread::id: C++ wrapper around it.
//      gettid(): kernel thread ID. getpid() is the same for all threads.
//
//  Q8. pthread_cancel vs stop_token?
//  A8. pthread_cancel stops the target at a cancellation point by forced
//      unwinding (destructors run, catch(...) without rethrow aborts).
//      stop_token is cooperative: the thread checks it and returns normally.
//      It cannot interrupt a blocking call that ignores the token.
//
//  Q9. When would you use a robust mutex?
//  A9. When a lock holder can die (crash, cancel) and the data protected by
//      the lock must be repaired, typically mutexes in shared memory between
//      processes. std::mutex has nothing like it.
//
// Q10. Can you mix pthread mutexes and std::condition_variable?
// A10. Not portably. libstdc++'s std::mutex wraps pthread_mutex_t, but the
//      standard does not guarantee it. Stay inside one family.
//
// Q11. Is std::thread slower than pthread_create?
// A11. Slightly: one extra heap allocation and a trampoline. Both issue the
//      same clone3. Creation cost is dominated by the kernel (Section 10).
//
// Q12. thread_local vs pthread_key?
// A12. thread_local gives per-thread objects with constructors/destructors,
//      no casts, no key limit. pthread_key is a void* slot with an optional
//      destructor callback. Prefer thread_local.
//
// Q13. When do you still need raw pthreads in C++?
// A13. Attributes at creation, affinity, scheduling policy, cancellation,
//      robust or process-shared sync, thread-level signals, C library interop.
//
// Q14. Does std::thread work on Windows? And pthread?
// A14. std::thread yes (Win32 threads underneath). pthread is not native;
//      it needs a compatibility layer such as winpthreads.
//
// Q15. Why are errors reported differently?
// A15. POSIX is a C API: return codes. C++ uses exceptions: std::system_error.
//      Same underlying errno values either way (Section 6).

// ============================================================
// SECTION 15 : FUNCTION REFERENCE
// ============================================================

// ---------------- mapping table: pthread -> C++ ----------------
//
//  pthread                              C++ equivalent
//  -----------------------------------  ---------------------------------------
//  pthread_create                       std::thread constructor / std::jthread
//  pthread_join                         thread::join
//  pthread_detach                       thread::detach
//  pthread_self                         std::this_thread::get_id
//  pthread_equal                        thread::id ==
//  pthread_exit                         just return from the function
//  pthread_cancel                       stop_token (cooperative), no equivalent
//  pthread_attr_*                       none at creation; native_handle() or
//                                       pthread_setattr_default_np
//  pthread_mutex_*                      std::mutex / recursive_mutex / timed_mutex
//  pthread_cond_*                       std::condition_variable
//  pthread_rwlock_*                     std::shared_mutex
//  pthread_once                         std::call_once
//  pthread_key_*                        thread_local
//  pthread_spin_*                       none (use std::atomic_flag)
//  pthread_barrier_*                    std::barrier (C++20)
//  sem_* (POSIX semaphores)             std::counting_semaphore (C++20)
//  robust / process-shared mutex        none
//  pthread_sigmask / pthread_kill       none

// FUNCTION      : pthread_create(&th, &attr, fn, arg)
// DESCRIPTION   : Start a thread running fn(arg). attr may be nullptr for defaults
// RETURN VALUE  : int — 0 on success, an error number (EAGAIN, EINVAL, ...) on failure
// HOW TO USE    :
//     pthread_t th;
//     int rc = pthread_create(&th, nullptr, worker, &args);
//     if (rc != 0)
//     {
//         /* handle it */
//     }

// FUNCTION      : pthread_join(th, &ret)
// DESCRIPTION   : Wait for th to finish and collect its void* result
// RETURN VALUE  : int — 0, EDEADLK (self-join), EINVAL, ESRCH
// HOW TO USE    :
//     void* ret = nullptr;
//     pthread_join(th, &ret);

// FUNCTION      : pthread_attr_setstacksize(&attr, bytes)
// DESCRIPTION   : Choose the stack size for threads created with this attribute
// RETURN VALUE  : int — 0 or EINVAL (below PTHREAD_STACK_MIN)
// HOW TO USE    :
//     pthread_attr_t attr;
//     pthread_attr_init(&attr);
//     pthread_attr_setstacksize(&attr, 1024 * 1024);

// FUNCTION      : pthread_setname_np(th, "name")
// DESCRIPTION   : Name a thread (15 characters plus NUL); visible in top -H and gdb
// RETURN VALUE  : int — 0 or ERANGE
// HOW TO USE    :
//     pthread_setname_np(t.native_handle(), "worker-1");

// FUNCTION      : pthread_setaffinity_np(th, size, &cpuset)
// DESCRIPTION   : Restrict a thread to a set of CPUs
// RETURN VALUE  : int — 0 or an error number
// HOW TO USE    :
//     cpu_set_t set;
//     CPU_ZERO(&set);
//     CPU_SET(2, &set);
//     pthread_setaffinity_np(t.native_handle(), sizeof(set), &set);

// FUNCTION      : pthread_cancel(th)
// DESCRIPTION   : Request cancellation; acted on at the next cancellation point
// RETURN VALUE  : int — 0 or ESRCH
// HOW TO USE    :
//     pthread_cancel(th);
//     void* ret;
//     pthread_join(th, &ret);       // ret == PTHREAD_CANCELED

// ============================================================
// SECTION 16 : PRACTICE
// ============================================================
//
//  Exercises:
//   - write an RAII class that wraps pthread_create / pthread_join with
//     std::thread-like semantics (move-only, terminate if joinable)
//   - write a thread pool twice: once with std::thread, once with raw pthreads
//   - pin worker threads to CPUs with pthread_setaffinity_np and watch them in top -H
//   - build a robust-mutex shared-memory counter between two processes
//
//  LeetCode concurrency set:
//    LC   #1114 Print in Order                   Easy
//    LC   #1115 Print FooBar Alternately         Medium
//    LC   #1116 Print Zero Even Odd              Medium
//    LC   #1117 Building H2O                     Medium
//    LC   #1195 FizzBuzz Multithreaded           Medium
//    LC   #1226 The Dining Philosophers          Medium
//    LC   #1188 Design Bounded Blocking Queue (premium)   Medium

// ============================================================
// SECTION 17 : QUICK-FIRE ORAL DRILL (2-minute recap)
// ============================================================
//
// what they are  -> pthread = POSIX C API (NPTL in glibc); std::thread = C++11 wrapper over it
// proof          -> native_handle_type is pthread_t; same clone3 flags; libstdc++ imports pthread_create
// identifiers    -> pthread_t (handle), std::thread::id (wrapper), gettid (kernel), getpid (shared)
// call style     -> void*(*)(void*) + one void* vs any callable with typed arguments
// arguments      -> pthread: you manage lifetime; std::thread: copies, std::ref to share
// results        -> void* / output field vs promise, future, async (value AND exception)
// errors         -> pthread returns codes, no errno; std::thread throws system_error
// lifetime       -> joinable ~thread = terminate; jthread joins; pthread has no destructor
// pthread only   -> attributes, affinity, names, scheduling, cancellation, robust mutex, signals
// C++ only       -> RAII locks, atomics + memory model, future/async, thread_local, stop_token
// stack size     -> not settable on std::thread; pthread_attr or pthread_setattr_default_np
// cancellation   -> pthread_cancel forced unwind (catch(...) hazard) vs cooperative stop_token
// mixing         -> create with std::thread, tune via native_handle, never double-join
// build          -> always -pthread

// ============================================================
// SECTION 18 : FULL WORKING DEMO
// ============================================================

int main()
{
    cout << "===== std::thread vs pthread — Complete Demo =====\n";

    banner("Section 2: std::thread IS pthread (handle, ids)");
    section2_stdthread_is_pthread();

    banner("Section 3: the same job, two ways");
    section3_same_job_two_ways();

    banner("Section 4: passing arguments");
    section4_arguments();

    banner("Section 5: return values and exceptions");
    section5_return_values_and_exceptions();

    banner("Section 6: error reporting (return code vs exception)");
    section6_error_reporting();

    banner("Section 7: lifetime and RAII");
    section7_lifetime();

    banner("Section 8a: stack size");
    section8a_stack_size();

    banner("Section 8b: thread name and CPU affinity via native_handle()");
    section8b_name_and_affinity();

    banner("Section 8c: pthread_cancel runs destructors");
    section8c_cancellation();

    banner("Section 8d: robust mutex");
    section8d_robust_mutex();

    banner("Section 8e: pthread_key vs thread_local");
    section8e_thread_specific_data();

    banner("Section 9: cooperative cancellation with stop_token");
    section9_cooperative_cancellation();

    banner("Section 10: creation cost");
    section10_creation_cost();

    cout << "\n===== Done =====\n";
    return 0;
}

// ============================================================
// END OF NOTES
// Compile (one line):
//   g++ -std=c++20 -O0 -g -Wall -Wextra -Wshadow -Wsign-conversion -pthread -fsanitize=address,undefined diffBetween_Thread_And_pthread.cpp -o thread_vs_pthread
// Thread check (one line, cannot be combined with address):
//   g++ -std=c++20 -O1 -g -pthread -fsanitize=thread diffBetween_Thread_And_pthread.cpp -o thread_vs_pthread_tsan
// ============================================================

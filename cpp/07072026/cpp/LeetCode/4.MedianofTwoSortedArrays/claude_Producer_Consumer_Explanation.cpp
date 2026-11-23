// ============================================================
//  FILE  : Producer_Consumer_Explanation.cpp
//  TOPIC : Producer-Consumer problem in C++ — concepts, four solutions,
//          shutdown, thread pool, interview bank
//  LEVEL : Concept -> senior interview depth (C++ Linux programmer)
//  STD   : C++20 (counting_semaphore, jthread, stop_token)
//  VERIFIED: every class in this file was compiled with
//            -Wall -Wextra -Wshadow -Wsign-conversion (zero warnings),
//            run under -fsanitize=address,undefined, and run under
//            -fsanitize=thread (zero reports).
//            Timing numbers quoted in comments were measured in a 1-core
//            sandbox: compare them with each other, not with your machine.
// ============================================================

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <semaphore>
#include <stop_token>
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

// CPU time consumed by the CALLING thread only.
static double threadCpuMs()
{
    timespec ts;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    return static_cast<double>(ts.tv_sec) * 1000.0 + static_cast<double>(ts.tv_nsec) / 1.0e6;
}

// ============================================================
// SECTION 1 : THE PROBLEM
// ============================================================
//
//  Producers create items. Consumers process them. A shared bounded buffer
//  sits between them:
//
//   producers              bounded buffer                consumers
//   +------+  push   +---+---+---+---+---+   pop   +------+
//   |  P1  | ------> | a | b | c |   |   | ------> |  C1  |
//   |  P2  | ------> +---+---+---+---+---+ ------> |  C2  |
//   +------+     full: producer sleeps    empty: consumer sleeps
//
//  Rules:
//   1. Mutual exclusion  only one thread touches the buffer internals at a time
//   2. No overflow       a producer waits while the buffer is full
//   3. No underflow      a consumer waits while the buffer is empty
//   4. Clean shutdown    when the work ends, sleeping threads are woken and exit
//
//  Where it shows up: thread-pool task queues, log pipelines, a network thread
//  handing packets to a parser thread, video frame pipelines, message brokers.
//  A thread pool IS a producer-consumer system (Section 12).

// ============================================================
// SECTION 2 : ATTEMPT 1 — NO SYNCHRONIZATION
// ============================================================
//
//     std::queue<int> q;                   // shared, NOT protected
//     producer thread:  q.push(i);
//     consumer thread:  if (!q.empty()) { q.pop(); }
//
//  ThreadSanitizer report from a real run (abridged):
//
//     WARNING: ThreadSanitizer: data race
//       ... std::deque<int>::push_back(...)    race.cpp:13   (producer thread)
//       ... std::deque<int>::empty() const     race.cpp:23   (consumer thread)
//       Location is global 'q'
//
//  Without TSan this is undefined behavior: it can crash, hang because pushes
//  are lost, or appear to work. The racy program is deliberately NOT part of
//  this file.

// ============================================================
// SECTION 3 : ATTEMPT 2 — LOCK IT, THEN POLL (BUSY-WAIT)
// ============================================================
//
//  A mutex removes the race, but a consumer that polls
//  ("lock, check, unlock, repeat") burns a whole core while it waits.
//  A blocked condition-variable wait sleeps in the kernel (on Linux a futex
//  wait, reached through pthread_cond_wait) and uses no CPU until notified.
//
//  Measured (1-core sandbox), both waiting 1 second for an item that
//  arrived late:
//      busy-wait consumer  burned   995.6 ms of CPU
//      cv.wait   consumer  burned     0.0 ms of CPU
//  The demo below waits 0.5 second and prints its own numbers.

void section3_spin_vs_wait()
{
    mutex m;
    condition_variable cv;
    bool ready = false;

    double spinMs = 0.0;
    double waitMs = 0.0;

    // consumer 1: busy-waits
    thread spinner([&]()
    {
        double start = threadCpuMs();
        while (true)
        {
            lock_guard<mutex> lock(m);
            if (ready)
            {
                break;
            }
        }
        spinMs = threadCpuMs() - start;
    });

    // consumer 2: sleeps on the condition variable
    thread waiter([&]()
    {
        double start = threadCpuMs();
        unique_lock<mutex> lock(m);
        cv.wait(lock, [&]()
        {
            return ready;
        });
        waitMs = threadCpuMs() - start;
    });

    this_thread::sleep_for(500ms);           // the "producer" is slow
    {
        lock_guard<mutex> lock(m);
        ready = true;
    }
    cv.notify_all();

    spinner.join();
    waiter.join();

    cout << fixed << setprecision(1);
    cout << "waited 0.5 s for an item that arrived late\n";
    cout << "  busy-wait consumer burned " << setw(7) << spinMs << " ms of CPU\n";
    cout << "  cv.wait   consumer burned " << setw(7) << waitMs << " ms of CPU\n";
    cout << defaultfloat;

    check(spinMs > waitMs, "a busy-wait consumer must burn more CPU than a sleeping one");
}

// ============================================================
// SECTION 4 : CONDITION VARIABLE BASICS
// ============================================================
//
//  A condition variable is a DOORBELL, not state. It lets a thread sleep until
//  another thread signals that shared state MAY have changed. It remembers
//  nothing. The truth lives in your own variables, protected by a mutex.
//
//  cv.wait(lock, pred) is exactly:
//       while (!pred())
//       {
//           cv.wait(lock);        // atomically unlock + sleep, re-lock on wake
//       }
//
//  WHY THE PREDICATE IS MANDATORY
//   - Spurious wakeup : the standard allows a wait to return with no notify.
//   - Lost wakeup     : a notify with nobody waiting is simply dropped.
//  The predicate form checks the state FIRST, so it never depends on having
//  been awake at the right moment.
//
//  WHY unique_lock AND NOT lock_guard
//   wait() must release the mutex while sleeping and re-acquire it on wake,
//   so it needs a lock object it can unlock and lock again.
//
//  THE LOST-UPDATE GAP (why shared state changes only under the mutex)
//   If `closed = true` is written WITHOUT the mutex (even on an atomic):
//       consumer:  locks, predicate false ---------------> (about to sleep)
//       producer:               sets flag, notifies --+     <- lands in the gap
//       consumer:                                   sleeps forever
//   Take the mutex for the write, then notify.

void section4_lost_wakeup()
{
    mutex m;
    condition_variable cv;
    bool ready = false;

    // the producer signals IMMEDIATELY, before any consumer is waiting
    thread producer([&]()
    {
        {
            lock_guard<mutex> lock(m);
            ready = true;
        }
        cv.notify_one();
    });
    producer.join();

    // the consumer arrives late
    {
        unique_lock<mutex> lock(m);
        cv_status status = cv.wait_for(lock, 300ms);               // NO predicate
        cout << "wait without predicate : "
             << (status == cv_status::timeout ? "TIMED OUT (the notification was lost)" : "woke up")
             << "\n";
    }
    {
        unique_lock<mutex> lock(m);
        bool ok = cv.wait_for(lock, 300ms, [&]()
        {
            return ready;
        });
        cout << "wait with predicate    : "
             << (ok ? "returned at once (state was already true)" : "timed out")
             << "\n";
        check(ok, "the predicate form must return immediately when the state is already true");
    }
}

// ============================================================
// SECTION 5 : THE SOLUTION — ONE MUTEX, TWO CONDITION VARIABLES
// ============================================================
//
//  - the MUTEX protects the state (the queue and the `closed` flag)
//  - notFull   : producers sleep here while the queue is full
//  - notEmpty  : consumers sleep here while the queue is empty
//
//  DESIGN DECISIONS
//   * TWO condition variables. With ONE shared CV and notify_one, a producer's
//     notification can wake ANOTHER producer; it sees "full" and goes back to
//     sleep while the consumer that could have progressed keeps sleeping.
//     (The alternative is notify_all, which wakes everyone every time.)
//   * notify_one after a push/pop (one item or one slot changed hands);
//     notify_all in close() (the state changed for EVERY waiter).
//   * pop() is ONE atomic operation that returns the item. Never expose
//     empty() / front() / pop() as three separately locked calls: another
//     thread can slip in between the check and the act (a TOCTOU race).
//   * pop() returns nullopt only when the queue is closed AND drained, so
//     close() means "no more input", not "throw away what is queued".
//   * push() returns false once the queue is closed (the item is not stored).
//   * notify AFTER unlocking avoids waking a thread that immediately blocks on
//     the mutex. Notifying while holding the lock is also correct, and is the
//     safer choice if the woken thread may destroy the queue.
//   * tryPush takes the item by REFERENCE and moves from it only on success,
//     so a failed non-blocking push does not destroy a move-only item.

enum class PopStatus
{
    Ok,
    Timeout,
    Closed
};

static const char* statusName(PopStatus s)
{
    switch (s)
    {
        case PopStatus::Ok:
            return "Ok";
        case PopStatus::Timeout:
            return "Timeout";
        case PopStatus::Closed:
            return "Closed";
    }
    return "?";
}

template <typename T>
class BoundedQueue
{
    queue<T> items;
    const size_t capacity;
    bool closed = false;

    mutex m;
    condition_variable notFull;           // producers sleep here
    condition_variable notEmpty;          // consumers sleep here

    size_t producerBlocks = 0;            // stats: pushes that had to sleep
    size_t consumerBlocks = 0;            // stats: pops that had to sleep

public:
    struct Stats
    {
        size_t producerBlocks;
        size_t consumerBlocks;
    };

    explicit BoundedQueue(size_t cap)
        : capacity(cap)
    {
    }

    // Blocking. false => the queue was closed, the item was NOT inserted.
    bool push(T item)
    {
        unique_lock<mutex> lock(m);

        if (items.size() >= capacity && !closed)
        {
            ++producerBlocks;
        }

        notFull.wait(lock, [this]()
        {
            return items.size() < capacity || closed;
        });

        if (closed)
        {
            return false;
        }

        items.push(move(item));
        lock.unlock();                    // notify without holding the lock
        notEmpty.notify_one();
        return true;
    }

    // Blocking. nullopt => the queue is closed AND fully drained.
    optional<T> pop()
    {
        unique_lock<mutex> lock(m);

        if (items.empty() && !closed)
        {
            ++consumerBlocks;
        }

        notEmpty.wait(lock, [this]()
        {
            return !items.empty() || closed;
        });

        if (items.empty())
        {
            return nullopt;
        }

        T item = move(items.front());
        items.pop();
        lock.unlock();
        notFull.notify_one();
        return item;
    }

    // Non-blocking. false => full or closed. `item` is moved from ONLY on success.
    bool tryPush(T& item)
    {
        unique_lock<mutex> lock(m);

        if (closed || items.size() >= capacity)
        {
            return false;
        }

        items.push(move(item));
        lock.unlock();
        notEmpty.notify_one();
        return true;
    }

    // Blocks at most `timeout`. Ok => `out` holds an item.
    PopStatus popFor(T& out, chrono::milliseconds timeout)
    {
        unique_lock<mutex> lock(m);

        bool ready = notEmpty.wait_for(lock, timeout, [this]()
        {
            return !items.empty() || closed;
        });

        if (!ready)
        {
            return PopStatus::Timeout;
        }

        if (items.empty())
        {
            return PopStatus::Closed;
        }

        out = move(items.front());
        items.pop();
        lock.unlock();
        notFull.notify_one();
        return PopStatus::Ok;
    }

    // "No more input". Wakes EVERY sleeper so it re-checks its predicate.
    void close()
    {
        {
            lock_guard<mutex> lock(m);
            closed = true;
        }
        notFull.notify_all();
        notEmpty.notify_all();
    }

    Stats stats()
    {
        lock_guard<mutex> lock(m);
        return Stats{producerBlocks, consumerBlocks};
    }
};

// ============================================================
// SECTION 6 : BASIC USAGE — ONE PRODUCER, ONE CONSUMER
// ============================================================
//
//  The consumer loop `while (optional v = q.pop())` ends by itself when the
//  queue is closed and drained. The thread that finishes the work calls close().

void section6_basic_demo()
{
    BoundedQueue<int> q(3);
    vector<int> received;

    thread consumer([&]()
    {
        while (optional<int> v = q.pop())
        {
            received.push_back(*v);
        }
    });

    for (int i = 0; i < 10; ++i)
    {
        q.push(i);
    }
    q.close();
    consumer.join();

    cout << "consumer received:";
    for (int v : received)
    {
        cout << " " << v;
    }
    cout << "\n";

    check(received.size() == 10, "all 10 items must arrive");
    for (size_t i = 0; i < received.size(); ++i)
    {
        check(received[i] == static_cast<int>(i), "a single consumer sees FIFO order");
    }
}

// ============================================================
// SECTION 7 : MULTI-PRODUCER / MULTI-CONSUMER TEST HARNESS
// ============================================================
//
//  Three checks, every run:
//   - EXACTLY ONCE : a counter per item; every counter must end at 1
//   - ORDER        : one consumer must see each producer's items in increasing order
//                    (a FIFO queue guarantees this per producer, per consumer)
//   - TOTALS       : consumed count equals produced count
//
//  Measured output (3 producers, 4 consumers, 50000 items each; unoptimized
//  ASan build, so compare the rows with each other):
//
//   capacity=   1  producerBlocks=118178 consumerBlocks=105743  752 ms
//   capacity=  16  producerBlocks= 13709 consumerBlocks= 26816  291 ms
//   capacity=1024  producerBlocks=   189 consumerBlocks=  7056  155 ms
//
//  Reading it: at capacity 1 about 79% of pushes had to sleep (the queue is a
//  hand-off with a context switch per item); at 16 about 9%; at 1024 about
//  0.1% (the buffer absorbs bursts). A bounded queue also gives BACKPRESSURE:
//  if consumers fall behind, producers slow down instead of memory growing
//  without limit.

struct Item
{
    int producer;
    int seq;
};

static bool runScenario(size_t capacity, int producers, int consumers, int perProducer)
{
    BoundedQueue<Item> q(capacity);

    const size_t P = static_cast<size_t>(producers);
    const size_t N = static_cast<size_t>(perProducer);
    vector<atomic<int>> seen(P * N);          // how many times each item was consumed
    atomic<long long> consumed{0};
    atomic<bool> orderOk{true};

    auto t0 = chrono::steady_clock::now();

    vector<thread> producerThreads;
    for (int p = 0; p < producers; ++p)
    {
        producerThreads.emplace_back([&q, p, perProducer]()
        {
            for (int i = 0; i < perProducer; ++i)
            {
                q.push(Item{p, i});
            }
        });
    }

    vector<thread> consumerThreads;
    for (int c = 0; c < consumers; ++c)
    {
        consumerThreads.emplace_back([&]()
        {
            vector<int> last(P, -1);
            while (optional<Item> item = q.pop())
            {
                size_t p = static_cast<size_t>(item->producer);

                // one consumer must see each producer's items in increasing order
                if (item->seq <= last[p])
                {
                    orderOk = false;
                }
                last[p] = item->seq;

                seen[p * N + static_cast<size_t>(item->seq)].fetch_add(1);
                consumed.fetch_add(1);
            }
        });
    }

    for (thread& t : producerThreads)
    {
        t.join();
    }
    q.close();                                // producers are done: no more input
    for (thread& t : consumerThreads)
    {
        t.join();
    }

    auto ms = chrono::duration_cast<chrono::milliseconds>(
                  chrono::steady_clock::now() - t0).count();

    bool exactlyOnce = true;
    for (const atomic<int>& s : seen)
    {
        if (s.load() != 1)
        {
            exactlyOnce = false;
        }
    }

    BoundedQueue<Item>::Stats st = q.stats();
    cout << "capacity=" << setw(4) << capacity
         << "  consumed=" << consumed.load() << "/" << P * N
         << "  exactly-once=" << (exactlyOnce ? "yes" : "NO")
         << "  order=" << (orderOk.load() ? "ok" : "BROKEN")
         << "  producerBlocks=" << st.producerBlocks
         << " consumerBlocks=" << st.consumerBlocks
         << "  " << ms << " ms\n";

    return exactlyOnce && orderOk.load() && consumed.load() == static_cast<long long>(P * N);
}

void section7_mpmc_harness()
{
    cout << "3 producers, 4 consumers, 50000 items each:\n";
    check(runScenario(1, 3, 4, 50000), "capacity 1 scenario");
    check(runScenario(16, 3, 4, 50000), "capacity 16 scenario");
    check(runScenario(1024, 3, 4, 50000), "capacity 1024 scenario");
}

// ============================================================
// SECTION 8 : SHUTDOWN — THE HARD PART
// ============================================================
//
//  A queue needs a `closed` flag, and the wait predicates must include it:
//     push waits for: space  OR closed
//     pop  waits for: item   OR closed
//  After close():
//     push returns false             (no new input is accepted)
//     pop  keeps returning items     (close means "no more input", not "discard")
//     pop  returns nullopt           only when the queue is closed AND drained
//
//  THE CLASSIC BUG: nobody wakes the sleepers. A consumer blocked in pop() on
//  an empty queue sleeps forever and join() hangs. close() must notify_all.
//
//  WITH SEVERAL PRODUCERS, close() must run once, after the LAST producer
//  finishes. Count them:
//       if (activeProducers.fetch_sub(1) == 1)
//       {
//           q.close();
//       }
//  (The harness above simply joins every producer, then closes.)
//
//  The demo makes a thread block FIRST (it waits on the stats counter, not on
//  a sleep), then closes the queue and shows the sleeper is released.

void section8_shutdown()
{
    // --- a producer blocked on a FULL queue is released by close() ---
    {
        BoundedQueue<int> q(1);
        q.push(1);                                    // the queue is now full
        bool pushResult = true;

        thread blockedProducer([&]()
        {
            pushResult = q.push(2);                   // blocks: no free slot
        });

        while (q.stats().producerBlocks == 0)         // wait until it really is blocked
        {
            this_thread::yield();
        }
        q.close();
        blockedProducer.join();

        optional<int> first = q.pop();                // a closed queue still drains
        optional<int> second = q.pop();               // closed AND drained

        cout << "blocked producer released: push returned "
             << (pushResult ? "true" : "false") << "\n";
        cout << "drain after close: first pop = " << (first ? to_string(*first) : string("nullopt"))
             << ", second pop = " << (second ? to_string(*second) : string("nullopt")) << "\n";

        check(!pushResult, "push on a closed queue must return false");
        check(first.has_value() && *first == 1, "items queued before close are still delivered");
        check(!second.has_value(), "pop on a closed, drained queue returns nullopt");
    }

    // --- a consumer blocked on an EMPTY queue is released by close() ---
    {
        BoundedQueue<int> q(4);
        bool gotItem = true;

        thread blockedConsumer([&]()
        {
            optional<int> v = q.pop();                // blocks: the queue is empty
            gotItem = v.has_value();
        });

        while (q.stats().consumerBlocks == 0)
        {
            this_thread::yield();
        }
        q.close();
        blockedConsumer.join();

        cout << "blocked consumer released: pop returned "
             << (gotItem ? "an item" : "nullopt (closed and drained)") << "\n";
        check(!gotItem, "a consumer blocked on an empty queue must be released by close()");
    }
}

// ============================================================
// SECTION 9 : try-VARIANTS AND TIMED WAITS
// ============================================================
//
//  tryPush(item)        never blocks; false if full or closed
//  popFor(out, timeout) blocks at most `timeout`; tells you WHY it returned:
//                         Ok / Timeout / Closed
//  Returning a bare optional would be ambiguous: nullopt could mean either
//  "timed out" or "closed and drained". The status enum removes the doubt.
//  Use them for: shutdown polling, heartbeats, load shedding (drop work
//  instead of blocking when the queue is full).

void section9_try_and_timed()
{
    BoundedQueue<int> q(3);

    int accepted = 0;
    for (int i = 0; i < 5; ++i)
    {
        int item = i;
        if (q.tryPush(item))
        {
            ++accepted;
        }
    }
    cout << "tryPush: 5 attempts on a capacity-3 queue, accepted " << accepted << "\n";
    check(accepted == 3, "tryPush accepts exactly capacity items");

    int out = 0;
    int drained = 0;
    while (q.popFor(out, 10ms) == PopStatus::Ok)
    {
        ++drained;
    }
    cout << "popFor drained " << drained << " items, then the queue was empty and open\n";

    PopStatus open = q.popFor(out, 50ms);
    cout << "popFor on an empty OPEN queue   : " << statusName(open) << "\n";

    q.close();
    PopStatus closedStatus = q.popFor(out, 50ms);
    cout << "popFor on an empty CLOSED queue : " << statusName(closedStatus) << "\n";

    check(drained == 3, "all three items drained");
    check(open == PopStatus::Timeout, "empty and open must time out");
    check(closedStatus == PopStatus::Closed, "empty and closed must report Closed");
}

// ============================================================
// SECTION 10 : THE TEXTBOOK VERSION — TWO SEMAPHORES (C++20)
// ============================================================
//
//  Dijkstra's classic solution:
//     emptySlots : counts free slots   (starts at capacity)
//     fullSlots  : counts items        (starts at 0)
//     mutex      : protects the ring indices with many producers / consumers
//
//  The state lives in the semaphore counters, so there are no predicate loops.
//  Shutdown is awkward: use POISON PILLS (one sentinel per consumer).
//
//                      | condition variable            | semaphores
//   -------------------+-------------------------------+---------------------------
//   state lives in     | your variables + a predicate  | the semaphore counters
//   wait logic         | predicate loop                | none needed
//   shutdown           | close() flag, wake everyone   | poison pills
//   arbitrary waits    | easy (any predicate)          | counts only
//   timeouts           | wait_for with a predicate     | try_acquire_for
//   signaling thread   | any thread, under the mutex   | ANY thread may release
//                      |                               | (a semaphore has no owner)
//
//  With exactly one producer and one consumer the mutex can go away: each
//  index is touched by one thread and the semaphores order the memory accesses.

template <typename T>
class SemaphoreQueue
{
    vector<T> buf;                            // ring buffer; T must be default-constructible
    size_t head = 0;
    size_t tail = 0;
    counting_semaphore<> emptySlots;          // how many free slots remain
    counting_semaphore<> fullSlots;           // how many items are waiting
    mutex m;                                  // protects head/tail/buf with many threads

public:
    explicit SemaphoreQueue(size_t cap)
        : buf(cap), emptySlots(static_cast<ptrdiff_t>(cap)), fullSlots(0)
    {
    }

    void push(T item)
    {
        emptySlots.acquire();                 // sleep until a slot is free
        {
            lock_guard<mutex> lock(m);
            buf[tail] = move(item);
            tail = (tail + 1) % buf.size();
        }
        fullSlots.release();                  // one more item is available
    }

    T pop()
    {
        fullSlots.acquire();                  // sleep until an item exists
        T item;
        {
            lock_guard<mutex> lock(m);
            item = move(buf[head]);
            head = (head + 1) % buf.size();
        }
        emptySlots.release();                 // one more slot is free
        return item;
    }
};

static bool runSemaphoreDemo()
{
    constexpr int kProducers = 3;
    constexpr int kConsumers = 4;
    constexpr int kPerProducer = 50000;
    constexpr int kStop = -1;                 // poison pill: tells a consumer to exit

    SemaphoreQueue<int> q(16);

    vector<atomic<int>> seen(static_cast<size_t>(kProducers) * kPerProducer);
    atomic<long long> consumed{0};

    vector<thread> producers;
    for (int p = 0; p < kProducers; ++p)
    {
        producers.emplace_back([&q, p]()
        {
            for (int i = 0; i < kPerProducer; ++i)
            {
                q.push(p * kPerProducer + i);
            }
        });
    }

    vector<thread> consumers;
    for (int c = 0; c < kConsumers; ++c)
    {
        consumers.emplace_back([&]()
        {
            while (true)
            {
                int v = q.pop();
                if (v == kStop)
                {
                    break;
                }
                seen[static_cast<size_t>(v)].fetch_add(1);
                consumed.fetch_add(1);
            }
        });
    }

    for (thread& t : producers)
    {
        t.join();
    }
    for (int c = 0; c < kConsumers; ++c)
    {
        q.push(kStop);                        // exactly one pill per consumer
    }
    for (thread& t : consumers)
    {
        t.join();
    }

    bool exactlyOnce = true;
    for (const atomic<int>& s : seen)
    {
        if (s.load() != 1)
        {
            exactlyOnce = false;
        }
    }

    cout << "semaphore queue: consumed=" << consumed.load() << "/" << kProducers * kPerProducer
         << "  exactly-once=" << (exactlyOnce ? "yes" : "NO") << "\n";
    return exactlyOnce && consumed.load() == kProducers * kPerProducer;
}

void section10_semaphore_demo()
{
    check(runSemaphoreDemo(), "semaphore queue must deliver every item exactly once");
}

// ============================================================
// SECTION 11 : MODERN SHUTDOWN — jthread + stop_token (C++20)
// ============================================================
//
//  condition_variable_any can wait on a stop_token, so no separate `closed`
//  flag is needed:
//     cv.wait(lock, stoken, pred)
//       returns true  : pred() is true
//       returns false : a stop was requested AND pred() is still false
//  When a stop is requested the call returns pred(), so consumers still DRAIN
//  items that are already queued and exit only when the queue is empty.
//
//  jthread passes its own stop_token as the first argument when the callable
//  accepts one, and its destructor does request_stop() + join().
//  The plain std::condition_variable has no stop_token overload;
//  condition_variable_any carries some extra overhead, so use it when you
//  need the stop_token (or a lock type other than std::mutex).

class StoppableQueue
{
    queue<int> items;
    mutex m;
    condition_variable_any cv;

public:
    void push(int v)
    {
        {
            lock_guard<mutex> lock(m);
            items.push(v);
        }
        cv.notify_one();
    }

    // false => stop was requested AND nothing is left to drain
    bool pop(int& out, stop_token st)
    {
        unique_lock<mutex> lock(m);
        if (!cv.wait(lock, st, [this]()
        {
            return !items.empty();
        }))
        {
            return false;
        }
        out = items.front();
        items.pop();
        return true;
    }
};

static bool runStopTokenDemo()
{
    constexpr int kProducers = 3;
    constexpr int kConsumers = 4;
    constexpr int kPerProducer = 50000;

    StoppableQueue q;
    atomic<long long> consumed{0};

    vector<jthread> consumers;
    for (int c = 0; c < kConsumers; ++c)
    {
        // jthread passes its own stop_token as the first argument
        consumers.emplace_back([&](stop_token st)
        {
            int v = 0;
            while (q.pop(v, st))
            {
                consumed.fetch_add(1);
            }
        });
    }

    {
        vector<jthread> producers;
        for (int p = 0; p < kProducers; ++p)
        {
            producers.emplace_back([&q]()
            {
                for (int i = 0; i < kPerProducer; ++i)
                {
                    q.push(i);
                }
            });
        }
    }                                         // producer jthreads join here

    for (jthread& c : consumers)
    {
        c.request_stop();                     // wakes sleepers; they drain, then exit
    }
    consumers.clear();                        // jthread destructors join

    cout << "stop_token queue: consumed=" << consumed.load() << "/" << kProducers * kPerProducer << "\n";
    return consumed.load() == kProducers * kPerProducer;
}

void section11_stop_token_demo()
{
    check(runStopTokenDemo(), "stop_token queue must drain every item before the consumers exit");
}

// ============================================================
// SECTION 12 : A THREAD POOL IS A PRODUCER-CONSUMER SYSTEM
// ============================================================
//
//  Callers of submit() are the producers, the worker threads are the
//  consumers, and the queue holds std::function<void()> tasks.
//    - the bounded queue gives BACKPRESSURE: submit() blocks when the queue
//      is full instead of letting memory grow without limit
//    - the destructor calls close(); workers drain the remaining tasks, then
//      pop() returns nullopt and each worker exits
//    - an exception escaping a std::thread calls std::terminate, so the
//      worker loop catches it (in real code: log it and keep the worker alive)
//    - member order matters: `tasks` is declared BEFORE `workers`, so the
//      queue exists before any worker starts

class ThreadPool
{
    BoundedQueue<function<void()>> tasks;
    vector<thread> workers;

public:
    ThreadPool(size_t threads, size_t queueCapacity)
        : tasks(queueCapacity)
    {
        for (size_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this]()
            {
                while (optional<function<void()>> task = tasks.pop())
                {
                    try
                    {
                        (*task)();
                    }
                    catch (...)
                    {
                        // swallow and keep the worker alive (log it in real code)
                    }
                }
            });
        }
    }

    // Blocks when the queue is full. false => the pool is shutting down.
    bool submit(function<void()> task)
    {
        return tasks.push(move(task));
    }

    ~ThreadPool()
    {
        tasks.close();                        // workers drain what is queued, then exit
        for (thread& w : workers)
        {
            w.join();
        }
    }
};

void section12_thread_pool()
{
    atomic<int> done{0};
    {
        ThreadPool pool(4, 8);
        for (int i = 0; i < 1000; ++i)
        {
            pool.submit([&done]()
            {
                done.fetch_add(1);
            });
        }
    }                                         // destructor: close, drain, join

    cout << "thread pool ran " << done.load() << " of 1000 submitted tasks\n";
    check(done.load() == 1000, "the pool must run every submitted task before it exits");
}

// ============================================================
// SECTION 13 : DESIGN CHOICES
// ============================================================
//
//  Capacity   bounded (backpressure) vs unbounded (out-of-memory risk when
//             producers outrun consumers). Tune it: Section 7 shows capacity 1
//             sleeping on 79% of pushes and capacity 1024 on 0.1%.
//  Blocking   push / pop block; tryPush returns at once; popFor has a timeout
//  Lock cost  take several items per lock acquisition (batch pop)
//  Ordering   FIFO queue, or a priority queue behind the same locking
//  Scaling    one shared queue vs one queue per consumer / work stealing
//  Item type  move-only types (unique_ptr) work: push by value, std::move inside
//  Lock-free  exactly one producer and one consumer: the SPSC ring in
//             Circular_Queue_Explanation.cpp (Section 14) uses two atomics with
//             release/acquire and no locks. It spins/yields instead of sleeping,
//             so idle consumers need a blocking fallback (semaphore, eventfd).
//             A lock-free MULTI-producer queue is far harder (ABA, memory
//             reclamation) and rarely worth writing yourself.

// ============================================================
// SECTION 14 : COMMON BUGS
// ============================================================
//
//   1. wait without a predicate                   (lost + spurious wakeups)
//   2. `if` instead of `while` in a hand-rolled wait loop
//   3. One CV plus notify_one with two kinds of waiters
//   4. Forgetting to notify after changing state (e.g. no notFull after a pop)
//   5. Writing `closed` or any shared state outside the mutex
//   6. Shutdown that never wakes the sleepers, so join() hangs
//   7. Holding the lock while processing an item or doing I/O
//   8. Handing out references into the queue after unlocking
//   9. A plain bool or volatile as the shutdown flag instead of a mutex/atomic
//  10. empty() / front() / pop() as three separate calls (TOCTOU)
//  11. Destroying the queue while threads still use it
//  12. A producer that never calls close(): consumers block forever

// ============================================================
// SECTION 15 : 5-YEAR EXPERIENCE INTERVIEW QUESTION BANK
// ============================================================
//
//  Q1. Why does a condition variable need both a mutex and a predicate?
//  A1. The mutex protects the state the predicate reads, and wait() needs it to
//      unlock-and-sleep atomically. The predicate protects against spurious
//      wakeups and lost wakeups (Section 4).
//
//  Q2. Spurious wakeup vs lost wakeup?
//  A2. Spurious: a wait returns with no notify (allowed by the standard).
//      Lost: a notify arrives while nobody is waiting and is dropped. The
//      predicate loop handles both because it re-checks the real state.
//
//  Q3. Why two condition variables?
//  A3. With one CV and notify_one the wake-up can reach the wrong kind of
//      waiter (a producer waking a producer), leaving the useful thread asleep.
//      Two CVs make each notification reach the right role.
//
//  Q4. notify_one vs notify_all?
//  A4. notify_one when one waiter can use the change (one item pushed);
//      notify_all when the change affects everyone (close) or when waiters
//      wait on different predicates sharing one CV.
//
//  Q5. Is it legal to notify without holding the mutex?
//  A5. Yes. Changing the state must happen under the mutex; the notify itself
//      may happen before or after unlocking. After unlocking avoids a
//      wake-and-block bounce; while holding is safer if the woken thread may
//      destroy the object.
//
//  Q6. How do you shut a producer-consumer pipeline down cleanly?
//  A6. A closed flag read by both wait predicates, set under the mutex, then
//      notify_all. Producers stop and push returns false; consumers drain and
//      pop returns nullopt. Alternatives: poison pills (Section 10) or
//      stop_token (Section 11).
//
//  Q7. Condition variable vs semaphore?
//  A7. The CV is more flexible (any predicate, close, timeouts). Semaphores
//      encode the state as counters, need no predicate loop, and may be
//      released by any thread, but shutdown needs poison pills.
//
//  Q8. Mutex vs semaphore?
//  A8. A mutex has an owner (the locker unlocks) and protects a critical
//      section. A semaphore is a counter signaling resource availability and
//      has no owner.
//
//  Q9. Why make pop() a single operation that returns the item?
//  A9. empty() then front() then pop() as separate locked calls leaves gaps
//      where another thread can take the item (TOCTOU). One call that checks
//      and takes under one lock has no gap.
//
// Q10. What happens if the consumer's processing throws?
// A10. An exception escaping a std::thread calls std::terminate. Catch inside
//      the worker loop (Section 12) and decide whether to log, retry or stop.
//
// Q11. Bounded or unbounded queue?
// A11. Bounded gives backpressure and a memory ceiling; the producer blocks
//      (or you shed load with tryPush). Unbounded never blocks producers but
//      can exhaust memory when consumers lag.
//
// Q12. How do you avoid copying items through the queue?
// A12. Push by value and std::move into storage; pop moves out. This works for
//      move-only types such as unique_ptr. tryPush takes a reference and moves
//      only on success so a failed push keeps the item.
//
// Q13. Is `volatile bool done` a valid shutdown flag?
// A13. No. volatile gives no atomicity or ordering. Use std::atomic<bool>, or
//      a plain bool protected by the same mutex as the queue.
//
// Q14. How would you make this lock-free?
// A14. Only for one producer and one consumer: a ring with two atomics,
//      release/acquire on the indices, one slot wasted. Multi-producer
//      lock-free queues need ABA and reclamation strategies (Section 13).
//
// Q15. How do you test it?
// A15. A harness with exactly-once counters, per-producer order checks and
//      total counts at several capacities (Section 7), plus
//      -fsanitize=thread and -fsanitize=address,undefined runs.

// ============================================================
// SECTION 16 : FUNCTION REFERENCE TABLE
//              Function | Description | Return value | How to use
// ============================================================

// ---------------- BoundedQueue<T> (this file) ----------------

// FUNCTION      : push(item)
// DESCRIPTION   : Blocking insert. Sleeps while the queue is full
// RETURN VALUE  : bool — false if the queue was closed (item not stored)
// HOW TO USE    :
//     if (!q.push(std::move(job)))
//     {
//         /* queue closed: stop producing */
//     }

// FUNCTION      : pop()
// DESCRIPTION   : Blocking remove. Sleeps while the queue is empty
// RETURN VALUE  : optional<T> — nullopt only when closed AND drained
// HOW TO USE    :
//     while (auto job = q.pop())
//     {
//         process(*job);
//     }

// FUNCTION      : tryPush(item&)
// DESCRIPTION   : Non-blocking insert. Moves from `item` only on success
// RETURN VALUE  : bool — false if full or closed
// HOW TO USE    :
//     if (!q.tryPush(job))
//     {
//         /* shed load, `job` is still intact */
//     }

// FUNCTION      : popFor(out, timeout)
// DESCRIPTION   : Remove, blocking at most `timeout`
// RETURN VALUE  : PopStatus — Ok, Timeout or Closed
// HOW TO USE    :
//     int v;
//     if (q.popFor(v, 100ms) == PopStatus::Ok)
//     {
//         use(v);
//     }

// FUNCTION      : close()
// DESCRIPTION   : "No more input". Wakes every sleeper
// RETURN VALUE  : void
// HOW TO USE    :
//     q.close();     // after the last producer finished

// ---------------- std::condition_variable ----------------

// FUNCTION      : wait(lock, pred)
// DESCRIPTION   : while (!pred()) { atomically unlock, sleep, re-lock }
// RETURN VALUE  : void
// HOW TO USE    :
//     std::unique_lock<std::mutex> lock(m);
//     cv.wait(lock, [&]() { return ready; });

// FUNCTION      : wait_for(lock, duration, pred) / wait_until(lock, time, pred)
// DESCRIPTION   : Same, but gives up after a duration / at a time point
// RETURN VALUE  : bool — the predicate's value (false = timed out)
//                 (the overloads WITHOUT a predicate return std::cv_status)
// HOW TO USE    :
//     bool ok = cv.wait_for(lock, 100ms, [&]() { return ready; });

// FUNCTION      : notify_one() / notify_all()
// DESCRIPTION   : Wake one / every waiting thread (they re-check the predicate)
// RETURN VALUE  : void
// HOW TO USE    :
//     cv.notify_one();

// ---------------- std::counting_semaphore (C++20) ----------------

// FUNCTION      : acquire() / try_acquire() / try_acquire_for(duration)
// DESCRIPTION   : Decrement the counter; sleep / fail at once / sleep with a timeout
// RETURN VALUE  : void / bool / bool
// HOW TO USE    :
//     slots.acquire();

// FUNCTION      : release(n = 1)
// DESCRIPTION   : Increment the counter by n, waking sleepers. Any thread may call it
// RETURN VALUE  : void
// HOW TO USE    :
//     items.release();

// ---------------- std::jthread / std::stop_token (C++20) ----------------

// FUNCTION      : request_stop()
// DESCRIPTION   : Ask the thread to stop; wakes a condition_variable_any wait
// RETURN VALUE  : bool — true if this call made the request
// HOW TO USE    :
//     worker.request_stop();

// FUNCTION      : stop_requested()
// DESCRIPTION   : Has a stop been requested?
// RETURN VALUE  : bool
// HOW TO USE    :
//     while (!st.stop_requested())
//     {
//         work();
//     }

// ============================================================
// SECTION 17 : PRACTICE PROBLEMS
// (LC = LeetCode, GFG = GeeksforGeeks, CN = Coding Ninjas,
//  NKR = Naukri Coding 360. Titles outside LeetCode and the GFG article are
//  approximate: search the platform by topic.)
// ============================================================
//
//  Pattern 1 — implement a bounded blocking queue
//    LC   #1188 Design Bounded Blocking Queue (premium)   Medium
//    GFG  Producer Consumer Problem using Semaphores | Set 1
//    CN   producer-consumer / multithreading problems
//    NKR  concurrency practice set
//
//  Pattern 2 — thread ordering and hand-off
//    LC   #1114 Print in Order                            Easy
//    LC   #1115 Print FooBar Alternately                  Medium
//    LC   #1116 Print Zero Even Odd                       Medium
//    LC   #1195 FizzBuzz Multithreaded                    Medium
//
//  Pattern 3 — synchronization puzzles
//    LC   #1117 Building H2O                              Medium
//    LC   #1226 The Dining Philosophers                   Medium

// ============================================================
// SECTION 18 : QUICK-FIRE ORAL DRILL (2-minute recap)
// ============================================================
//
// rules          -> mutual exclusion, no overflow, no underflow, clean shutdown
// doorbell       -> a condition variable stores nothing; the predicate is the truth
// wait           -> atomically unlock + sleep; ALWAYS the predicate form
// two CVs        -> notFull (producers), notEmpty (consumers)
// notify         -> notify_one per item; notify_all on close
// shared state   -> modify only under the mutex
// pop            -> ONE atomic call that returns the item (no TOCTOU)
// shutdown       -> closed flag + notify_all; drain, then exit
// semaphores     -> two counters + mutex; shutdown by poison pills
// C++20          -> jthread + stop_token + condition_variable_any
// thread pool    -> the callers are producers, the workers are consumers
// SPSC           -> lock-free ring, exactly one producer and one consumer
// busy-wait      -> burned ~996 ms of CPU per second vs 0.0 ms for cv.wait
// exception      -> escaping a std::thread calls std::terminate; catch in the loop

// ============================================================
// SECTION 19 : FULL WORKING DEMO
// ============================================================

int main()
{
    cout << "===== Producer-Consumer — Complete Demo =====\n";

    banner("Section 3: busy-wait vs cv.wait (CPU burned)");
    section3_spin_vs_wait();

    banner("Section 4: lost wakeup vs predicate");
    section4_lost_wakeup();

    banner("Section 6: one producer, one consumer");
    section6_basic_demo();

    banner("Section 7: multi-producer / multi-consumer harness");
    section7_mpmc_harness();

    banner("Section 8: shutdown releases blocked threads");
    section8_shutdown();

    banner("Section 9: tryPush and popFor");
    section9_try_and_timed();

    banner("Section 10: semaphore queue with poison pills");
    section10_semaphore_demo();

    banner("Section 11: jthread + stop_token");
    section11_stop_token_demo();

    banner("Section 12: thread pool");
    section12_thread_pool();

    cout << "\n===== Done =====\n";
    return 0;
}

// ============================================================
// END OF NOTES
// Compile (one line):
//   g++ -std=c++20 -O0 -g -Wall -Wextra -Wshadow -Wsign-conversion -pthread -fsanitize=address,undefined Producer_Consumer_Explanation.cpp -o pc_demo
// Thread check (one line, cannot be combined with address):
//   g++ -std=c++20 -O1 -g -pthread -fsanitize=thread Producer_Consumer_Explanation.cpp -o pc_demo_tsan
// ============================================================

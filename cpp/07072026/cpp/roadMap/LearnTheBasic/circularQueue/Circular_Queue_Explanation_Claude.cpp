// ============================================================
//  FILE  : Circular_Queue_Explanation.cpp
//  TOPIC : Circular Queue — lecture L78 notes + 5-year-experience extensions
//  LEVEL : Concept -> senior interview depth (C++ Linux programmer)
//  STD   : C++17 or later
//  LECTURE : "L78. Circular Queue in Data Structure"
//            https://www.youtube.com/watch?v=4mKKolshFD0
//            Sections 1-7   follow the lecture
//            Sections 8-21  are extensions (bugs, variants, interview)
//  VERIFIED: every class in this file was compiled and run with
//            -fsanitize=address,undefined. The SPSC ring (Section 14)
//            was also run with -fsanitize=thread (no reports).
// ============================================================

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <vector>
using namespace std;

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

// ------------------------------------------------------------
// VIDEO MAP (approximate timestamps, from a transcript of the lecture)
//   ~0:50   circular queue idea: fixed size, wrap-around
//   ~2:25   implementing it with an array ("cut the circle open")
//   ~4:10   push / pop walkthrough with capacity 3
//   ~6:30   the wrap-around problem -> (r + 1) % cap
//   ~11:20  writing the class in C++
//   ~14:35  time complexity: everything is O(1)
//   ~15:00  demo run (prints 4 2 3, then 2 3 4)
//
// TRANSCRIPT NOTE: the initial value of rear is garbled in the
// transcript. -1 is the only value for which the push logic works.
// ------------------------------------------------------------

// ============================================================
// SECTION 1 : WHAT IS A CIRCULAR QUEUE ?
// ============================================================
//
//  A circular queue (also called a ring buffer) is a FIFO queue with a
//  FIXED capacity, stored in an array, where the index after the last
//  slot wraps around to slot 0.
//
//  Same interface as a normal queue:
//    push(x)   insert at the REAR
//    pop()     remove from the FRONT
//    front()   read the FRONT element
//    empty()   is it empty ?
//
//  What is different:
//    - the size is fixed (capacity)
//    - when the rear passes the last slot it wraps to the first slot
//
//  PICTURE — the circle "cut open" into an array:
//
//          +-----+-----+-----+
//    idx:  |  0  |  1  |  2  |        capacity = 3
//          +-----+-----+-----+
//
//    the index sequence is:  0 -> 1 -> 2 -> 0 -> 1 -> 2 -> 0 ...
//                            (after 2 comes 0 again)
//
//  The queue is tracked with two pointers:
//    f  index of the FRONT element (where pop / front work)
//    r  index of the REAR  element (where push works)

// ============================================================
// SECTION 2 : WHY NOT A PLAIN ARRAY QUEUE ?
// ============================================================
//
//  (Standard background, not from the lecture.)
//  A plain array queue only ever moves f and r FORWARD. Once r reaches
//  the last slot it reports "full" — even if pop() has freed many slots
//  at the front. Those freed slots can never be reused.
//  Wrapping r around to slot 0 is exactly what fixes this.

class NaiveArrayQueue
{
    vector<int> arr;
    int f;
    int r;

public:
    explicit NaiveArrayQueue(size_t n)
        : arr(n), f(0), r(-1)
    {
    }

    bool push(int x)
    {
        if (r + 1 == static_cast<int>(arr.size()))
        {
            return false;          // rear hit the end -> "full"
        }

        ++r;
        arr[static_cast<size_t>(r)] = x;
        return true;
    }

    bool pop()
    {
        if (f > r)
        {
            return false;          // empty
        }

        ++f;                       // slot freed, but it can NEVER be reused
        return true;
    }

    int size() const
    {
        return r - f + 1;
    }
};

void section2_naive_queue_problem()
{
    NaiveArrayQueue q(3);
    q.push(1);
    q.push(2);
    q.push(3);
    q.pop();
    q.pop();

    bool accepted = q.push(4);
    cout << "naive queue holds " << q.size() << " element(s) in 3 slots, "
         << "push(4) accepted? " << boolalpha << accepted << noboolalpha << "\n";
    // -> holds 1 element(s) in 3 slots, push(4) accepted? false
}

// ============================================================
// SECTION 3 : STATE VARIABLES AND INITIALIZATION
// ============================================================
//
//  VARIABLES (from the lecture)
//    arr       the fixed array of cap ints
//    cap       capacity — never changes
//    currSize  how many elements are stored right now
//    f         index of the FRONT element
//    r         index of the last pushed element (REAR)
//
//  INITIAL VALUES
//    currSize = 0
//    f = 0     so the first pop() / front() already points at a real slot
//    r = -1    so the first push's (r + 1) % cap lands on index 0
//
//  The lecture first sets BOTH f and r to -1, then changes f to 0 to
//  make pop() work. It also forgets to initialize currSize in the
//  constructor at first and adds it later (~15:25).
//
//  EQUIVALENT CONVENTION: r = cap - 1 instead of -1.
//    (cap - 1 + 1) % cap == 0 gives the same first slot, and r is never
//    negative, which is safer if anything reads r while the queue is empty.

// ============================================================
// SECTION 4 : THE WRAP-AROUND FORMULA
// ============================================================
//
//    next = (i + 1) % cap
//
//  The result is always in 0 .. cap-1.
//  Example, cap = 3:   (2 + 1) % 3 = 0     -> wraps to the start
//
//  STEPPING BACKWARD (needed for a deque, Section 11):
//    (i - 1) % cap is WRONG for i = 0 in C++.
//    The % operator takes the sign of the left operand (C++11 and later),
//    so (0 - 1) % 3 == -1, a negative index (undefined behavior).
//    Correct:  (i + cap - 1) % cap

void section4_wrap_formula()
{
    const int cap = 3;

    int r = -1;
    cout << "rear index after each push (cap=3): ";
    for (int i = 0; i < 7; ++i)
    {
        r = (r + 1) % cap;
        cout << r << " ";
    }
    cout << "\n";                  // 0 1 2 0 1 2 0

    int idx = 0;
    cout << "(0 - 1) % 3     = " << (idx - 1) % cap
         << "   <- negative, never use as an index\n";            // -1
    cout << "(0 + 3 - 1) % 3 = " << (idx + cap - 1) % cap
         << "   <- correct step backward\n";                       // 2
}

// ============================================================
// SECTION 5 : OPERATIONS AND COMPLEXITY
// ============================================================
//
//  push(x)
//    1. if currSize == cap      -> full, reject
//    2. r = (r + 1) % cap
//    3. arr[r] = x
//    4. currSize++
//
//  pop()
//    1. if currSize == 0        -> empty, reject
//    2. f = (f + 1) % cap
//    3. currSize--
//    (the old value is NOT erased; the slot is simply reusable)
//
//  front()   empty ? error : arr[f]
//  empty()   currSize == 0
//  full()    currSize == cap
//
//  COMPLEXITY: every operation O(1). Space O(cap).
//
//  Derived fact (used in Sections 9 and 10), when currSize >= 1:
//      r == (f + currSize - 1) % cap

// ============================================================
// SECTION 6 : THE LECTURE IMPLEMENTATION
// ============================================================
//
//  push / pop / front / empty / printArr are the lecture's functions.
//  The destructor, deleted copy operations, full(), tryFront(),
//  invariantHolds() and dump() are NOT in the lecture. They are added
//  here because the lecture version leaks memory (Section 8).

class CircularQueue
{
    int* arr;
    int  cap;
    int  currSize;
    int  f;          // index of the FRONT element
    int  r;          // index of the last pushed element (REAR)

public:
    explicit CircularQueue(int size)
    {
        cap = size;
        arr = new int[cap]();      // () value-initializes: slots start as 0
        currSize = 0;
        f = 0;
        r = -1;
    }

    // --- NOT in the lecture: without these the class leaks ---
    ~CircularQueue()
    {
        delete[] arr;
    }

    CircularQueue(const CircularQueue&) = delete;
    CircularQueue& operator=(const CircularQueue&) = delete;

    void push(int data)
    {
        if (currSize == cap)
        {
            cout << "CQ is full\n";
            return;
        }

        r = (r + 1) % cap;
        arr[r] = data;
        currSize++;
    }

    void pop()
    {
        if (empty())
        {
            cout << "CQ is empty\n";
            return;
        }

        f = (f + 1) % cap;
        currSize--;
    }

    int front()
    {
        if (empty())
        {
            cout << "CQ is empty\n";
            return -1;
        }

        return arr[f];
    }

    bool empty() const
    {
        return currSize == 0;
    }

    void printArr() const
    {
        for (int i = 0; i < cap; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    // --- extras (not in the lecture) ---
    bool full() const
    {
        return currSize == cap;
    }

    optional<int> tryFront() const
    {
        if (empty())
        {
            return nullopt;
        }

        return arr[f];
    }

    bool invariantHolds() const
    {
        return currSize == 0 || r == (f + currSize - 1) % cap;
    }

    void dump(const char* op) const
    {
        cout << left << setw(9) << op << right
             << " f=" << f << " r=" << setw(2) << r
             << " size=" << currSize << "  array: ";
        for (int i = 0; i < cap; i++)
        {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }
};

void section6_lecture_demo()
{
    CircularQueue cq(3);

    cq.push(1);
    cq.push(2);
    cq.push(3);
    cq.push(4);        // CQ is full

    cq.pop();          // removes 1, front moves to index 1
    cq.push(4);        // rear wraps around to index 0

    cq.printArr();     // physical array order -> 4 2 3

    while (!cq.empty())
    {
        cout << cq.front() << " ";
        cq.pop();
    }
    cout << endl;      // logical queue order  -> 2 3 4
}

// ============================================================
// SECTION 7 : DRY RUN (the lecture's demo, cap = 3)
// ============================================================
//
//  op        f   r   size   array [0] [1] [2]    note
//  start     0  -1    0     _  _  _
//  push(1)   0   0    1     1  _  _
//  push(2)   0   1    2     1  2  _
//  push(3)   0   2    3     1  2  3              now full
//  push(4)   0   2    3     1  2  3              rejected: "CQ is full"
//  pop()     1   2    2     1  2  3              1 is gone logically, still in memory
//  push(4)   1   0    3     4  2  3              r = (2+1) % 3 = 0  -> wrapped
//
//  Physical (array) order: 4 2 3
//  Logical (queue) order : start at f=1 and wrap -> 2 3 4
//
//  The program below prints the real trace ("0" = slot never written).

void section7_dry_run()
{
    CircularQueue cq(3);

    cq.dump("start");

    cq.push(1);
    cq.dump("push(1)");

    cq.push(2);
    cq.dump("push(2)");

    cq.push(3);
    cq.dump("push(3)");

    cq.push(4);        // prints "CQ is full"
    cq.dump("push(4)");

    cq.pop();
    cq.dump("pop()");

    cq.push(4);
    cq.dump("push(4)");
}

// ============================================================
// SECTION 8 : PROBLEMS IN THE LECTURE CODE (and the fixes)
// ============================================================
//
//  1. MEMORY LEAK. The lecture does `arr = new int[cap];` and never
//     calls delete[]. Built with -fsanitize=address, the lecture
//     version reports (abridged, real output):
//
//        LeakSanitizer: detected memory leaks
//        Direct leak of 12 byte(s) in 1 object(s) allocated from:
//            #0 operator new[](unsigned long)
//            #1 CircularQueue::CircularQueue(int)
//        SUMMARY: AddressSanitizer: 12 byte(s) leaked in 1 allocation(s).
//
//     Fix: add ~CircularQueue() { delete[] arr; } and delete the copy
//     operations (Rule of Three: a copy would double-free). Better: hold
//     a std::vector<int> so nothing needs manual cleanup (Rule of Zero) —
//     see MyCircularQueue in Section 10.
//
//  2. SENTINEL RETURN. front() returns -1 when empty. If -1 can be a real
//     element the caller cannot tell the two cases apart.
//     Fix: return optional<int> (tryFront below), or a bool + out parameter.
//
//  3. UNINITIALIZED SLOTS. `new int[cap]` leaves values undefined, so
//     printArr() would print garbage for slots never written.
//     Fix: `new int[cap]()` (value-initialize) or a vector.
//
//  4. NOT THREAD-SAFE. currSize is changed by BOTH push and pop, so a
//     producer and a consumer on different threads race on it.
//     Fix: Section 14 (separate head / tail atomics).

void section8_sentinel_demo()
{
    CircularQueue cq(2);

    cq.push(-1);                   // -1 is a perfectly valid element
    int a = cq.front();            // -1 : a real element
    cq.pop();
    int b = cq.front();            // prints "CQ is empty", returns -1 : sentinel
    cout << "front() on {-1} -> " << a << ",  front() on {} -> " << b
         << "   (same value, different meaning)\n";

    cq.push(-1);
    optional<int> x = cq.tryFront();
    cq.pop();
    optional<int> y = cq.tryFront();
    cout << "tryFront() on {-1} -> " << (x ? to_string(*x) : string("none"))
         << ",  tryFront() on {} -> " << (y ? to_string(*y) : string("none")) << "\n";
}

// ============================================================
// SECTION 9 : FULL vs EMPTY
// ============================================================
//
//  With only f and r (no counter) the state "(r + 1) % cap == f" means
//  BOTH empty and full. Ways to tell them apart:
//
//   A. Keep a counter (currSize) — the lecture's approach.
//        All cap slots are usable.
//   B. Waste one slot (below). With head = next slot to READ and
//        tail = next slot to WRITE:
//          empty  <=>  head == tail
//          full   <=>  (tail + 1) % cap == head
//          usable capacity = cap - 1
//        Used when two threads own one index each (Section 14).
//   C. Keep a boolean "full" flag.
//   D. Free-running counters with a power-of-two capacity (Section 12):
//        size = tail - head, full <=> size == cap, no slot wasted.
//
//  Also: because r == (f + currSize - 1) % cap, you can drop r and
//  compute it. MyCircularQueue (Section 10) does exactly that.

class WasteSlotQueue
{
    vector<int> buf;
    size_t head;     // next slot to READ
    size_t tail;     // next slot to WRITE

public:
    explicit WasteSlotQueue(size_t n)
        : buf(n), head(0), tail(0)
    {
    }

    bool empty() const
    {
        return head == tail;
    }

    bool full() const
    {
        return (tail + 1) % buf.size() == head;
    }

    size_t usableCapacity() const
    {
        return buf.size() - 1;
    }

    bool push(int v)
    {
        if (full())
        {
            return false;
        }

        buf[tail] = v;
        tail = (tail + 1) % buf.size();
        return true;
    }

    bool pop(int& out)
    {
        if (empty())
        {
            return false;
        }

        out = buf[head];
        head = (head + 1) % buf.size();
        return true;
    }
};

void section9_full_vs_empty()
{
    WasteSlotQueue w(4);
    int pushed = 0;
    while (w.push(pushed + 1))
    {
        ++pushed;
    }
    cout << "waste-slot queue : 4 slots, accepted " << pushed
         << " pushes (usable capacity = " << w.usableCapacity() << ")\n";

    CircularQueue cq(4);
    int accepted = 0;
    for (int i = 1; i <= 4; ++i)
    {
        if (!cq.full())
        {
            cq.push(i);
            ++accepted;
        }
    }
    cout << "counter-based    : 4 slots, accepted " << accepted << " pushes\n";

    cq.pop();
    cq.push(5);
    cout << "r == (f + size - 1) % cap holds after wrap-around? "
         << boolalpha << cq.invariantHolds() << noboolalpha << "\n";
}

// ============================================================
// SECTION 10 : LEETCODE #622 — DESIGN CIRCULAR QUEUE
// ============================================================
//
//  Same idea as the lecture, written for interviews:
//    - std::vector owns the memory (no leak, Rule of Zero)
//    - head + count instead of f / r / currSize: the rear slot is derived
//    - size_t internally, so -Wsign-conversion stays quiet
//  Requires k >= 1 (LeetCode guarantees it); % 0 would crash.

class MyCircularQueue
{
    vector<int> buf;
    size_t cap;
    size_t head;     // index of the front element
    size_t count;    // number of stored elements

public:
    explicit MyCircularQueue(int k)
        : buf(static_cast<size_t>(k)), cap(static_cast<size_t>(k)), head(0), count(0)
    {
    }

    bool enQueue(int value)
    {
        if (isFull())
        {
            return false;
        }

        buf[(head + count) % cap] = value;     // rear slot is derived
        ++count;
        return true;
    }

    bool deQueue()
    {
        if (isEmpty())
        {
            return false;
        }

        head = (head + 1) % cap;
        --count;
        return true;
    }

    int Front() const
    {
        if (isEmpty())
        {
            return -1;
        }

        return buf[head];
    }

    int Rear() const
    {
        if (isEmpty())
        {
            return -1;
        }

        return buf[(head + count - 1) % cap];
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    bool isFull() const
    {
        return count == cap;
    }
};

void section10_lc622_sample()
{
    MyCircularQueue q(3);
    cout << boolalpha;
    cout << q.enQueue(1) << " "      // true
         << q.enQueue(2) << " "      // true
         << q.enQueue(3) << " "      // true
         << q.enQueue(4) << " "      // false (full)
         << q.Rear()     << " "      // 3
         << q.isFull()   << " "      // true
         << q.deQueue()  << " "      // true
         << q.enQueue(4) << " "      // true (wraps around)
         << q.Rear()     << "\n";    // 4
    cout << noboolalpha;
}

// ============================================================
// SECTION 11 : LEETCODE #641 — DESIGN CIRCULAR DEQUE
// ============================================================
//
//  A deque needs wrap-around in BOTH directions.
//    insertFront : step head BACKWARD  -> head = (head + cap - 1) % cap
//    deleteLast  : only count shrinks
//  The "+ cap" is what keeps the backward step from going negative
//  (Section 4).

class MyCircularDeque
{
    vector<int> buf;
    size_t cap;
    size_t head;     // index of the front element
    size_t count;

public:
    explicit MyCircularDeque(int k)
        : buf(static_cast<size_t>(k)), cap(static_cast<size_t>(k)), head(0), count(0)
    {
    }

    bool insertFront(int value)
    {
        if (isFull())
        {
            return false;
        }

        head = (head + cap - 1) % cap;
        buf[head] = value;
        ++count;
        return true;
    }

    bool insertLast(int value)
    {
        if (isFull())
        {
            return false;
        }

        buf[(head + count) % cap] = value;
        ++count;
        return true;
    }

    bool deleteFront()
    {
        if (isEmpty())
        {
            return false;
        }

        head = (head + 1) % cap;
        --count;
        return true;
    }

    bool deleteLast()
    {
        if (isEmpty())
        {
            return false;
        }

        --count;
        return true;
    }

    int getFront() const
    {
        if (isEmpty())
        {
            return -1;
        }

        return buf[head];
    }

    int getRear() const
    {
        if (isEmpty())
        {
            return -1;
        }

        return buf[(head + count - 1) % cap];
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    bool isFull() const
    {
        return count == cap;
    }
};

void section11_lc641_sample()
{
    MyCircularDeque d(3);
    cout << boolalpha;
    cout << d.insertLast(1)  << " "      // true
         << d.insertLast(2)  << " "      // true
         << d.insertFront(3) << " "      // true
         << d.insertFront(4) << " "      // false (full)
         << d.getRear()      << " "      // 2
         << d.isFull()       << " "      // true
         << d.deleteLast()   << " "      // true
         << d.insertFront(4) << " "      // true
         << d.getFront()     << "\n";    // 4
    cout << noboolalpha;
}

// ============================================================
// SECTION 12 : POWER-OF-TWO CAPACITY — MASK AND FREE-RUNNING COUNTERS
// ============================================================
//
//  If cap is a power of two:
//      i % cap  ==  i & (cap - 1)          (a mask instead of a division)
//
//  Free-running counters (the idea behind Linux's kfifo):
//    - head and tail are unsigned and only ever INCREMENT
//    - the slot is  counter & mask
//    - size = tail - head
//    - full <=> size == cap  (no slot wasted, no separate counter)
//
//  Why it still works when the counters overflow:
//    - unsigned overflow is DEFINED (wraps modulo 2^32); signed is UB
//    - tail - head is also correct across the wrap (modular arithmetic)
//    - slot = counter & mask stays consistent across the wrap only
//      because 2^32 is a multiple of cap — i.e. cap is a power of two.

class Pow2Ring
{
    vector<int> buf;
    uint32_t mask;
    uint32_t head;     // free-running read counter
    uint32_t tail;     // free-running write counter

public:
    explicit Pow2Ring(uint32_t n, uint32_t startCounter = 0)
        : buf(n), mask(n - 1), head(startCounter), tail(startCounter)
    {
        check(n != 0 && (n & (n - 1)) == 0, "capacity must be a power of two");
    }

    uint32_t size() const
    {
        return tail - head;        // correct even after the counters wrapped
    }

    bool full() const
    {
        return size() == mask + 1;
    }

    bool push(int v)
    {
        if (full())
        {
            return false;
        }

        buf[tail & mask] = v;
        ++tail;                    // never reduced modulo cap
        return true;
    }

    bool pop(int& out)
    {
        if (size() == 0)
        {
            return false;
        }

        out = buf[head & mask];
        ++head;
        return true;
    }

    uint32_t headCounter() const
    {
        return head;
    }

    uint32_t tailCounter() const
    {
        return tail;
    }
};

void section12_pow2_ring()
{
    for (unsigned i = 0; i < 100000; ++i)
    {
        check((i + 1) % 8 == ((i + 1) & 7), "mask equals modulo for cap 8");
    }
    cout << "(i + 1) % 8 == (i + 1) & 7 for 100000 values\n";

    // start the counters just below the 32-bit limit so they wrap while in use
    Pow2Ring ring(4, 0xFFFFFFFEu);
    for (int i = 1; i <= 4; ++i)
    {
        ring.push(i);
    }
    cout << "after 4 pushes: head=0x" << hex << ring.headCounter()
         << " tail=0x" << ring.tailCounter() << dec
         << " size=" << ring.size() << "   (tail wrapped past 0)\n";

    cout << "popped in order:";
    int v = 0;
    while (ring.pop(v))
    {
        cout << " " << v;
    }
    cout << "\n";
}

// ============================================================
// SECTION 13 : GROWING A CIRCULAR QUEUE
// ============================================================
//
//  When the queue is full and must grow, a raw copy of the array is
//  WRONG: the elements are in physical order, but the queue order starts
//  at head and may wrap. You must copy in LOGICAL order and reset head
//  to 0. Doubling gives amortized O(1) push, like vector.

class GrowableCircularQueue
{
    vector<int> buf;
    size_t head;
    size_t count;

    void grow()
    {
        vector<int> bigger(buf.size() * 2);
        for (size_t i = 0; i < count; ++i)
        {
            bigger[i] = buf[(head + i) % buf.size()];     // LOGICAL order
        }
        buf = move(bigger);
        head = 0;                                          // re-linearized
    }

public:
    GrowableCircularQueue()
        : buf(2), head(0), count(0)
    {
    }

    void push(int v)
    {
        if (count == buf.size())
        {
            grow();
        }

        buf[(head + count) % buf.size()] = v;
        ++count;
    }

    bool pop()
    {
        if (count == 0)
        {
            return false;
        }

        head = (head + 1) % buf.size();
        --count;
        return true;
    }

    int front() const
    {
        return buf[head];
    }

    size_t size() const
    {
        return count;
    }

    size_t capacity() const
    {
        return buf.size();
    }

    void dump(const char* tag) const
    {
        cout << left << setw(22) << tag << right << " cap=" << buf.size()
             << " head=" << head << "  physical:";
        for (int x : buf)
        {
            cout << " " << x;
        }
        cout << "   logical:";
        for (size_t i = 0; i < count; ++i)
        {
            cout << " " << buf[(head + i) % buf.size()];
        }
        cout << "\n";
    }
};

void section13_growing()
{
    // state before growing: cap 2, head 1, count 2, buf = [3, 2]  (logical: 2 3)
    vector<int> oldBuf = {3, 2};
    size_t oldHead = 1;
    size_t oldCount = 2;

    vector<int> wrong(4);
    copy(oldBuf.begin(), oldBuf.end(), wrong.begin());    // WRONG: raw copy, head kept
    cout << "WRONG grow (raw copy)  logical order:";
    for (size_t i = 0; i < oldCount; ++i)
    {
        cout << " " << wrong[(oldHead + i) % wrong.size()];
    }
    cout << "   <- expected 2 3\n";

    GrowableCircularQueue q;
    q.push(1);
    q.push(2);
    q.pop();
    q.push(3);
    q.dump("before grow");
    q.push(4);                                             // triggers grow()
    q.dump("after grow (correct)");
}

// ============================================================
// SECTION 14 : THREAD SAFETY — SPSC RING (preview of Phase 8)
// ============================================================
//
//  Single Producer, Single Consumer. No locks.
//    - head is written ONLY by the consumer, tail ONLY by the producer
//    - there is no shared counter, so one slot is wasted to tell full
//      from empty (Section 9, way B)
//    - producer: relaxed-load own tail, ACQUIRE-load head (to see freed
//      slots), write the element, RELEASE-store tail (publishes it)
//    - consumer is the mirror image
//    - head and tail sit on separate cache lines (alignas(64)) to avoid
//      false sharing
//  Valid ONLY for exactly one producer thread and one consumer thread.

class SpscRing
{
    vector<int> buf;
    const size_t cap;
    alignas(64) atomic<size_t> head;     // next slot to READ  (consumer writes)
    alignas(64) atomic<size_t> tail;     // next slot to WRITE (producer writes)

public:
    explicit SpscRing(size_t n)
        : buf(n), cap(n), head(0), tail(0)
    {
    }

    bool push(int v)                      // producer thread only
    {
        size_t t = tail.load(memory_order_relaxed);
        size_t next = (t + 1) % cap;

        if (next == head.load(memory_order_acquire))
        {
            return false;                 // full
        }

        buf[t] = v;
        tail.store(next, memory_order_release);
        return true;
    }

    bool pop(int& out)                    // consumer thread only
    {
        size_t h = head.load(memory_order_relaxed);

        if (h == tail.load(memory_order_acquire))
        {
            return false;                 // empty
        }

        out = buf[h];
        head.store((h + 1) % cap, memory_order_release);
        return true;
    }
};

void section14_spsc()
{
    constexpr int N = 200000;
    SpscRing ring(64);

    thread producer([&ring]()
    {
        for (int i = 0; i < N; ++i)
        {
            while (!ring.push(i))
            {
                this_thread::yield();
            }
        }
    });

    long long sum = 0;
    int expected = 0;
    bool inOrder = true;
    int received = 0;

    while (received < N)
    {
        int v = 0;
        if (ring.pop(v))
        {
            if (v != expected)
            {
                inOrder = false;
            }
            ++expected;
            sum += v;
            ++received;
        }
        else
        {
            this_thread::yield();
        }
    }

    producer.join();

    long long expectedSum = static_cast<long long>(N) * (N - 1) / 2;
    cout << "transferred " << received << " ints through a 64-slot ring: "
         << "in order = " << boolalpha << inOrder
         << ", sum correct = " << (sum == expectedSum) << noboolalpha << "\n";
    check(inOrder && sum == expectedSum, "SPSC transfer must be ordered and complete");
}

// ============================================================
// SECTION 15 : ELEMENT LIFETIME FOR NON-TRIVIAL T
// ============================================================
//
//  pop() only moves the front index. The popped object STAYS CONSTRUCTED
//  inside its slot until something overwrites it. For int that is
//  harmless; for shared_ptr, a big string, a file handle, a lock, it
//  silently extends the lifetime of the resource.
//  Fix: reset the slot on pop (buf[head] = T{}).
//  The template flag below shows both behaviors side by side.

template <typename T, bool ClearOnPop = true>
class CircularQueueT
{
    vector<T> buf;
    size_t head;
    size_t count;

public:
    explicit CircularQueueT(size_t n)
        : buf(n), head(0), count(0)
    {
    }

    bool push(T value)
    {
        if (count == buf.size())
        {
            return false;
        }

        buf[(head + count) % buf.size()] = move(value);
        ++count;
        return true;
    }

    bool pop()
    {
        if (count == 0)
        {
            return false;
        }

        if constexpr (ClearOnPop)
        {
            buf[head] = T{};              // release whatever the slot owned
        }

        head = (head + 1) % buf.size();
        --count;
        return true;
    }

    const T& front() const
    {
        return buf[head];                 // caller must check non-empty first
    }

    bool empty() const
    {
        return count == 0;
    }
};

void section15_element_lifetime()
{
    auto spKeep  = make_shared<int>(1);
    auto spClear = make_shared<int>(2);

    CircularQueueT<shared_ptr<int>, false> keepQ(2);
    keepQ.push(spKeep);
    keepQ.pop();

    CircularQueueT<shared_ptr<int>, true> clearQ(2);
    clearQ.push(spClear);
    clearQ.pop();

    cout << "ClearOnPop=false : use_count after pop = " << spKeep.use_count()
         << "   (stale copy still alive in the slot)\n";
    cout << "ClearOnPop=true  : use_count after pop = " << spClear.use_count()
         << "   (slot released)\n";
}

// ============================================================
// SECTION 16 : COMMON MISTAKES
// ============================================================
//
//  1. Forgetting the wrap:       r = r + 1            instead of (r + 1) % cap
//  2. Wrong initial rear:        r = 0 (first push then skips slot 0)
//                                use r = -1 or r = cap - 1
//  3. Updating r but not currSize (or the reverse): full/empty then lie
//  4. Checking "full" as f == r + 1 without a counter: ambiguous with empty
//  5. Stepping backward with (i - 1) % cap: negative in C++ (Section 4)
//  6. new[] without delete[]: the lecture code leaks (Section 8)
//  7. Returning -1 as "empty": ambiguous when -1 is a valid value
//  8. Copying a raw array on resize instead of re-linearizing (Section 13)
//  9. Sharing a counter between producer and consumer threads (Section 14)
// 10. int indices into a vector: -Wsign-conversion; use size_t internally

// ============================================================
// SECTION 17 : 5-YEAR EXPERIENCE INTERVIEW QUESTION BANK
// ============================================================
//
//  Q1. Why use a circular queue instead of a plain array queue?
//  A1. In a plain array queue the freed front slots are never reused, so it
//      reports "full" while slots are free (Section 2). Wrapping the rear
//      reuses them. Memory stays fixed and every operation is O(1).
//
//  Q2. How do you tell full from empty?
//  A2. Counter; waste one slot; boolean full flag; or free-running unsigned
//      counters with a power-of-two capacity (Section 9).
//
//  Q3. Why r = -1 (or cap - 1) and f = 0?
//  A3. So the first push computes (r + 1) % cap == 0, and the first
//      pop / front already points at slot 0. With r = 0 the first push would
//      skip slot 0 unless you write before incrementing (a different convention).
//
//  Q4. How can you get the rear index without storing it?
//  A4. r == (f + currSize - 1) % cap whenever currSize >= 1 (Section 10).
//
//  Q5. Why does stepping backward with (i - 1) % cap break?
//  A5. In C++ the sign of % follows the left operand (C++11 and later), so
//      (0 - 1) % 3 == -1. Use (i + cap - 1) % cap (Sections 4 and 11).
//
//  Q6. Why must free-running counters be unsigned, and why a power-of-two
//      capacity?
//  A6. Unsigned overflow is defined (modulo 2^32); signed overflow is UB.
//      tail - head stays correct across the wrap. counter & mask only keeps
//      matching counter % cap across the wrap when 2^32 is a multiple of cap,
//      i.e. cap is a power of two (Section 12).
//
//  Q7. How do you make it thread-safe for ONE producer and ONE consumer?
//  A7. Two atomics, each written by exactly one thread. Release-store the
//      index after writing/reading the slot, acquire-load the other thread's
//      index before using it. Waste one slot (no shared counter). Put head
//      and tail on separate cache lines. Not valid for multiple producers or
//      consumers (Section 14).
//
//  Q8. How do you grow a circular queue?
//  A8. Allocate double the capacity, copy elements in LOGICAL order starting
//      at head, set head = 0. A raw copy keeps the wrong order (Section 13).
//
//  Q9. Does the standard library have a ring buffer?
//  A9. No container in C++17/20/23. Use boost::circular_buffer or write your
//      own. std::queue<T, std::deque<T>> is the dynamic (unbounded) option.
//
// Q10. What happens to a popped element of non-trivial type?
// A10. It stays constructed in its slot until overwritten, silently extending
//      the resource's lifetime. Reset the slot on pop (Section 15).
//
// Q11. Linked-list vs array implementation?
// A11. Both O(1). The linked list can grow without bound but allocates per
//      node (malloc cost, poor cache locality). The array version uses one
//      contiguous allocation with a fixed capacity.
//
// Q12. Complexity?
// A12. push, pop, front, empty, full: O(1). Space O(cap). The growable
//      version has amortized O(1) push.

// ============================================================
// SECTION 18 : FUNCTION REFERENCE TABLE
//              Function | Description | Return value | How to use
// ============================================================

// ---------------- LECTURE API (class CircularQueue) ----------------

// FUNCTION      : push(data)
// DESCRIPTION   : Insert at the rear. Rejects (prints "CQ is full") when full
// RETURN VALUE  : void
// HOW TO USE    :
//     cq.push(10);

// FUNCTION      : pop()
// DESCRIPTION   : Remove the front element by moving f forward with wrap-around.
//                 Prints "CQ is empty" and does nothing when empty
// RETURN VALUE  : void
// HOW TO USE    :
//     cq.pop();

// FUNCTION      : front()
// DESCRIPTION   : Read the front element without removing it
// RETURN VALUE  : int — the element, or -1 (after printing) when empty
// HOW TO USE    :
//     int x = cq.front();

// FUNCTION      : empty() / full()
// DESCRIPTION   : currSize == 0  /  currSize == cap
// RETURN VALUE  : bool
// HOW TO USE    :
//     while (!cq.empty())
//     {
//         cq.pop();
//     }

// FUNCTION      : tryFront()                         [extra]
// DESCRIPTION   : Front element without the -1 sentinel ambiguity
// RETURN VALUE  : optional<int> — nullopt when empty
// HOW TO USE    :
//     if (auto x = cq.tryFront())
//     {
//         cout << *x;
//     }

// ---------------- LEETCODE #622 API (class MyCircularQueue) ----------------

// FUNCTION      : enQueue(value) / deQueue()
// DESCRIPTION   : Insert at the rear / remove from the front
// RETURN VALUE  : bool — false when full / empty
// HOW TO USE    :
//     MyCircularQueue q(3);
//     q.enQueue(1);
//     q.deQueue();

// FUNCTION      : Front() / Rear()
// DESCRIPTION   : Read the front / rear element
// RETURN VALUE  : int — the element, or -1 when empty
// HOW TO USE    :
//     int last = q.Rear();

// FUNCTION      : isEmpty() / isFull()
// DESCRIPTION   : count == 0  /  count == cap
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (q.isFull())
//     {
//         /* reject */
//     }

// ---------------- LEETCODE #641 ADDITIONS (class MyCircularDeque) ----------------

// FUNCTION      : insertFront(value) / insertLast(value)
// DESCRIPTION   : Insert at the front (head steps BACKWARD with wrap) / at the rear
// RETURN VALUE  : bool — false when full
// HOW TO USE    :
//     d.insertFront(3);

// FUNCTION      : deleteFront() / deleteLast()
// DESCRIPTION   : Remove from the front (head forward) / from the rear (count--)
// RETURN VALUE  : bool — false when empty
// HOW TO USE    :
//     d.deleteLast();

// FUNCTION      : getFront() / getRear()
// DESCRIPTION   : Read the front / rear element
// RETURN VALUE  : int — the element, or -1 when empty
// HOW TO USE    :
//     int f = d.getFront();

// ============================================================
// SECTION 19 : PRACTICE PROBLEMS
// (LC = LeetCode, GFG = GeeksforGeeks, STV = Striver sheet,
//  CN = Coding Ninjas, NKR = Naukri Coding 360.
//  Titles outside LeetCode and the two GFG articles are approximate:
//  search the platform by topic.)
// ============================================================
//
//  Pattern 1 — implement a circular queue
//    LC   #622 Design Circular Queue                      Medium
//    GFG  Circular Queue | Set 1 (Introduction and Array Implementation)
//    GFG  Circular Queue | Set 2 (Circular Linked List Implementation)
//    STV  Stack and Queue -> implement queue using array   Easy
//    CN   queue / circular queue implementation problems    Easy
//    NKR  queue implementation practice set                 Easy
//
//  Pattern 2 — circular deque and variants
//    LC   #641 Design Circular Deque                       Medium
//    LC   #1670 Design Front Middle Back Queue              Medium
//
//  Pattern 3 — a circular buffer as a sliding window
//    LC   #933 Number of Recent Calls                       Easy
//    LC   #346 Moving Average from Data Stream (premium)    Easy
//    LC   #362 Design Hit Counter (premium)                 Medium
//
//  Pattern 4 — queue contrast problems
//    LC   #225 Implement Stack using Queues                 Easy
//    LC   #232 Implement Queue using Stacks                 Easy
//    LC   #239 Sliding Window Maximum (deque)               Hard

// ============================================================
// SECTION 20 : QUICK-FIRE ORAL DRILL (2-minute recap)
// ============================================================
//
// next index          -> (i + 1) % cap
// init                -> f = 0, r = -1 (or cap - 1), currSize = 0
// push                -> reject if full; r = (r+1) % cap; arr[r] = x; size++
// pop                 -> reject if empty; f = (f+1) % cap; size--  (value not erased)
// full / empty        -> currSize == cap / currSize == 0
// physical != logical -> array shows 4 2 3, queue order is 2 3 4
// rear is derivable   -> r == (f + size - 1) % cap
// step backward       -> (i + cap - 1) % cap, never (i - 1) % cap
// resize              -> copy in LOGICAL order, reset head to 0
// pow-2 capacity      -> % cap becomes & (cap - 1); free-running unsigned counters
// thread-safe SPSC    -> head/tail atomics, release/acquire, one slot wasted
// non-trivial T       -> reset the slot on pop or the resource lives on
// lecture code        -> leaks (no delete[]); use vector or add a destructor
// complexity          -> every operation O(1), space O(cap)

// ============================================================
// SECTION 21 : FUZZ TESTS AND FULL WORKING DEMO
// ============================================================
//
//  Each implementation is compared with std::queue / std::deque over
//  thousands of random operations.

static void fuzzLecture()
{
    for (int trial = 0; trial < 200; ++trial)
    {
        int k = 1 + rand() % 8;
        CircularQueue cq(k);
        queue<int> ref;

        for (int step = 0; step < 2000; ++step)
        {
            if (rand() % 2 == 0)
            {
                bool canPush = static_cast<int>(ref.size()) < k;
                check(cq.full() == !canPush, "lecture: full() matches reference");
                if (canPush)
                {
                    int v = rand() % 1000;
                    cq.push(v);
                    ref.push(v);
                }
            }
            else
            {
                check(cq.empty() == ref.empty(), "lecture: empty() matches reference");
                if (!ref.empty())
                {
                    check(cq.front() == ref.front(), "lecture: front() matches reference");
                    cq.pop();
                    ref.pop();
                }
            }
            check(cq.invariantHolds(), "lecture: r == (f + size - 1) % cap");
        }
    }
}

static void fuzzLC622()
{
    for (int trial = 0; trial < 200; ++trial)
    {
        int k = 1 + rand() % 8;
        MyCircularQueue mine(k);
        queue<int> ref;

        for (int step = 0; step < 2000; ++step)
        {
            if (rand() % 2 == 0)
            {
                int v = rand() % 1000;
                bool okRef = static_cast<int>(ref.size()) < k;
                if (okRef)
                {
                    ref.push(v);
                }
                check(mine.enQueue(v) == okRef, "622: enQueue result");
            }
            else
            {
                bool okRef = !ref.empty();
                if (okRef)
                {
                    ref.pop();
                }
                check(mine.deQueue() == okRef, "622: deQueue result");
            }

            check(mine.isEmpty() == ref.empty(), "622: isEmpty");
            check(mine.Front() == (ref.empty() ? -1 : ref.front()), "622: Front");
            check(mine.Rear() == (ref.empty() ? -1 : ref.back()), "622: Rear");
        }
    }
}

static void fuzzWasteSlot()
{
    for (int trial = 0; trial < 200; ++trial)
    {
        int k = 2 + rand() % 8;                 // array slots
        WasteSlotQueue mine(static_cast<size_t>(k));
        queue<int> ref;
        size_t usable = static_cast<size_t>(k - 1);

        for (int step = 0; step < 2000; ++step)
        {
            if (rand() % 2 == 0)
            {
                int v = rand() % 1000;
                bool okRef = ref.size() < usable;
                if (okRef)
                {
                    ref.push(v);
                }
                check(mine.push(v) == okRef, "waste-slot: push result");
            }
            else
            {
                int out = 0;
                bool okRef = !ref.empty();
                int expected = okRef ? ref.front() : 0;
                if (okRef)
                {
                    ref.pop();
                }
                check(mine.pop(out) == okRef, "waste-slot: pop result");
                if (okRef)
                {
                    check(out == expected, "waste-slot: popped value");
                }
            }
            check(mine.empty() == ref.empty(), "waste-slot: empty");
        }
    }
}

static void fuzzDeque()
{
    for (int trial = 0; trial < 200; ++trial)
    {
        int k = 1 + rand() % 8;
        MyCircularDeque mine(k);
        deque<int> ref;

        for (int step = 0; step < 2000; ++step)
        {
            int op = rand() % 4;
            int v = rand() % 1000;
            bool canInsert = static_cast<int>(ref.size()) < k;
            bool canDelete = !ref.empty();

            if (op == 0)
            {
                if (canInsert)
                {
                    ref.push_front(v);
                }
                check(mine.insertFront(v) == canInsert, "641: insertFront result");
            }
            else if (op == 1)
            {
                if (canInsert)
                {
                    ref.push_back(v);
                }
                check(mine.insertLast(v) == canInsert, "641: insertLast result");
            }
            else if (op == 2)
            {
                if (canDelete)
                {
                    ref.pop_front();
                }
                check(mine.deleteFront() == canDelete, "641: deleteFront result");
            }
            else
            {
                if (canDelete)
                {
                    ref.pop_back();
                }
                check(mine.deleteLast() == canDelete, "641: deleteLast result");
            }

            check(mine.isEmpty() == ref.empty(), "641: isEmpty");
            check(mine.getFront() == (ref.empty() ? -1 : ref.front()), "641: getFront");
            check(mine.getRear() == (ref.empty() ? -1 : ref.back()), "641: getRear");
        }
    }
}

static void fuzzGrowable()
{
    GrowableCircularQueue mine;
    queue<int> ref;

    for (int step = 0; step < 50000; ++step)
    {
        if (rand() % 10 < 6)
        {
            int v = rand() % 1000;
            mine.push(v);
            ref.push(v);
        }
        else
        {
            bool okRef = !ref.empty();
            if (okRef)
            {
                ref.pop();
            }
            check(mine.pop() == okRef, "growable: pop result");
        }

        check(mine.size() == ref.size(), "growable: size");
        if (!ref.empty())
        {
            check(mine.front() == ref.front(), "growable: front");
        }
    }
}

static void fuzzPow2()
{
    for (int trial = 0; trial < 200; ++trial)
    {
        uint32_t caps[4] = {1, 2, 4, 8};
        uint32_t k = caps[rand() % 4];
        uint32_t start = 0xFFFFFFF0u + static_cast<uint32_t>(rand() % 16);
        Pow2Ring mine(k, start);
        queue<int> ref;

        for (int step = 0; step < 2000; ++step)
        {
            if (rand() % 2 == 0)
            {
                int v = rand() % 1000;
                bool okRef = ref.size() < k;
                if (okRef)
                {
                    ref.push(v);
                }
                check(mine.push(v) == okRef, "pow2: push result");
            }
            else
            {
                int out = 0;
                bool okRef = !ref.empty();
                int expected = okRef ? ref.front() : 0;
                if (okRef)
                {
                    ref.pop();
                }
                check(mine.pop(out) == okRef, "pow2: pop result");
                if (okRef)
                {
                    check(out == expected, "pow2: popped value");
                }
            }
            check(mine.size() == ref.size(), "pow2: size");
        }
    }
}

static void section21_fuzz()
{
    srand(2026);
    fuzzLecture();
    fuzzLC622();
    fuzzWasteSlot();
    fuzzDeque();
    fuzzGrowable();
    fuzzPow2();
    cout << "all fuzz tests passed (lecture class, #622, waste-slot, #641, growable, pow2)\n";
}

int main()
{
    cout << "===== Circular Queue — Complete Demo =====\n";

    banner("Section 2: why not a plain array queue");
    section2_naive_queue_problem();

    banner("Section 4: wrap-around formula");
    section4_wrap_formula();

    banner("Section 6: lecture demo (expect: CQ is full / 4 2 3 / 2 3 4)");
    section6_lecture_demo();

    banner("Section 7: dry run trace");
    section7_dry_run();

    banner("Section 8: -1 sentinel vs optional");
    section8_sentinel_demo();

    banner("Section 9: full vs empty");
    section9_full_vs_empty();

    banner("Section 10: LeetCode #622 sample (expect: true true true false 3 true true true 4)");
    section10_lc622_sample();

    banner("Section 11: LeetCode #641 sample (expect: true true true false 2 true true true 4)");
    section11_lc641_sample();

    banner("Section 12: power-of-two ring, counters wrapping");
    section12_pow2_ring();

    banner("Section 13: growing a circular queue");
    section13_growing();

    banner("Section 14: SPSC ring across two threads");
    section14_spsc();

    banner("Section 15: element lifetime");
    section15_element_lifetime();

    banner("Section 21: fuzz tests");
    section21_fuzz();

    cout << "\n===== Done =====\n";
    return 0;
}

// ============================================================
// END OF NOTES
// Compile (one line):
//   g++ -std=c++20 -O0 -g -Wall -Wextra -Wshadow -Wsign-conversion -pthread -fsanitize=address,undefined Circular_Queue_Explanation.cpp -o cq_demo
// Thread check (one line, cannot be combined with address):
//   g++ -std=c++20 -O1 -g -pthread -fsanitize=thread Circular_Queue_Explanation.cpp -o cq_demo_tsan
// ============================================================

/*

i am 4.5 year  c++ programmer on linux os , give  interview question on stl std::vector  in details

Architected comprehensive vector interview guide with 28 questions.
std::vector — Complete Interview Question Bank

Organized by category, senior-level depth, GCC libstdc++ specifics called out where relevant (since you're on Linux).

Category 1: Internal Implementation & Memory Layout
Q1. What is std::vector internally? Describe its exact memory layout.

Answer: A std::vector<T> object itself is a small, fixed-size struct — typically 24 bytes on 64-bit Linux (GCC libstdc++) — holding three pointers (or two pointers + a size, depending on implementation):

cpp
template <typename T>
class vector
{
    T* _M_start;            // pointer to first element
    T* _M_finish;           // pointer to one-past-last CONSTRUCTED element
    T* _M_end_of_storage;   // pointer to one-past-last ALLOCATED byte
};
size() = _M_finish - _M_start
capacity() = _M_end_of_storage - _M_start

The actual element data lives in a separate heap allocation pointed to by _M_start. The vector object on the stack (or wherever it's declared) never grows — only the heap buffer it points to grows.

cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v = {1, 2, 3};
    std::cout << "sizeof(vector<int>) = " << sizeof(v) << '\n';  // 24
}
Q2. Difference between size() and capacity()?

Answer: size() is the number of elements actually constructed and usable right now. capacity() is how many elements the current heap allocation could hold before a reallocation is needed. capacity() >= size() always holds.

cpp
std::vector<int> v;
v.reserve(10);
v.push_back(1);
v.push_back(2);

std::cout << "size = "     << v.size()     << '\n';   // 2
std::cout << "capacity = " << v.capacity() << '\n';    // 10

The gap between them is unused, already-allocated memory — pushing more elements is free (no allocation) until size() == capacity().

Q3. Is vector's storage guaranteed to be contiguous?

Answer: Yes — guaranteed since C++03, and made unambiguous by C++11 when data() was added to the standard specifically to expose this guarantee directly. This is the defining property that separates vector from deque/list, and it's why vector<T> can be handed to any C API expecting T* + a count:

cpp
void legacy_c_api(const int* buf, size_t n);

std::vector<int> v = {1, 2, 3, 4, 5};
legacy_c_api(v.data(), v.size());   // always safe — guaranteed contiguous
Q4. What's on the stack vs the heap for a vector<int> v(1000)?

Answer:

Stack (or wherever v itself lives): the 24-byte vector object (3 pointers).
Heap: one contiguous allocation of 1000 * sizeof(int) = 4000 bytes, holding the actual elements.
cpp
std::vector<int> v(1000);
// &v            -> stack address (24 bytes)
// v.data()      -> heap address  (4000 bytes)
Category 2: Growth Policy & Amortized Complexity
Q5. How does vector grow when push_back exceeds capacity? What's the growth factor?

Answer: When size() == capacity() and another element is pushed, the vector:

Allocates a new, larger buffer.
Moves (or copies) all existing elements into it.
Destroys the old elements.
Deallocates the old buffer.

The growth factor is implementation-defined, not standard-mandated:

GCC libstdc++: doubles capacity (2×)
MSVC STL: grows by 1.5×
Clang libc++: doubles capacity (2×)
cpp
std::vector<int> v;
size_t last_cap = v.capacity();

for (int i = 0; i < 20; ++i)
{
    v.push_back(i);
    if (v.capacity() != last_cap)
    {
        std::cout << "size=" << v.size()
                   << " new capacity=" << v.capacity() << '\n';
        last_cap = v.capacity();
    }
}
// On GCC: capacity sequence typically goes 1, 2, 4, 8, 16, 32...
Q6. Why is push_back called "amortized O(1)"? Prove it mathematically.

Answer: Individual push_back calls are sometimes O(n) (when reallocation triggers a full copy), but averaged over n insertions, the cost per insertion is O(1). This is the classic aggregate analysis argument.

With doubling growth starting at capacity 1, reallocations happen at sizes 1, 2, 4, 8, ..., 2^k where 2^k >= n.

Total copying cost = 1 + 2 + 4 + ... + 2^k
                   = 2^(k+1) - 1
                   < 2 * 2^k
                   < 4n            (since 2^k < 2n)

Plus n actual element insertions = n

Total cost for n push_backs < 4n + n = 5n

Amortized cost per push_back < 5n / n = O(1)

The geometric series is the key insight: even though reallocation copies grow exponentially, they happen exponentially less often, and the sum of a geometric series is bounded by a constant multiple of the last term.

cpp
#include <chrono>
#include <vector>
#include <iostream>

int main()
{
    const int N = 10'000'000;
    auto t0 = std::chrono::steady_clock::now();

    std::vector<int> v;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(i);
    }

    auto t1 = std::chrono::steady_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    std::cout << "Total: " << us << " us, per-element: "
               << (double)us / N << " us\n";
    // per-element time stays roughly constant regardless of N — proof of O(1) amortized
}
Q7. Difference between reserve() and resize()?

Answer:

	reserve(n)	resize(n)
Changes	capacity() only	size() (and capacity if growing)
Effect on size()	unchanged	becomes n
New elements	not constructed	value-initialized (or default-constructed)
Shrinking	no-op if n <= capacity()	destroys elements if n < size()
cpp
std::vector<int> v = {1, 2, 3};

v.reserve(100);
std::cout << v.size() << " " << v.capacity() << '\n';   // 3 100 — size unchanged!

v.resize(5);
std::cout << v.size() << " " << v[4] << '\n';           // 5 0   — new elements are 0

v.resize(2);
std::cout << v.size() << '\n';                          // 2 — extra elements destroyed

Rule: always call reserve() before a known bulk push_back loop — it eliminates every intermediate reallocation.

Q8. What does shrink_to_fit() do? Is it guaranteed to actually shrink?

Answer: shrink_to_fit() is a non-binding request to reduce capacity() to size(). The standard explicitly allows implementations to ignore it. In practice, GCC and Clang honor it in most cases, but it is not guaranteed.

cpp
std::vector<int> v(1000);
v.resize(10);
std::cout << v.capacity() << '\n';   // still 1000

v.shrink_to_fit();
std::cout << v.capacity() << '\n';   // likely 10, but NOT guaranteed by standard
Q9. What's the guaranteed way to release a vector's excess memory?

Answer: The swap-with-temporary idiom — swapping with a freshly-constructed vector of the exact right size is guaranteed to work because it doesn't rely on any "hint" being honored:

cpp
std::vector<int> v(1000);
v.resize(10);

std::vector<int>(v).swap(v);
// Copy-constructs a temporary with capacity == size (10),
// swaps internals with v, temporary (holding old 1000-capacity buffer)
// is destroyed at the end of the statement — GUARANTEED release.

std::cout << v.capacity() << '\n';   // guaranteed 10

To release everything (empty the vector and free memory):

cpp
std::vector<int>().swap(v);   // v is now empty with 0 capacity — guaranteed
Category 3: Iterators, References & Invalidation
Q10. When exactly are vector iterators, pointers, and references invalidated?

Answer: This is the single most-asked vector question in senior interviews. Full rule table:

Operation	If reallocation happens	If it doesn't
push_back / emplace_back	ALL iterators/pointers/refs invalidated	only end() invalidated
insert(pos, val)	ALL invalidated	elements at/after pos invalidated; before pos stay valid
erase(pos)	never reallocates	elements at/after pos invalidated; before pos stay valid
reserve(n)	if n > capacity(): ALL invalidated	no-op, nothing invalidated
resize(n)	same rule as push_back/erase depending on grow/shrink	—
clear()	—	ALL element iterators invalidated
assign()	—	ALL invalidated
swap(other)	iterators stay valid — but now refer to the OTHER vector's contents	—

swap() is the interesting exception — it's the only vector operation that does not invalidate iterators, because it just swaps the three internal pointers. But the iterator now semantically belongs to whichever vector received the swapped-in buffer.

cpp
std::vector<int> a = {1, 2, 3};
std::vector<int> b = {10, 20};

auto it = a.begin();
a.swap(b);

std::cout << *it << '\n';   // prints 1 — iterator is still valid,
                             // but now dereferences through b's old memory,
                             // which a now owns after the swap
Q11. What happens if you push_back a value that is itself a reference into the same vector — e.g. v.push_back(v[0])?

Answer: This used to be a famous undefined-behavior trap. Since C++11 (LWG issue 526 resolution), standard library containers are required to behave correctly even when an argument aliases an element already inside the container — implementations must effectively copy the source value before any reallocation invalidates it.

Modern GCC libstdc++ and Clang libc++ handle this correctly. That said, it remains a classic interview trap and defensive habit worth knowing, since not every standard-library-like container (custom containers, some embedded-toolchain STL ports) honors this guarantee.

cpp
std::vector<int> v = {1, 2, 3};
v.push_back(v[0]);            // well-defined since C++11 on GCC/Clang — v becomes {1,2,3,1}

// Defensive style still worth knowing for less-standard-conforming code:
int tmp = v[0];
v.push_back(tmp);
Q12. If you erase() one element from the middle, what exactly gets invalidated?

Answer: All iterators/pointers/references at or after the erased position are invalidated (because every subsequent element is shift-copied one slot left). Everything before the erased position remains valid. erase() never triggers a reallocation.

cpp
std::vector<int> v = {10, 20, 30, 40, 50};

auto it_before = v.begin();       // points to 10
auto it_erased = v.begin() + 2;   // points to 30

auto next = v.erase(it_erased);   // erases 30
// it_before is STILL VALID (points to 10)
// it_erased is now DANGLING — do not use it
// next points to the element that took 30's place — now 40

std::cout << *it_before << " " << *next << '\n';   // 10 40
Q13. What is the erase-remove idiom, and why is it needed?

Answer: std::remove/std::remove_if don't actually shrink the container — they can't, since algorithms only have iterators, not access to the container's erase(). They shift the "kept" elements to the front and return an iterator to the new logical end, leaving the tail elements in an unspecified but valid state. You must follow up with erase() to actually shrink size().

cpp
std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

// Remove all even numbers
v.erase(
    std::remove_if(v.begin(), v.end(), [](int x)
    {
        return x % 2 == 0;
    }),
    v.end()
);

for (int x : v)
{
    std::cout << x << ' ';   // 1 3 5 7
}

C++20 alternative — std::erase_if does both steps in one call:

cpp
std::erase_if(v, [](int x)
{
    return x % 2 == 0;
});
Q14. How do you safely erase elements while iterating over a vector?

Answer: Never increment a "dangling" iterator returned by erase. Use the returned iterator as your new cursor, and only increment manually in the non-erase branch:

cpp
std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

for (auto it = v.begin(); it != v.end(); )
{
    if (*it % 2 == 0)
    {
        it = v.erase(it);    // erase returns the next valid iterator
    }
    else
    {
        ++it;                 // only advance when NOT erasing
    }
}

for (int x : v)
{
    std::cout << x << ' ';   // 1 3 5 7 9
}

The classic bug is writing v.erase(it); ++it; unconditionally — after erase, it already points to the next element, so an extra ++it skips one and can eventually run past end().

Category 4: Move Semantics & Performance
Q15. How does noexcept on a type's move constructor affect vector's reallocation behavior?

Answer: This is a critical, frequently-missed detail. During reallocation, vector must decide: move the old elements into the new buffer, or copy them? It uses std::move_if_noexcept internally, which checks is_nothrow_move_constructible<T>.

If T's move constructor is noexcept → vector moves elements (fast, O(1) per element for heap-owning types).
If T's move constructor is not noexcept (and a copy constructor exists) → vector copies elements instead.

Why? To preserve push_back's strong exception guarantee. If a move constructor could throw partway through reallocation, the vector could end up in a corrupted, half-moved state with no way to roll back (the source elements are already partially destroyed). Copying is fully rollback-safe (the old buffer stays untouched until every copy succeeds), so the standard mandates falling back to copy whenever move might throw.

cpp
#include <vector>
#include <iostream>

struct NoexceptMove
{
    std::vector<int> data;

    NoexceptMove() : data(1000) {}
    NoexceptMove(NoexceptMove&&) noexcept = default;      // fast path
    NoexceptMove(const NoexceptMove&) = default;
};

struct ThrowingMove
{
    std::vector<int> data;

    ThrowingMove() : data(1000) {}
    ThrowingMove(ThrowingMove&&) = default;                // NOT marked noexcept
    ThrowingMove(const ThrowingMove&) = default;
};

int main()
{
    std::cout << std::boolalpha;
    std::cout << "NoexceptMove is_nothrow_move_constructible: "
               << std::is_nothrow_move_constructible<NoexceptMove>::value << '\n';   // true

    std::cout << "ThrowingMove is_nothrow_move_constructible: "
               << std::is_nothrow_move_constructible<ThrowingMove>::value << '\n';   // false

    // Reallocating a vector<ThrowingMove> will COPY every element instead of moving,
    // silently costing far more than expected — a real, measurable perf bug.
}

Rule: always mark your move constructors noexcept if they genuinely can't throw. Forgetting this is one of the most common silent performance bugs in real C++ codebases.

Q16. push_back vs emplace_back — what's the actual difference?

Answer: push_back(T) takes an already-constructed T (or something convertible to T) and copies/moves it in. emplace_back(args...) forwards the arguments directly to T's constructor, building the object in-place inside the vector's storage — no temporary object is created at all.

cpp
#include <vector>
#include <string>

struct Point
{
    int x, y;

    Point(int x_, int y_) : x(x_), y(y_)
    {
        std::cout << "Point(" << x << "," << y << ") constructed\n";
    }
};

int main()
{
    std::vector<Point> v;
    v.reserve(2);

    v.push_back(Point(1, 2));    // constructs a temporary Point, then MOVES it in
    v.emplace_back(3, 4);        // constructs the Point directly in vector's storage — no temp
}

For simple types like int, the difference is negligible (compilers often optimize the temporary away anyway). For types with expensive constructors — especially ones holding heap resources — emplace_back avoids a full extra construct+move/copy+destroy cycle.

Q17. Does returning a vector by value from a function cause a copy?

Answer: No — since C++17, mandatory copy elision (guaranteed RVO) applies for prvalues, and NRVO (named return value optimization) applies in almost all compilers even before C++17 for a single named local. The vector is constructed directly in the caller's storage — zero copies, zero moves.

cpp
std::vector<int> make_vector()
{
    std::vector<int> v = {1, 2, 3};
    return v;               // NRVO — constructed in-place at the call site, no copy/move
}

std::vector<int> make_vector_bad()
{
    std::vector<int> v = {1, 2, 3};
    return std::move(v);    // ANTI-PATTERN: disables NRVO, forces an unnecessary move
}

Never write return std::move(local_vector); — it actively prevents the compiler's zero-cost optimization and downgrades it to a (still cheap, but strictly worse) move.

Q18. vector<T> vs vector<T*> vs vector<unique_ptr<T>> vs vector<shared_ptr<T>> — trade-offs?

Answer:

	Ownership	Copy cost	Cache locality	Polymorphism
vector<T>	vector owns objects directly	full object copy	best (contiguous objects)	no — object slicing risk
vector<T*>	no ownership (dangling risk)	pointer copy (cheap)	poor (objects scattered on heap)	yes
vector<unique_ptr<T>>	vector exclusively owns	move-only (no copies)	poor	yes
vector<shared_ptr<T>>	shared ownership	atomic refcount increment	poor	yes
cpp
class Shape { public: virtual double area() const = 0; virtual ~Shape() = default; };
class Circle : public Shape { /* ... */ };
class Square : public Shape { /* ... */ };

// Polymorphic collection — must use pointers, not vector<Shape>
// (vector<Shape> would slice every Circle/Square down to just the Shape base part)
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>());
shapes.push_back(std::make_unique<Square>());

vector<T> wins on raw iteration speed (contiguous, prefetcher-friendly). Reach for pointer-based vectors only when you genuinely need polymorphism or shared ownership — never as a default.

Category 5: The vector<bool> Trap
Q19. What's wrong with std::vector<bool>? Why do senior engineers avoid it?

Answer: vector<bool> is a template specialization, not a real container in the usual STL sense. To save memory, it packs booleans as individual bits (1 bit per element) instead of the usual 1 byte per bool.

Because there's no addressable "1-bit object" in C++, operator[] cannot return a real bool&. Instead it returns a proxy object (std::vector<bool>::reference) that fakes reference-like behavior via overloaded operator= and operator bool().

cpp
std::vector<bool> v = {true, false, true};

auto x = v[0];              // x is NOT a bool — it's vector<bool>::reference (a proxy!)
// If v reallocates later, x can become a DANGLING proxy — silent bug

bool& ref = v[0];            // COMPILE ERROR — cannot bind proxy to a real bool&

void takes_ref(bool& b);
// takes_ref(v[0]);          // COMPILE ERROR — same reason

This breaks generic code that assumes Container::reference is a real T&, and it's one of the most-cited examples of "the STL has a design mistake" in C++ history (Scott Meyers wrote an entire Item about it in Effective STL).

Q20. How do you work around the vector<bool> problem?

Answer: Three common alternatives, chosen by what you actually need:

cpp
// Option 1: real bool references, 1 byte per element (no bit-packing)
std::vector<char> flags(100, 0);   // use 0/1 as bool

// Option 2: fixed-size, compile-time-known count, zero container overhead
std::bitset<64> bits;
bits.set(3);
bool b = bits[3];                   // real bool, no proxy issue

// Option 3: dynamic-size bitset with a bitset-like API (not in std, use Boost)
// boost::dynamic_bitset<> dbits(1000);

std::deque<bool> is also an option — it is not specialized like vector<bool> and stores real bools, at the cost of deque's usual (slightly worse) cache locality vs a flat array.

Category 6: Comparison With Other Containers
Q21. vector vs deque vs list — complexity and cache trade-offs?

Answer:

	Memory layout	Random access	push_front	push_back	insert/erase middle	Cache locality
vector	one contiguous block	O(1)	O(n)	O(1) amortized	O(n)	best
deque	array of fixed-size blocks	O(1) (extra indirection)	O(1)	O(1)	O(n)	moderate
list	separate heap node per element	O(n)	O(1)	O(1)	O(1) (given iterator)	worst
cpp
// Benchmark intuition: list "wins" on paper for middle insertion (O(1) vs O(n)),
// but in practice, on real hardware, vector often beats list even for
// insert-heavy workloads up to surprisingly large N, because:
//   - list: every access is a pointer-chase to a separately-allocated node
//           (cache miss on nearly every dereference)
//   - vector: sequential memory access lets the CPU prefetcher hide latency

Default to vector. Reach for deque only when you genuinely need O(1) push at both ends. Reach for list only when you need O(1) splice or truly frequent middle insertion/erasure with iterators already in hand — and even then, profile first.

Q22. vector<T> vs std::array<T,N> — what's actually different?

Answer:

	vector<T>	array<T,N>
Size	dynamic, runtime	fixed, compile-time (N is part of the type)
Storage	heap (via internal pointer)	wherever the array itself lives (stack, member, etc.)
Overhead	24 bytes (3 pointers) + heap block	zero — just N * sizeof(T), nothing extra
Decay in function params	n/a (already reference/pointer semantics via .data())	a raw T[N] decays to T*; array<T,N> does NOT — no decay
.size()	O(1) runtime	O(1), often constexpr
cpp
void f(std::array<int, 5>& arr)
{
    std::cout << arr.size();   // always 5, known at compile time
}

void g(int arr[5])   // this is secretly just int*  — the [5] is decorative
{
    // sizeof(arr) here is 8 (pointer), NOT 20 — classic array decay trap
}
Q23. Why can vector<vector<T>> hurt performance compared to a flat 1D vector simulating 2D?

Answer: vector<vector<T>> stores each row as a separately heap-allocated buffer. Iterating row-by-row means:

Following a pointer to find each row (pointer-chasing, cache miss).
Rows can end up scattered anywhere in the heap — no guarantee of adjacency.
Extra per-row overhead: each inner vector costs 24 bytes just for its own bookkeeping.

A flat vector with manual row-major indexing keeps everything in one contiguous block:

cpp
// Poor cache locality — rows scattered across the heap
std::vector<std::vector<int>> matrix2d(1000, std::vector<int>(1000));

// Excellent cache locality — one contiguous 4MB block
std::vector<int> matrix_flat(1000 * 1000);
auto at = [&](int row, int col) -> int&
{
    return matrix_flat[row * 1000 + col];
};

// Benchmark difference on large matrices is often 2-5x for sequential scans,
// due to prefetcher effectiveness on the flat layout vs pointer-chasing on 2D.
Category 7: Exception Safety & Advanced Corner Cases
Q24. What exception safety guarantee does push_back provide?

Answer: push_back provides the strong exception guarantee: if it throws, the vector is left exactly as it was before the call — no partial state, no leak.

No reallocation needed: trivially strong — either the new element is constructed successfully, or it isn't; nothing else changes.
Reallocation needed: the new buffer is built up (moved or copied from the old one) before the old buffer is destroyed. If any move/copy throws partway through, the operation aborts and the original buffer is left completely untouched.

This is exactly why the noexcept-move-vs-copy decision from Q15 exists — a throwing move constructor would make it impossible to guarantee this rollback, so the standard forces a fallback to copying in that case.

Q25. at() vs operator[] — what's the real trade-off?

Answer:

cpp
std::vector<int> v = {1, 2, 3};

v.at(10);        // throws std::out_of_range — bounds-checked
v[10];            // UNDEFINED BEHAVIOR — no bounds check, may read garbage or segfault

operator[] is unchecked by design for raw performance in hot paths (this mirrors C array semantics). at() pays for a branch and potential exception machinery on every call. Rule of thumb: use operator[] in performance-critical code where the index is provably in-bounds by logic; use at() at API boundaries or anywhere the index comes from untrusted/external input.

Q26. Can you use a custom allocator with vector? Why would you?

Answer: Yes — vector<T, Allocator> is the full template signature; Allocator defaults to std::allocator<T> but can be replaced. Common reasons: pool allocators (fast fixed-size allocation), arena/stack allocators (zero heap allocation for scratch buffers), or tracking/debugging allocators.

cpp
#include <memory_resource>
#include <vector>
#include <array>

int main()
{
    std::array<std::byte, 4096> buffer;
    std::pmr::monotonic_buffer_resource pool(buffer.data(), buffer.size());

    std::pmr::vector<int> v(&pool);   // C++17 PMR vector — allocates from the stack buffer
    v.push_back(1);
    v.push_back(2);
    // Zero heap allocations occur, as long as `buffer` is large enough
}
Q27. What's the right way to pass a vector to a function?

Answer:

cpp
void read_only(const std::vector<int>& v);     // DEFAULT CHOICE — no copy, read-only
void modify_in_place(std::vector<int>& v);     // read-write, caller's vector modified

// "Sink" pattern — function takes ownership:
void take_ownership(std::vector<int> v)         // by value — cheap if caller passes an rvalue
{
    member_ = std::move(v);                      // move into storage — no extra copy
}

// Usage:
std::vector<int> data = {1, 2, 3};
take_ownership(data);              // COPIES data (data is an lvalue)
take_ownership(std::move(data));   // MOVES data — zero-cost, data is now empty
take_ownership({1, 2, 3});          // constructs temporary in-place, then moves — zero-cost

For read-only access when you don't need vector-specific operations (push_back etc.), C++20 std::span<const T> is even more general — it accepts vector, array, or a raw array identically:

cpp
void process(std::span<const int> data);   // works with vector<int>, array<int,N>, int[N]
Q28. Is v = v; (self-assignment) safe? What about v.assign(v.begin(), v.end())?

Answer: operator= must handle self-assignment safely per the standard's container requirements — this is guaranteed:

cpp
std::vector<int> v = {1, 2, 3};
v = v;                 // guaranteed safe, no-op effectively

assign() with a range that aliases the vector's own elements is a different story — it is generally not specified to be self-reference-safe the way push_back is (per Q11's LWG 526 resolution, which is specific to single-element insertion). Avoid it:

cpp
std::vector<int> v = {1, 2, 3};
v.assign(v.begin(), v.end());   // AVOID — not guaranteed safe, implementation-dependent
Quick-Fire Recap (for a 2-minute oral drill)
sizeof(vector<T>)         -> 24 bytes on 64-bit (3 pointers)
Growth factor             -> 2x (GCC/Clang), 1.5x (MSVC) — implementation-defined
push_back amortized cost  -> O(1), proven via geometric series (<5n for n inserts)
reserve() vs resize()     -> capacity only vs size (+ construct/destroy elements)
Guaranteed shrink         -> vector<T>(v).swap(v)
Iterator invalidation     -> reallocation invalidates ALL; erase invalidates from point onward
swap() and iterators      -> the ONE op that does NOT invalidate iterators
noexcept move impact      -> throwing move ctor => vector copies instead during realloc
vector<bool>               -> bit-packed, operator[] returns a PROXY not bool&
Contiguous guarantee      -> since C++03, exposed via data() since C++11
at() vs operator[]        -> bounds-checked+throws vs unchecked UB

*/




#include <iostream>

int main()
{
        int a = 5 , b= 10;

        // define lamda function
        auto addAndPrint = [] (int x, int y)
        {
                int c = x + y;
                std::cout<<"c = "<<c<<std::endl;
                x = c;
        };

        auto addAndPrint1 = [a,b] ()
        {
                int c = a + b;
                std::cout<<"c = "<<c<<std::endl;
                a = c;
        };

        addAndPrint(a,b);
        addAndPrint1();

        auto addAndPrint2 = [a,b] ()
        {
                int c = a + b;
                std::cout<<"c = "<<c<<std::endl;
        };
        addAndPrint2();


        return 0;


/*
deepkSeek

Lambda Functions in C++ — Complete Detailed Reference

Below is the full expanded version — every section in depth, with runnable code, outputs, complexity/overhead notes, and interview soundbites. Structured so you can study it or deliver it.
Part 1 — What a Lambda Actually Is
1.1 The compiler's view

A lambda expression is not a function. It is an expression that constructs an object of a unique, compiler-generated class type called a closure type. That class:

    Has a public operator() whose signature matches what you wrote.

    Has data members for each captured entity (a copy for value-capture, a reference for reference-capture).

    Is unique per lambda expression — two textually identical lambdas have different types.

    Is not default-constructible if it has captures (C++11/14/17); C++20 grants default constructibility to captureless lambdas.

Mentally translate:
cpp

int base = 10;
auto f = [base](int x) { return x + base; };

into what the compiler roughly generates:
cpp

class __Lambda_1234 {
    int base;                       // captured copy
public:
    __Lambda_1234(int b) : base(b) {}
    int operator()(int x) const {   // note: const by default
        return x + base;
    }
};
__Lambda_1234 f{base};

    Interview soundbite: "Lambdas are sugar over function objects. The compiler synthesizes a closure class with an operator() and capture data members. Because the type is unique per expression, lambdas passed to templates are fully inlinable — zero overhead."

1.2 Why this matters

    Uniqueness → each lambda is a distinct type → templates specialize per lambda → inlining → zero overhead.

    Closure state → captures live inside the object → lifetime is tied to the closure object, not the lambda expression.

    const operator() → a lambda is callable through a const reference by default (important for storage, containers, std::function).

Part 2 — Full Syntax Anatomy
text

[capture-list] (params) specifiers exception-spec trailing-return { body }
 └ required ┘ └ optional ┘ └───────────────────── all optional ──────────┘

Component	Example	Required?	Notes
Capture list	[x, &y]	Yes (can be empty [])	Defines closure members
Parameters	(int a, int b)	Optional (C++11 had no default args; C++14 allows auto params)	Omit if no params
mutable	mutable	Optional	Drops const on operator()
constexpr	constexpr	Optional (C++17)	Force compile-time evaluability
consteval	consteval	Optional (C++20)	Must be compile-time
noexcept	noexcept	Optional	Exception specification
Attributes	[[nodiscard]]	Optional	e.g., [[nodiscard]] before params
Trailing return	-> int	Optional	Required if return type can't be deduced
Body	{ return x; }	Yes (can be empty {})	

Minimal lambda: []{} — capture nothing, take nothing, return nothing.

Maximal example:
cpp

int y = 2;
auto f = [x = 1, &y](int a) mutable constexpr noexcept -> int {
    y += a;
    return x + y;
};

Part 3 — Capture Semantics (Deep)
3.1 Complete capture table
Syntax	Meaning	Closure member
[]	Nothing	—
[=]	Default by value (copy all odr-used locals)	copies
[&]	Default by reference (reference all odr-used locals)	references
[x]	x by value	T x; (copy)
[&x]	x by reference	T& x;
[=, &x]	Default value, x by ref	mixed
[&, x]	Default ref, x by value	mixed
[this]	Capture this pointer	C* this;
[*this]	Copy *this (C++17)	C obj;
[x = expr]	Init-capture (C++14)	member initialized from expr
[p = std::move(p)]	Move into closure (C++14)	moved member
[&x = expr]	Init-capture by reference (C++14)	reference to lvalue expr
[...pack]	Pack capture (C++20)	pack expansion of captures
3.2 What "capture" really means — odr-use

A lambda captures only variables it odr-uses in the body. If you name a variable but don't use it (e.g., only in sizeof), it's not captured.
cpp

int a = 1, b = 2;
auto f = [=] { return a; };  // only `a` is captured; `b` is not

[=]/[&] are defaults, not blanket captures. The compiler captures the minimum set of odr-used entities.
3.3 Value-capture: copy at creation time
cpp

int x = 5;
auto f = [x] { return x; };
x = 100;
std::cout << f();   // prints 5 — the closure holds a snapshot

The copy is made when the lambda expression is evaluated (i.e., when the closure is constructed). Mutating x afterward does not affect the closure.

Value-captured members are const inside a non-mutable lambda body. So this fails to compile:
cpp

auto f = [x] { x++; };   // error: assignment to const member
auto g = [x]() mutable { x++; };  // OK — modifies the closure's copy

3.4 Reference-capture: a reference stored in the closure
cpp

int x = 5;
auto f = [&x] { return x; };
x = 100;
std::cout << f();  // 100 — reads through the reference

The closure stores a T&. The referent must outlive the closure. This is the canonical UB source:
cpp

auto make() {
    int local = 42;
    return [&local] { return local; };   // UB: local destroyed
}
auto f = make();
f();   // reads freed stack memory — undefined behavior

    Interview soundbite: "Reference capture does not extend lifetime. If the closure escapes the scope of the referent — returned, stored, posted to a thread — it's a dangling reference. Use value-capture or [p = std::move(p)] with a shared_ptr/unique_ptr when the closure must outlive the scope."

3.5 this capture — the subtle one

Inside a member function, [=] captures this by value (a pointer copy) — not the object. So:
cpp

struct Foo {
    int v = 0;
    auto makeLambda() {
        return [=] { return v; };   // captures this pointer (implicitly)
    }
};

The lambda's body does this->v, and the closure holds a Foo*. If Foo dies, the returned lambda dangles.

C++17 fix — [*this]:
cpp

auto makeLambda() {
    return [*this] { return v; };   // copies the entire *this into the closure
}

[*this] performs a copy of the whole object at closure construction, so the lambda is self-contained and safe to outlive the original.

C++20 note: [=] still implicitly captures this, but some compilers warn; the recommended style is explicit [this] or [*this].
3.6 Init-capture (C++14) — move and compute

Init-capture lets you capture the result of an arbitrary expression under a new name:
cpp

auto p = std::make_unique<Widget>();
auto f = [p = std::move(p)] { return p->size(); };   // moves unique_ptr into closure
// p is now null; the closure owns the widget

Without init-capture, [p] would try to copy a unique_ptr — compile error. So init-capture is the idiomatic way to move move-only objects into closures.

More examples:
cpp

int a = 1, b = 2;
auto f1 = [x = a + b] { return x; };         // capture computed value 3
auto f2 = [&r = a] { return r; };            // reference to a (lvalue)
auto f3 = [v = std::vector<int>{1,2,3}] { return v.size(); };  // move vector in

3.7 Pack capture (C++20)
cpp

template <typename... Ts>
auto captureAll(Ts... ts) {
    return [...ts = std::move(ts)] { return (ts + ...); };  // fold over captures
}

Useful for variadic forwarding into closures.
Part 4 — mutable in Detail
4.1 Why it exists

The closure's operator() is const by default:
cpp

class Closure {
    int x;
public:
    int operator()() const { return x; }   // const
};

Because it's const, you can't mutate captured-by-value members. mutable removes that const:
cpp

auto f = [x]() mutable { return ++x; };  // OK

4.2 What mutable does NOT do

    It does not affect reference-captured variables — the referent is already non-const. [&x] lets you modify x even without mutable.

    It does not propagate back to the original value-captured variable:

cpp

int x = 0;
auto f = [x]() mutable { return ++x; };
std::cout << f() << f() << '\n';   // 1 2 — closure's copy increments
std::cout << x << '\n';            // 0 — original untouched

4.3 When mutable matters in practice

    Stateful callbacks: a counter, a cache, a "called once" flag stored in the closure.

    Storing a lambda in a const context and still needing to mutate internal state.

cpp

auto counter = [count = 0]() mutable { return ++count; };

Without mutable, count is const in the body → compile error.
Part 5 — The Closure Type, Formally
5.1 Generated members

For a lambda with captures, the compiler generates:

    A constructor (not default, not user-callable) initializing the capture members.

    operator() — const unless mutable.

    Copy constructor, move constructor (if members are copyable/movable).

    Deleted copy/move assignment before C++20; in C++20, assignment is generated if members are assignable.

    No default constructor (until C++20, captureless lambdas get one).

5.2 Captureless lambda → function pointer

A captureless lambda has an implicit conversion to a function pointer:
cpp

int (*fp)(int) = [](int x) { return x * 2; };

Why? Because a captureless lambda is effectively a stateless function — the generated operator() can be a static call. This is how you pass lambdas to C APIs (qsort, pthread_create, std::atexit, signal).

A capturing lambda has no such conversion — the closure needs this to access its members, which a plain function pointer cannot represent.
5.3 Overload resolution

A non-generic lambda's operator() is not a template — it has a fixed signature. So:
cpp

auto f = [](int x) { return x; };
f(1);      // OK
f(1.5);    // implicit double->int conversion, not deduction

A generic lambda ([](auto x){...}) is a template operator() — deduction applies:
cpp

auto g = [](auto x) { return x + x; };
g(1);     // int
g(1.5);   // double

5.4 constexpr and consteval lambdas

    C++17: lambdas are implicitly constexpr if they satisfy constexpr rules; you can also write []() constexpr { ... } explicitly.

    C++20: consteval lambdas must be evaluated at compile time.

cpp

constexpr auto sq = [](int x) constexpr { return x * x; };
static_assert(sq(5) == 25);   // compile-time evaluation

5.5 C++20 generic lambda with explicit template params
cpp

auto f = []<typename T>(T x) { return x; };

Useful when you need to name the template parameter (e.g., for requires clauses or perfect forwarding).
Part 6 — Performance, Storage & Overhead (the Linux angle)
6.1 Zero-overhead when passed to a template
cpp

template <typename F>
void forEach(std::vector<int>& v, F f) {
    for (auto& x : v) f(x);
}
forEach(v, [](int& x) { x *= 2; });

Here F is the closure's unique type. The compiler instantiates forEach<ClosureType>, inlines operator(), and produces code identical to a hand-written loop. No allocation, no indirect call.

Closure size = sum of captured members:

    [] → sizeof == 1 (empty class).

    [x] (int) → sizeof == sizeof(int) (plus padding).

    [&x] → sizeof == sizeof(T*).

    [a, b, c] → sum of their sizes (alignment padded).

6.2 std::function — type erasure overhead

std::function<R(Args)> is a type-erased wrapper. Storing a lambda in it can cost:

    Heap allocation if the closure's size exceeds the small-buffer (typically 16 bytes on libstdc++/libc++).

    Indirect call per invocation — operator() cannot be inlined through the type-erased interface.

cpp

std::function<int(int)> f = [base = 10](int x) { return x + base; };
// likely heap-allocates (8-byte capture + vtable pointer may exceed SBO)
// every f() call is an indirect dispatch

cpp

// Zero-overhead alternative
auto f = [base = 10](int x) { return x + base; };   // stack, inlinable

Rule of thumb for a 5-year engineer:

    "Use auto F/template<typename F> in hot paths — zero overhead. Use std::function only when you need a stable type-erased interface: callback registries, storing heterogeneous callables in a container, or crossing an ABI boundary. Profile before assuming std::function is fine in a hot loop."

6.3 Recursive lambdas

A lambda cannot name itself. Options:

Option A — std::function (simple, slow):
cpp

std::function<int(int)> fib = [&](int n) {
    return n < 2 ? n : fib(n - 1) + fib(n - 2);
};

Option B — Y-combinator / self-passing (zero overhead):
cpp

auto fib = [](auto self, int n) -> int {
    return n < 2 ? n : self(self, n - 1) + self(self, n - 2);
};
int r = fib(fib, 10);

Option C — capture a reference to a function object / struct with operator() — often clearest for complex recursion.
6.4 Copy and move of closures

    The closure is copyable if all captured members are copyable.

    The closure is movable if all captured members are movable (C++20 formalizes assignment rules).

    Moving a closure moves its captures:

cpp

auto p = std::make_unique<int>(5);
auto f = [q = std::move(p)] { return *q; };
auto g = std::move(f);   // moves the unique_ptr into g

6.5 [[no_unique_address]] (C++20) and empty closures

A captureless lambda is an empty class (sizeof == 1). As a member of another struct, C++20's [[no_unique_address]] lets the compiler give it zero size:
cpp

struct S {
    [[no_unique_address]] F f;   // empty lambda can occupy 0 bytes
    int x;
};

Relevant for zero-cost policy/comparator wrappers (e.g., std::less).
Part 7 — Lambdas in the Linux/Systems Context
7.1 C APIs need function pointers

pthread_create, epoll callbacks, signal, atexit, qsort, bsearch — all take raw function pointers. Capturing lambdas cannot be passed directly.
cpp

// Cannot do this — pthread_create needs a function pointer:
// pthread_create(&t, nullptr, [&](void*) -> void* { ... }, arg);

Trampoline pattern:
cpp

struct Ctx { int value; };
extern "C" void* thread_fn(void* arg) {
    auto* c = static_cast<Ctx*>(arg);
    // work
    return nullptr;
}
pthread_create(&t, nullptr, thread_fn, &ctx);

For captureless lambdas, the implicit conversion to a function pointer works:
cpp

void (*fn)(int) = [](int x) { /*...*/ };
qsort(arr, n, sizeof(int), [](const void* a, const void* b) -> int {
    return *static_cast<const int*>(a) - *static_cast<const int*>(b);
});  // captureless lambda converts to function pointer — OK

7.2 Threading lifetimes

Value-capturing a shared_ptr extends lifetime into the thread:
cpp

auto p = std::make_shared<Resource>();
std::thread t([p] { use(*p); });   // p kept alive until t's closure is destroyed
t.join();

Reference-capturing a local that dies before the thread runs is UB:
cpp

void spawn() {
    int x = 42;
    std::thread t([&x] { use(x); });   // UB if spawn() returns before t runs
    t.detach();
}

This is the #1 concurrency bug in Linux codebases — always capture by value (or a shared_ptr) for work posted to threads.
7.3 Callback registries

Storing callbacks with different captures requires type erasure:
cpp

std::vector<std::function<void()>> callbacks;
callbacks.push_back([a = 1] { /*...*/ });
callbacks.push_back([b = 2, c = 3] { /*...*/ });

Here std::function is the right tool — you need heterogeneous storage. Accept the indirect-call cost.
Part 8 — Common Pitfalls (Production Checklist)
#	Pitfall	Symptom	Fix
1	Dangling reference capture	UB / crash after scope ends	Value-capture; or capture a shared_ptr
2	[=] implicitly captures this	Dangling this in stored lambda	Use [*this] (C++17)
3	mutable + value capture	Original variable unchanged	Understand closure owns a copy
4	std::function in hot loop	Silent alloc + indirect call	Template parameter instead
5	Capturing lambda to C API	Compile error	Trampoline / captureless lambda
6	Generic lambda auto by value	Copies expensive args	auto const&
7	Recursive lambda	Can't name itself	std::function or self-passing
8	Captureless lambda assigned to auto then converted	Extra closure type	Convert explicitly to fnptr
9	Capturing a reference to a temporary	Dangling reference	Capture by value or extend lifetime
10	[&] in a returned lambda	Dangling everything	Use [=] or [*this] / shared_ptr
Part 9 — Complete Runnable Demo Program
cpp

// lambda_demo.cpp
// Compile: g++ -std=c++17 -O2 -Wall -Wextra lambda_demo.cpp -o lambda_demo
// Run:     ./lambda_demo

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include <string>
#include <thread>
#include <cstdio>

// ---------- 1. Captureless lambda -> function pointer ----------
static int (*g_fn)(int) = [](int x) { return x * 2; };

// ---------- 2. Template taking a lambda: zero overhead ----------
template <typename F>
void forEach(std::vector<int>& v, F f) {
    for (auto& x : v) f(x);
}

// ---------- 3. A struct that returns a lambda ----------
struct Foo {
    int v = 7;
    auto byRef()  { return [this] { return v; }; }   // dangles if Foo dies
    auto byCopy() { return [*this] { return v; }; }  // C++17 — safe
};

int main() {
    std::cout << "=== 1. Captureless lambda as function pointer ===\n";
    std::cout << "g_fn(21) = " << g_fn(21) << "\n\n";

    std::cout << "=== 2. Value vs reference capture ===\n";
    int x = 5;
    auto byVal = [x] { return x; };
    auto byRef = [&x] { return x; };
    x = 100;
    std::cout << "byVal() = " << byVal() << "  (snapshot at creation)\n";
    std::cout << "byRef() = " << byRef() << "  (live reference)\n\n";

    std::cout << "=== 3. mutable: closure owns a mutable copy ===\n";
    int counter = 0;
    auto inc = [counter]() mutable { return ++counter; };
    std::cout << "inc() = " << inc() << ", inc() = " << inc()
              << ", original counter = " << counter << "\n\n";

    std::cout << "=== 4. Init-capture: move a unique_ptr into a lambda ===\n";
    auto p = std::make_unique<int>(42);
    auto owns = [q = std::move(p)] { return *q; };
    std::cout << "owns() = " << owns() << ", p is "
              << (p ? "non-null" : "null") << "\n\n";

    std::cout << "=== 5. this vs *this capture ===\n";
    Foo foo;
    auto r = foo.byRef();
    auto c = foo.byCopy();
    foo.v = 999;
    std::cout << "byRef()  = " << r() << "  (sees mutation)\n";
    std::cout << "byCopy() = " << c() << "  (snapshot)\n\n";

    std::cout << "=== 6. Generic lambda ===\n";
    auto show = [](auto const& v) { std::cout << v << ' '; };
    show(1); show(2.5); show(std::string("hi")); std::cout << "\n\n";

    std::cout << "=== 7. Zero-overhead template vs std::function ===\n";
    std::vector<int> v{1, 2, 3, 4};
    forEach(v, [](int& n) { n *= 10; });                  // inlined
    std::function<void(int&)> erased = [](int& n) { n += 1; };  // indirect
    forEach(v, erased);
    for (int n : v) std::cout << n << ' ';
    std::cout << "\n\n";

    std::cout << "=== 8. Recursive lambda (self-passing, zero overhead) ===\n";
    auto fib = [](auto self, int n) -> long long {
        return n < 2 ? n : self(self, n - 1) + self(self, n - 2);
    };
    std::cout << "fib(20) = " << fib(fib, 20) << "\n\n";

    std::cout << "=== 9. constexpr lambda ===\n";
    constexpr auto sq = [](int n) constexpr { return n * n; };
    static_assert(sq(9) == 81, "compile-time check");
    std::cout << "sq(9) = " << sq(9) << " (checked at compile time)\n\n";

    std::cout << "=== 10. Lambda + thread with shared_ptr lifetime ===\n";
    auto shared = std::make_shared<int>(77);
    std::thread t([shared] { std::printf("thread sees %d\n", *shared); });
    t.join();

    return 0;
}

Expected output (order of 5/6 may vary by compiler):
text

=== 1. Captureless lambda as function pointer ===
g_fn(21) = 42

=== 2. Value vs reference capture ===
byVal() = 5  (snapshot at creation)
byRef() = 100  (live reference)

=== 3. mutable: closure owns a mutable copy ===
inc() = 1, inc() = 2, original counter = 0

=== 4. Init-capture: move a unique_ptr into a lambda ===
owns() = 42, p is null

=== 5. this vs *this capture ===
byRef()  = 999  (sees mutation)
byCopy() = 7  (snapshot)

=== 6. Generic lambda ===
1 2.5 hi

=== 7. Zero-overhead template vs std::function ===
11 21 31 41

=== 8. Recursive lambda (self-passing, zero overhead) ===
fib(20) = 6765

=== 9. constexpr lambda ===
sq(9) = 81 (checked at compile time)

=== 10. Lambda + thread with shared_ptr lifetime ===
thread sees 77

Complexity / overhead summary for the demo
Construct	Time	Space	Notes
forEach(v, lambda) template	O(n)	O(1) auxiliary	Inlined, zero overhead
forEach(v, std::function)	O(n)	possible heap alloc for closure	Indirect call per element
fib(fib, 20) self-passing	O(2ⁿ) naive fib	O(n) stack	Zero-overhead recursion
[*this] capture	O(1) at closure creation	copies sizeof(Foo)	Safe outliving
[q = std::move(p)]	O(1)	owns the moved object	Zero-copy move
Part 10 — Evolution Timeline
Standard	Feature
C++11	Lambdas, capture list, mutable, captureless→function pointer
C++14	Generic lambdas (auto params), init-capture ([x = expr]), constexpr-friendly
C++17	[*this], constexpr lambdas explicitly, noexcept in lambdas
C++20	Pack capture, explicit template params ([]<typename T>), consteval lambdas, captureless default-constructible, assignment operators generated
C++23	Minor refinements (attributes on lambdas, etc.)
Part 11 — Model Answer (90-second delivery)

    "A lambda is a compiler-synthesized closure type — an unnamed function object with an operator() and data members for captures. The capture list defines those members: value-capture copies at closure creation and is const in the body unless you mark the lambda mutable; reference-capture stores a reference and is a lifetime hazard if the closure outlives the referent — the classic dangling-capture UB, and [=] implicitly capturing this is the same trap, fixed by C++17's [*this]. Init-capture ([p = std::move(p)], C++14) is how you move move-only objects like unique_ptr into a closure. Captureless lambdas convert to function pointers, which is how you bridge to C APIs like pthread_create; capturing lambdas can't. On performance: a lambda passed to a template is zero-overhead — unique type, inlined operator(), closure size equals the sum of captures — whereas std::function type-erases, may heap-allocate, and forces an indirect call, so I use template<typename F>/auto F in hot paths and reserve std::function for stable interfaces and callback registries. Generic lambdas ([](auto x)) are templated operator(). Recursive lambdas can't name themselves, so I use the self-passing pattern or std::function. And on Linux specifically, the lifetime rules are what bite — value-capture or shared_ptr for anything posted to a thread."

Part 12 — Likely Follow-up Questions (with answers)

Q: Why can't a capturing lambda convert to a function pointer?

    A function pointer has no state. The closure needs its capture members, and operator() needs this to reach them. A captureless lambda's operator() is effectively stateless, so the compiler can emit a free function and a conversion. Captures make that impossible.

Q: How does std::function store a lambda?

    Type erasure: it holds a pointer to a type-erased vtable-like manager plus storage for the closure. Small closures live in an inline buffer (SBO, typically 16 bytes); larger ones heap-allocate. Every call is an indirect dispatch, preventing inlining.

Q: [=] vs [&] — which is safer?

    [=] is safer in terms of dangling because it copies, but it can be expensive and it implicitly captures this by pointer (pre-C++20 semantics), which is itself a dangling risk. [&] is cheaper but a lifetime hazard the moment the closure escapes its scope. The senior answer is: capture explicitly ([x], [&x], [p = std::move(p)]) and use [*this] for object-copy semantics.

Q: What's the size of a lambda?

    Captureless: 1 byte (empty class). Otherwise, the sum of its captured members with alignment padding. A value-captured int → 4 bytes; a reference capture → pointer size.

Q: Are lambdas copyable?

    Copy-constructible iff all captures are copy-constructible. Move-constructible iff all captures are movable. Before C++20, copy/move assignment was deleted; C++20 generates them when members are assignable.

Q: mutable — does it change the original?

    No. It only permits modifying the closure's copy of a value-captured member. The original variable is untouched. With reference capture, there's no copy to mutate — you're modifying the referent directly, mutable or not.

That's the complete detailed treatment. Want me to also produce:

    A one-page cheat sheet (capture rules, pitfalls, complexity table) for quick revision, or

    A Q&A drill (30 lambda interview questions with short answers), or

    The same deep-dive style for another topic (std::function internals, move semantics, shared_ptr/weak_ptr, or constexpr)?


*/

// lambda_demo.cpp
// Compile: g++ -std=c++17 -O2 -Wall -Wextra lambda_demo.cpp -o lambda_demo
// Run:     ./lambda_demo

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include <string>
#include <thread>
#include <cstdio>

// ---------- 1. Captureless lambda -> function pointer ----------
static int (*g_fn)(int) = [](int x) { return x * 2; };

// ---------- 2. Template taking a lambda: zero overhead ----------
template <typename F>
void forEach(std::vector<int>& v, F f) 
{
    for (auto& x : v) f(x);
}

// ---------- 3. A struct that returns a lambda ----------
struct Foo 
{
    int v = 7;
    auto byRef()  { return [this] { return v; }; }   // dangles if Foo dies
    auto byCopy() { return [*this] { return v; }; }  // C++17 — safe
};

int main() 
{
    std::cout << "=== 1. Captureless lambda as function pointer ===\n";
    std::cout << "g_fn(21) = " << g_fn(21) << "\n\n";

    std::cout << "=== 2. Value vs reference capture ===\n";
    int x = 5;
    auto byVal = [x] { return x; };
    auto byRef = [&x] { return x; };
    x = 100;
    std::cout << "byVal() = " << byVal() << "  (snapshot at creation)\n";
    std::cout << "byRef() = " << byRef() << "  (live reference)\n\n";

    std::cout << "=== 3. mutable: closure owns a mutable copy ===\n";
    int counter = 0;
    auto inc = [counter]() mutable { return ++counter; };
    std::cout << "inc() = " << inc() << ", inc() = " << inc()
              << ", original counter = " << counter << "\n\n";

    std::cout << "=== 4. Init-capture: move a unique_ptr into a lambda ===\n";
    auto p = std::make_unique<int>(42);
    auto owns = [q = std::move(p)] { return *q; };
    std::cout << "owns() = " << owns() << ", p is "
              << (p ? "non-null" : "null") << "\n\n";

    std::cout << "=== 5. this vs *this capture ===\n";
    Foo foo;
    auto r = foo.byRef();
    auto c = foo.byCopy();
    foo.v = 999;
    std::cout << "byRef()  = " << r() << "  (sees mutation)\n";
    std::cout << "byCopy() = " << c() << "  (snapshot)\n\n";

    std::cout << "=== 6. Generic lambda ===\n";
    auto show = [](auto const& v) { std::cout << v << ' '; };
    show(1); show(2.5); show(std::string("hi")); std::cout << "\n\n";

    std::cout << "=== 7. Zero-overhead template vs std::function ===\n";
    std::vector<int> v{1, 2, 3, 4};
    forEach(v, [](int& n) { n *= 10; });                  // inlined
    std::function<void(int&)> erased = [](int& n) { n += 1; };  // indirect
    forEach(v, erased);
    for (int n : v) std::cout << n << ' ';
    std::cout << "\n\n";

    std::cout << "=== 8. Recursive lambda (self-passing, zero overhead) ===\n";
    auto fib = [](auto self, int n) -> long long {
        return n < 2 ? n : self(self, n - 1) + self(self, n - 2);
    };
    std::cout << "fib(20) = " << fib(fib, 20) << "\n\n";

    std::cout << "=== 9. constexpr lambda ===\n";
    constexpr auto sq = [](int n) constexpr { return n * n; };
    static_assert(sq(9) == 81, "compile-time check");
    std::cout << "sq(9) = " << sq(9) << " (checked at compile time)\n\n";

    std::cout << "=== 10. Lambda + thread with shared_ptr lifetime ===\n";
    auto shared = std::make_shared<int>(77);
    std::thread t([shared] { std::printf("thread sees %d\n", *shared); });
    t.join();

    return 0;
}

/*

chatGpt


C++ Lambda Function – Complete Interview Explanation (5 Years Experience)
For a 5-year experienced C++ Linux programmer, you should be able to explain lambda functions beyond just syntax. Interviewers may ask about capture mechanisms, memory behaviour, lifetime issues, mutable, generic lambdas, STL algorithms, callbacks, threading and practical Linux application scenarios.

Let's cover it from basic to advanced, with runnable C++ examples.
1. What is a Lambda Function in C++?
Interview answer
A lambda function is an anonymous function (a function without a name) that we can define directly inside another function or wherever an expression is allowed.

It was introduced in C++11.

We use lambda functions when we need small, temporary functionality without creating a separate named function or function object.

Lambdas are commonly used with:
- STL algorithms such as sort(), find_if(), for_each().
- Callbacks.
- Thread functions.
- Event handling.
- Custom comparison logic.
- Filtering and searching data.
Normal function vs Lambda function
Normal function:
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    cout << add(10, 20) << endl;

    return 0;
}

Lambda function:
#include <iostream>
using namespace std;

int main()
{
    auto add = [](int a, int b)
    {
        return a + b;
    };

    cout << add(10, 20) << endl;

    return 0;
}

Output:
30

Here:
auto add = [](int a, int b)
{
    return a + b;
};

- auto – compiler automatically determines the lambda's type.
- add – variable holding the lambda object.
- [] – capture list.
- (int a, int b) – parameters.
- {} – lambda body.
- return – returns the result.
Important interview point: A lambda is not simply a function stored in a variable. A lambda expression creates an unnamed closure object whose type is generated by the compiler.
2. Lambda Function Syntax
General syntax:
[capture_list](parameters) mutable -> return_type
{
    // Function body
};

Let's understand every component.
Component	Purpose
[]	Capture variables from the surrounding scope
(parameters)	Input arguments
mutable	Allows modification of value-captured variables inside the lambda
-> return_type	Explicit return type
{}	Lambda implementation


Example:
int x = 10;

auto lambda = [x](int y) mutable -> int
{
    x = x + y;

    return x;
};

Here:
- x is captured by value.
- y is a lambda parameter.
- mutable allows modification of the lambda's captured copy of x.
- Return type is explicitly int.
3. What Is a Capture List?
This is one of the most important lambda interview topics.

A capture list specifies which variables from the surrounding scope are accessible inside the lambda.

There are two main ways to capture variables:
1. Capture by value.
2. Capture by reference.
A. Capture by Value [x]
The lambda gets its own copy of the variable.
#include <iostream>
using namespace std;

int main()
{
    int x = 10;

    auto lambda = [x]()
    {
        cout << x << endl;
    };

    lambda();

    return 0;
}

Output:
10

Memory concept:
Original variable                  Lambda object

    x = 10        ------------>       x = 10
                                       (copy)

Changing the original variable after creating the lambda does not change the captured copy.
int x = 10;

auto lambda = [x]()
{
    cout << x << endl;
};

x = 50;

lambda();

Output:
10

Interview answer: Capture by value stores a copy of the variable inside the lambda's closure object.
B. Capture by Reference [&x]
The lambda accesses the original variable instead of making a copy.
#include <iostream>
using namespace std;

int main()
{
    int x = 10;

    auto lambda = [&x]()
    {
        x = 50;
    };

    lambda();

    cout << x << endl;

    return 0;
}

Output:
50

Memory concept:
Original variable

    x = 10
       |
       |
       v
  Lambda object
  (references x)
       |
       v
  Modifies original x

Interview answer: Capture by reference allows the lambda to access and modify the original variable. We must ensure that the referenced variable remains alive whenever the lambda is executed.
Capture by Value vs Reference
Feature	Capture by Value [x]	Capture by Reference [&x]
Copy created	Yes	No copy of the variable
Original modified	No, normally	Yes, if modified
Memory	Stores captured value	Stores a reference-like capture
Lifetime dependency	Does not depend on original variable's lifetime for the captured value	Original object must remain alive
Use case	Independent data	Modifying shared data


4. Different Types of Capture Lists
You should remember these for interviews.
Syntax	Meaning
[]	Capture nothing
[x]	Capture x by value
[&x]	Capture x by reference
[=]	Default capture of used local variables by value
[&]	Default capture of used local variables by reference
[=, &x]	Capture everything else by value, but x by reference
[&, x]	Capture everything else by reference, but x by value


Example 1: Capture everything by value
int a = 10;
int b = 20;

auto lambda = [=]()
{
    cout << a + b << endl;
};

Output:
30

Example 2: Capture everything by reference
int a = 10;
int b = 20;

auto lambda = [&]()
{
    a = 100;
    b = 200;
};

lambda();

cout << a << " " << b << endl;

Output:
100 200

Example 3: Mixed capture
int a = 10;
int b = 20;

auto lambda = [a, &b]()
{
    cout << a << endl;

    b = 100;
};

lambda();

cout << b << endl;

Output:
10
100

Here:
- a is copied.
- b is accessed by reference.
Interview tip: Prefer explicit captures such as [this, &result] or [value] in production code when they make ownership and lifetime clearer. Broad captures like [&] can hide lifetime dependencies.
5. What Is mutable in Lambda?
This is a frequently asked interview question.

By default, a lambda that captures variables by value cannot modify its captured copies.

The mutable keyword allows modification of those copies.
Without mutable
#include <iostream>
using namespace std;

int main()
{
    int x = 10;

    auto lambda = [x]()
    {
        // x = 20; // Compilation error
        cout << x << endl;
    };

    lambda();

    return 0;
}

With mutable
#include <iostream>
using namespace std;

int main()
{
    int x = 10;

    auto lambda = [x]() mutable
    {
        x = 20;

        cout << x << endl;
    };

    lambda();

    cout << x << endl;

    return 0;
}

Output:
20
10

Why?
Original x = 10

Lambda's captured copy = 10

Inside lambda:
Captured copy = 20

Original x = 10 (unchanged)

Interview answer: mutable removes the default const qualification of the lambda's function call operator, allowing modification of value-captured copies. It does not change the original variable.
6. Lambda with Return Type
The compiler can usually automatically deduce the return type.
auto add = [](int a, int b)
{
    return a + b;
};

Explicit return type:
auto divide = [](double a, double b) -> double
{
    return a / b;
};

When should we specify the return type?
- When the return type is not obvious.
- When different return expressions need a common type.
- When returning a type that requires explicit control.
Example:
auto check = [](int value) -> string
{
    if (value > 0)
    {
        return "Positive";
    }

    return "Non-positive";
};

7. Lambda with STL Algorithms
This is one of the most practical uses of lambdas in C++ development.
Example 1: Lambda with sort()
Suppose we want to sort numbers in descending order.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> numbers = {10, 50, 20, 40, 30};

    sort(numbers.begin(), numbers.end(),
         [](int a, int b)
         {
             return a > b;
         });

    for (int value : numbers)
    {
        cout << value << " ";
    }

    return 0;
}

Output:
50 40 30 20 10

Explanation:
[](int a, int b)
{
    return a > b;
}

The lambda acts as a custom comparator.

If a > b is true, a should come before b.

Interview question: Why use a lambda instead of a separate comparator function?

Answer: When comparison logic is small and only needed in one place, a lambda keeps the logic close to the algorithm call and avoids creating a separate named function.
Example 2: Lambda with find_if()
Find the first even number.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> numbers = {11, 15, 21, 24, 30, 35};

    auto it = find_if(numbers.begin(), numbers.end(),
                      [](int value)
                      {
                          return value % 2 == 0;
                      });

    if (it != numbers.end())
    {
        cout << "Found: " << *it << endl;
    }
    else
    {
        cout << "Not found" << endl;
    }

    return 0;
}

Output:
Found: 24

Example 3: Lambda with for_each()
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> numbers = {10, 20, 30, 40};

    for_each(numbers.begin(), numbers.end(),
             [](int value)
             {
                 cout << value << endl;
             });

    return 0;
}

Example 4: Lambda with count_if()
Count even numbers.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> numbers = {10, 15, 20, 25, 30, 35};

    int count = count_if(numbers.begin(), numbers.end(),
                         [](int value)
                         {
                             return value % 2 == 0;
                         });

    cout << "Even count: " << count << endl;

    return 0;
}

Output:
Even count: 3

8. What Is a Generic Lambda?
Generic lambdas were introduced in C++14.

A generic lambda uses auto as a parameter type. The compiler generates the appropriate call operator for the argument types.

Example:
#include <iostream>
using namespace std;

int main()
{
    auto add = [](auto a, auto b)
    {
        return a + b;
    };

    cout << add(10, 20) << endl;

    cout << add(10.5, 20.5) << endl;

    cout << add(string("Hello "), string("World")) << endl;

    return 0;
}

Output:
30
31
Hello World

Why is it useful?

The same lambda can work with multiple compatible data types.
Interview answer
A generic lambda uses auto parameters and provides a templated call operator. It supports reusable functionality without explicitly writing a function template.
9. Lambda with std::function
std::function is a general-purpose polymorphic function wrapper introduced in C++11.

It can store callable objects, including:
- Normal functions.
- Lambda expressions.
- Function objects.
- Bind expressions.
Example:
#include <iostream>
#include <functional>

using namespace std;

int main()
{
    function<int(int, int)> operation =
        [](int a, int b)
        {
            return a + b;
        };

    cout << operation(10, 20) << endl;

    return 0;
}

Output:
30

auto vs std::function
Feature	auto	std::function
Type	Compiler-deduced concrete type	Type-erased callable wrapper
Stores lambda	Yes	Yes
Reassignment	Generally only compatible same closure type	Can store different compatible callable types
Runtime overhead	Usually minimal	May involve type-erasure overhead
Empty state	No default empty lambda object for a normal closure variable	Can be empty
Typical usage	Local lambda	Callbacks and type-erased interfaces


5-year experience tip: Prefer auto for local lambdas unless you specifically need type erasure or a common callable type. std::function can introduce overhead and possibly dynamic allocation.
10. Lambda with std::thread
This is particularly useful for C++ Linux programmers.

We can pass a lambda directly to a thread.
#include <iostream>
#include <thread>

using namespace std;

int main()
{
    thread t([]()
    {
        cout << "Thread is running" << endl;
    });

    t.join();

    return 0;
}

Output:
Thread is running

Passing variables to a thread using lambda
#include <iostream>
#include <thread>

using namespace std;

int main()
{
    int value = 100;

    thread t([value]()
    {
        cout << "Thread value: " << value << endl;
    });

    t.join();

    return 0;
}

Output:
Thread value: 100

Important: Reference capture and thread lifetime
Consider:
int value = 100;

thread t([&value]()
{
    cout << value << endl;
});

t.join();

This is safe because join() ensures the thread finishes before value goes out of scope.

But this can cause a dangling reference:
thread createThread()
{
    int value = 100;

    return thread([&value]()
    {
        cout << value << endl;
    });
}

Here value is destroyed when createThread() returns, but the thread may still try to access it.

This is undefined behaviour.

Interview answer: When using lambdas with threads, we must carefully manage captured variable lifetimes. Capturing by value is often safer for data that the thread needs independently.

Also, join() waits for thread completion; it does not automatically make concurrent access to shared data race-free. Use mutexes or atomics where required.
11. What Is a Closure Object?
This is an important advanced interview topic.

When the compiler encounters a lambda expression, it generates an unnamed class-like type called a closure type.

The object created from that type is called a closure object.

Example:
int x = 10;

auto lambda = [x]()
{
    return x + 10;
};

Conceptually, the compiler generates something similar to:
class GeneratedLambda
{
private:
    int captured_x;

public:
    GeneratedLambda(int value)
    {
        captured_x = value;
    }

    int operator()() const
    {
        return captured_x + 10;
    }
};

Then:
GeneratedLambda lambda(10);

cout << lambda() << endl;

Output:
20

Important: This is a conceptual illustration, not the exact implementation or ABI generated by every compiler.
What is operator()?
A lambda is callable because its closure type has a function call operator.

For example:
auto lambda = [](int a, int b)
{
    return a + b;
};

cout << lambda(10, 20);

The expression:
lambda(10, 20);

internally invokes the closure object's operator().
12. Lambda with this Pointer
This is important in C++ class development.

Consider:
#include <iostream>
using namespace std;

class Employee
{
private:
    int salary;

public:

    Employee()
    {
        salary = 50000;
    }

    void display()
    {
        auto lambda = [this]()
        {
            cout << salary << endl;
        };

        lambda();
    }
};

int main()
{
    Employee emp;

    emp.display();

    return 0;
}

Output:
50000

[this] captures the this pointer, allowing the lambda to access the current object's members.
Difference between [this] and [*this]
Capture	Meaning
[this]	Captures the current object's pointer
[*this]	Captures a copy of the current object


[*this] was introduced in C++17.

Be careful: capturing this does not extend the lifetime of the object. If the lambda executes after the object is destroyed, accessing its members can cause undefined behaviour.
13. Lambda with Init Capture (C++14)
C++14 introduced init capture, also called generalized lambda capture.

It allows us to create and initialize a new variable inside the lambda capture list.

Example:
#include <iostream>
using namespace std;

int main()
{
    int value = 100;

    auto lambda = [newValue = value + 50]()
    {
        cout << newValue << endl;
    };

    lambda();

    return 0;
}

Output:
150

Here:
[newValue = value + 50]

creates a new captured variable called newValue.
Move capture
Very useful when working with smart pointers.
#include <iostream>
#include <memory>
using namespace std;

int main()
{
    unique_ptr<int> ptr = make_unique<int>(100);

    auto lambda = [p = move(ptr)]()
    {
        cout << *p << endl;
    };

    lambda();

    return 0;
}

Output:
100

Here:
- unique_ptr cannot be copied.
- We transfer ownership into the lambda using std::move.
- The lambda owns the pointer.
This is useful for asynchronous operations and callbacks where a lambda needs to own resources.
14. Lambda with Recursive Function
A lambda can call itself recursively, but it needs a way to refer to itself.

C++14 generic-lambda approach:
#include <iostream>
using namespace std;

int main()
{
    auto factorial = [](auto self, int n) -> int
    {
        if (n <= 1)
        {
            return 1;
        }

        return n * self(self, n - 1);
    };

    cout << factorial(factorial, 5) << endl;

    return 0;
}

Output:
120

This technique passes the lambda itself as an argument.

C++23 also supports explicit object parameters, which provide another way to write recursive lambdas.
15. Lambda with std::sort() – Real Interview Example
Suppose we have employee records and want to sort employees by salary.

This is a realistic application of lambda functions.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Employee
{
    int id;
    string name;
    int salary;
};

int main()
{
    vector<Employee> employees =
    {
        {101, "Amit", 50000},
        {102, "Rahul", 30000},
        {103, "Vijay", 70000},
        {104, "Raj", 40000}
    };

    sort(employees.begin(), employees.end(),
         [](const Employee& e1, const Employee& e2)
         {
             return e1.salary < e2.salary;
         });

    for (const Employee& emp : employees)
    {
        cout << emp.id << " "
             << emp.name << " "
             << emp.salary << endl;
    }

    return 0;
}

Output:
102 Rahul 30000
104 Raj 40000
101 Amit 50000
103 Vijay 70000

Why use const Employee&?
- Avoids copying Employee objects.
- Prevents accidental modification.
- Improves efficiency for larger objects.
Interview explanation: We use a lambda as a custom comparator to sort employee objects based on salary. Passing parameters by const reference avoids unnecessary copies.
16. Lambda Function Versions (C++11 to C++23)
For a 5-year experienced C++ programmer, you should know the important version changes.
C++ Version	Lambda feature
C++11	Basic lambda, capture by value/reference, mutable, return type
C++14	Generic lambdas (auto parameters), init capture
C++17	constexpr lambdas, [*this] capture
C++20	Template parameter lists, concepts support, improved constant evaluation
C++23	Explicit object parameter for lambdas


Example of a C++20 template lambda:
auto add = []<typename T>(T a, T b)
{
    return a + b;
};

Example:
cout << add(10, 20) << endl;
cout << add(10.5, 20.5) << endl;

17. Important Lambda Interview Questions and Answers
Interview question	Answer
What is a lambda?	An anonymous function expression that creates a closure object.
When was lambda introduced?	C++11.
What is a capture list?	It specifies which surrounding local variables the lambda captures.
What is [=]?	Default capture of used local variables by value.
What is [&]?	Default capture of used local variables by reference.
What is mutable?	Allows modification of value-captured copies.
What is a closure object?	An object of the compiler-generated closure type.
What is operator()?	The call operator that makes the closure object callable.
Can lambda return a value?	Yes.
Can lambda be recursive?	Yes, with an appropriate self-reference technique.
Can lambda be passed to a thread?	Yes.
Can lambda capture this?	Yes, using [this].
What is a generic lambda?	A lambda with auto parameters or a template parameter list.
What is init capture?	Creating a new variable in the capture list.
What is move capture?	Transferring an object's resources into the closure using std::move.
Can we store lambda in std::function?	Yes.
Is lambda always faster than a normal function?	No. It often enables inlining, but performance depends on implementation and usage.
Can we use lambda without a capture list?	Yes, use [].
Can a lambda access static variables without capturing them?	Yes. Static and global variables do not need capture.
Can we modify a reference-captured variable without mutable?	Yes.


18. Advanced Questions for 5-Year Experience
These are the questions that help distinguish an experienced developer from someone who only knows lambda syntax.
Q1. What happens if we return a lambda that captures a local variable by reference?
Example:
auto createLambda()
{
    int x = 100;

    return [&x]()
    {
        return x;
    };
}

Answer: This creates a dangling reference. The local variable x is destroyed when createLambda() returns. Calling the returned lambda results in undefined behaviour.

Correct approach:
auto createLambda()
{
    int x = 100;

    return [x]()
    {
        return x;
    };
}

Now the lambda owns a copy of x.
Q2. Is lambda stored on stack or heap?
Answer: The closure object follows the storage duration and allocation mechanism of the variable holding it.

For example:
auto lambda = []()
{
    cout << "Hello";
};

If lambda is a local automatic variable, it normally has automatic storage duration. It is not automatically allocated on the heap.

However, a lambda can be dynamically allocated or stored inside a dynamically allocated object.
Q3. Is lambda faster than a normal function?
Answer: Not necessarily. A lambda can be efficient because its concrete closure type is known to the compiler, which often allows inlining and optimization. But actual performance depends on the code, compiler optimizations and whether type erasure or indirect calls are involved.
Q4. Can we overload a lambda?
Answer: A single ordinary lambda expression has one closure type and its call operator can have multiple overloads only if the callable is designed accordingly. Generic lambdas generate templated call operators. You can also create a class with overloaded operator() or combine multiple lambdas into an overloaded visitor.
Q5. Can two lambda expressions have the same type?
Answer: Each lambda expression has a unique closure type, even if two lambda expressions have identical source code.

Example:
auto lambda1 = []()
{
    cout << "Hello";
};

auto lambda2 = []()
{
    cout << "Hello";
};

lambda1 and lambda2 have different types.
Q6. What is the difference between lambda and function pointer?
Lambda	Function Pointer
Can capture variables	Cannot capture local variables
Has a closure type	Stores a function address
Can maintain captured state	Cannot maintain its own captured state
Can be passed to STL algorithms	Can be passed to compatible callable interfaces
Capturing lambda cannot directly convert to a function pointer	Is already a function pointer


A non-capturing lambda can convert to a compatible function pointer.

Example:
int (*ptr)(int, int) = [](int a, int b)
{
    return a + b;
};

cout << ptr(10, 20) << endl;

Output:
30

19. Practical Use Cases in C++ Linux Development
For your C++ Linux programming interviews, connect lambda knowledge with real development scenarios.
Real-world scenario	Lambda usage
Sorting log records	Custom comparator using sort()
Filtering log entries	remove_if() or find_if()
Processing file metadata	for_each()
Thread execution	Passing lambda to std::thread
Asynchronous tasks	std::async
Callback handling	Passing lambda as a callable
Resource ownership	Move capture with unique_ptr
Event handling	Capturing required object state
Searching process information	Predicate-based filtering


Example: Filtering Linux process information
Imagine you have process information collected from /proc and want to filter processes consuming more than 100 MB of memory.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process
{
    int pid;
    string name;
    int memoryMB;
};

int main()
{
    vector<Process> processes =
    {
        {101, "firefox", 250},
        {102, "sshd", 20},
        {103, "scanner", 150},
        {104, "bash", 15}
    };

    for_each(processes.begin(), processes.end(),
             [](const Process& process)
             {
                 if (process.memoryMB > 100)
                 {
                     cout << "PID: " << process.pid
                          << " Name: " << process.name
                          << " Memory: " << process.memoryMB
                          << " MB" << endl;
                 }
             });

    return 0;
}

Output:
PID: 101 Name: firefox Memory: 250 MB
PID: 103 Name: scanner Memory: 150 MB

This demonstrates how lambdas can simplify filtering and processing system-related data.
20. How to Explain Lambda in an Interview (2-Minute Answer)
If the interviewer asks, "Explain lambda functions in C++", you can say:
A lambda function in C++ is an anonymous function that was introduced in C++11.

It allows us to define small functions directly where they are needed, without creating a separate named function.

The general syntax consists of a capture list, parameter list, optional mutable keyword, optional return type and function body.

The most important feature is the capture list. We can capture local variables by value or by reference.

When we capture by value, the lambda stores its own copy of the variable. When we capture by reference, it accesses the original variable.

We can use mutable when we want to modify value-captured variables inside the lambda.

In practical C++ development, lambdas are commonly used with STL algorithms such as sort, find_if, for_each and count_if. They are also useful for callbacks, multithreading and asynchronous operations.

Internally, the compiler generates a unique closure type for every lambda expression. The closure object has an operator() that makes it callable.

C++14 introduced generic lambdas and init capture. C++17 introduced features such as constexpr lambdas and capturing *this by value. C++20 added template parameter lists for lambdas, and C++23 introduced explicit object parameters.

While using lambdas in multithreaded or asynchronous code, we must be careful about captured variable lifetimes, especially when capturing by reference.

I generally prefer lambdas for small, local operations because they keep the logic close to where it is used and improve code readability.



21. What You Should Practise Before Your Interview
Try writing these programs without looking at the solution.
No.	Coding problem	Concept
1	Add two numbers using lambda	Basic lambda
2	Swap two variables using reference capture	[&]
3	Sort vector ascending and descending	sort()
4	Sort employees by salary	Custom comparator
5	Find even numbers	find_if()
6	Count positive numbers	count_if()
7	Remove negative numbers	remove_if()
8	Calculate factorial using recursive lambda	Recursion
9	Run lambda using std::thread	Multithreading
10	Capture unique_ptr using move capture	C++14
11	Use generic lambda for multiple data types	C++14
12	Return lambda from a function safely	Lifetime management


My recommendation for a 5-year C++ Linux interview
Give extra attention to these five areas:
1. Capture by value vs reference – including dangling references.
2. Closure object and operator() – internal compiler implementation.
3. Lambda with STL algorithms – especially custom comparators and predicates.
4. Lambda with threads – lifetime, data races, mutex and ownership.
5. Generic lambda and move capture – modern C++14 and later.
These topics demonstrate practical C++ knowledge rather than just theoretical syntax.

Next step: I can conduct a C++ lambda mock interview at a 5-year experience level, asking one question at a time, checking your answer and explaining what an interviewer expects.

*/

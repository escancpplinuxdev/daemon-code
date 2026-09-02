// ============================================================
//  FILE  : Set_Explanation.cpp
//  TOPIC : std::set — Complete Reference
//  LEVEL : Intermediate to Advanced (C++ Linux Programmer)
//  STD   : C++11 and later
// ============================================================

#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
using namespace std;

// ============================================================
// SECTION 1 : WHAT IS std::set ?
// ============================================================
//
//  std::set<T> is an ORDERED ASSOCIATIVE CONTAINER that:
//
//   - Stores UNIQUE elements (no duplicates allowed)
//   - Keeps elements ALWAYS SORTED (ascending, by default)
//   - Provides GUARANTEED O(log n) WORST CASE for insert/erase/find
//     (NOT average case — this is the key contrast with unordered_set,
//      whose O(1) is only an AVERAGE, degrading to O(n) on bad hashing)
//   - Is implemented internally as a self-balancing BINARY SEARCH TREE
//     (a red-black tree in every major implementation — GCC libstdc++,
//      Clang libc++, MSVC STL all use one)
//
//  Header required:
//   #include <set>
//
//  Full template signature:
//   template
//       class Key,
//       class Compare   = std::less<Key>,
//       class Allocator = std::allocator<Key>
//   > class set;
//
//  Parameters:
//   Key       — type of stored elements
//   Compare   — strict-weak-ordering comparator (default: ascending via std::less<Key>)
//   Allocator — memory allocator (default: std::allocator<Key>)
//
//  CRITICAL DIFFERENCE FROM unordered_set:
//   There is NO Hash parameter and NO KeyEqual parameter here.
//   The single Compare does BOTH jobs at once: two elements a, b are
//   considered EQUIVALENT (duplicates) if BOTH of these are true:
//       !Compare(a, b)  AND  !Compare(b, a)
//   i.e. neither is "less than" the other — this is how set decides
//   "same key" WITHOUT ever needing an operator== or a hash function.

// ============================================================
// SECTION 2 : INTERNAL IMPLEMENTATION (RED-BLACK TREE)
// ============================================================
//
//  STRUCTURE:
//   Each node holds: key, left child ptr, right child ptr, parent ptr,
//   and one COLOR bit (red or black). No bucket array — unlike
//   unordered_set, there is nothing here resembling a hash table.
//
//  VISUAL LAYOUT (small example, keys 10,20,30,40,50,60,70 inserted):
//
//                    (40,B)
//                   /       \
//               (20,B)      (60,B)
//               /    \       /    \
//           (10,R) (30,R) (50,R) (70,R)
//
//  RED-BLACK PROPERTIES (what keeps the tree balanced):
//   1. Every node is either RED or BLACK
//   2. The root is always BLACK
//   3. Every leaf (nullptr) is considered BLACK
//   4. A RED node cannot have a RED child (no two reds in a row)
//   5. Every path from a node to its descendant leaves has the
//      SAME NUMBER of black nodes ("black-height")
//
//  These 5 properties together GUARANTEE the tree height never
//  exceeds 2*log2(n+1) — this is the mathematical source of set's
//  WORST-CASE O(log n) guarantee, something a hash table can never
//  promise (a hash table's worst case is always O(n), just made
//  statistically unlikely by a good hash function).
//
//  WHY IN-ORDER TRAVERSAL GIVES SORTED OUTPUT:
//   A binary SEARCH tree, by definition, keeps every node's left
//   subtree entirely "less than" it and right subtree entirely
//   "greater than" it. Walking left-root-right (in-order) therefore
//   visits every node in ascending order automatically — THIS is
//   the mechanical reason set::begin()...set::end() always iterates
//   in sorted order, with zero extra bookkeeping required.
//
//  REBALANCING ON INSERT/ERASE:
//   Insert/erase can violate properties 4 or 5 above. The tree fixes
//   this via ROTATIONS (left-rotate, right-rotate) and RECOLORING —
//   both O(1) local operations, and AT MOST O(log n) of them are ever
//   needed per insert/erase (bounded by the tree's height). This is
//   why insert/erase stay O(log n) even though they must maintain
//   balance, not just find a spot and stop.
//
//  TIME COMPLEXITIES (ALL WORST CASE, NOT AVERAGE):
//   Operation         Complexity
//   ---------         ----------
//   insert            O(log n)
//   erase             O(log n)
//   find/count        O(log n)
//   lower_bound       O(log n)
//   upper_bound       O(log n)
//   clear             O(n)
//   begin/end         O(1)   (leftmost/one-past-rightmost cached)
//   size              O(1)
//
//  SPACE COMPLEXITY:
//   O(n). Per-node overhead is 3 pointers (left, right, parent) +
//   1 color bit (usually packed into a pointer's spare bit or padding)
//   — typically LARGER per-element overhead than unordered_set's
//   single "next" pointer per node, but set has NO separate bucket
//   array to also account for.

// ============================================================
// SECTION 3 : DECLARATION AND INITIALIZATION
// ============================================================

void section3_declaration()
{
    // --- 3.1 Default constructor — ascending order via std::less ---
    set<int>    s1;
    set<string> s2;

    // --- 3.2 Initializer list — automatically SORTED on construction ---
    set<int> s3 = {50, 10, 40, 20, 30};
    // s3 now holds {10, 20, 30, 40, 50} — insertion order is IRRELEVANT,
    // the tree sorts on every insert, so the final order is always ascending.

    // --- 3.3 Descending order — pass std::greater<int> as Compare ---
    set<int, greater<int>> s4 = {50, 10, 40, 20, 30};
    // s4 iterates as {50, 40, 30, 20, 10}

    // --- 3.4 Copy constructor ---
    set<int> s5 = s3;

    // --- 3.5 Move constructor ---
    set<int> s6 = move(s5);

    // --- 3.6 Range constructor (from any iterable, duplicates removed) ---
    vector<int> v = {5, 10, 15, 10, 5};
    set<int> s7(v.begin(), v.end());        // s7 = {5, 10, 15}

    // --- 3.7 Assignment ---
    s1 = s3;               // copy assignment
    s2 = {"delta", "alpha", "charlie", "bravo"};   // init-list assignment, auto-sorted

    cout << "s7 size: " << s7.size() << "\n";      // 3 (duplicates removed)
    for (int x : s3)
    {
        cout << x << " ";      // 10 20 30 40 50 — always sorted, regardless of insert order
    }
    cout << "\n";
}

// ============================================================
// SECTION 4 : INSERTING ELEMENTS
// ============================================================

void section4_insert()
{
    set<int> s;

    // --- 4.1 insert(value) ---
    // Returns: pair<iterator, bool>
    //   iterator → points to inserted element (or the existing equivalent one)
    //   bool     → true if inserted, false if an equivalent element already existed
    auto [it1, ok1] = s.insert(30);   // ok1 = true
    auto [it2, ok2] = s.insert(30);   // ok2 = false — already present
    cout << *it1 << " " << ok1 << "\n";   // 30 1
    cout << *it2 << " " << ok2 << "\n";   // 30 0

    // --- 4.2 insert with HINT — this genuinely matters for set, unlike unordered_set ---
    // If the hint points to the CORRECT neighboring position, insertion drops
    // from O(log n) to AMORTIZED O(1) — the tree can skip the top-down search.
    // Standard trick: when inserting values in ASCENDING order, always hint
    // with end() — this turns an O(n log n) bulk-insert into O(n) overall.
    set<int> ascending;
    for (int i = 0; i < 1000; ++i)
    {
        ascending.insert(ascending.end(), i);   // O(1) amortized per insert — correct hint!
    }
    // Compare: ascending.insert(i) WITHOUT the hint would cost O(log n) each,
    // i.e. O(n log n) total for the same bulk workload.

    // --- 4.3 insert range ---
    vector<int> v = {70, 80, 90, 80};   // 80 is a duplicate
    s.insert(v.begin(), v.end());        // inserts 70, 80, 90 (80 once)

    // --- 4.4 insert initializer list ---
    s.insert({100, 110, 120, 100});      // 100 inserted once

    // --- 4.5 emplace(args...) ---
    // Constructs the element IN-PLACE — avoids a temporary object.
    // For int this is equivalent to insert, but for structs it matters.
    auto [it3, ok3] = s.emplace(130);

    // --- 4.6 emplace_hint ---
    s.emplace_hint(s.end(), 140);        // same amortized-O(1) benefit as insert-with-hint

    cout << "Size after inserts: " << s.size() << "\n";

    // ITERATOR INVALIDATION — much friendlier than unordered_set here:
    // insert() NEVER invalidates any existing iterator or reference
    // (except, obviously, an iterator to an element that already equals
    // the newly-inserted one, which simply isn't touched anyway).
    // This is a genuinely stronger guarantee than unordered_set, which
    // invalidates EVERYTHING on a rehash.
}

// ============================================================
// SECTION 5 : ACCESSING / LOOKING UP ELEMENTS
// ============================================================

void section5_lookup()
{
    set<int> s = {10, 20, 30, 40, 50};

    // --- 5.1 find(key) — MEMBER function, O(log n), tree-aware ---
    auto it = s.find(30);
    if (it != s.end())
    {
        cout << "Found: " << *it << "\n";   // Found: 30
    }

    // CRITICAL TRAP: std::find() (the FREE function from <algorithm>) does
    // NOT know the range is sorted — it does a LINEAR O(n) scan instead.
    // Both compile, both give the same ANSWER, but wildly different speed.
    auto it_slow = find(s.begin(), s.end(), 30);   // O(n) — avoid this on a set!
    auto it_fast = s.find(30);                       // O(log n) — always prefer this

    // --- 5.2 count(key) ---
    // Returns 0 or 1 (set has no duplicates) — O(log n)
    cout << s.count(20) << "\n";   // 1
    cout << s.count(99) << "\n";   // 0

    // --- 5.3 contains(key) [C++20] ---
#if __cplusplus >= 202002L
    if (s.contains(50))
    {
        cout << "50 exists (C++20)\n";
    }
#else
	cout << "c++ lower version\n";
#endif

    // --- 5.4 lower_bound(key) — REAL range-query power, unlike unordered_set ---
    // Returns iterator to first element >= key
    auto lb = s.lower_bound(25);
    cout << "lower_bound(25): " << *lb << "\n";   // 30 (first elem >= 25)

    // --- 5.5 upper_bound(key) ---
    // Returns iterator to first element > key
    auto ub = s.upper_bound(30);
    cout << "upper_bound(30): " << *ub << "\n";   // 40 (first elem > 30)

    // --- 5.6 equal_range(key) ---
    // Returns pair<iterator,iterator> = {lower_bound, upper_bound}
    // For set: spans at most 1 element (no duplicates)
    auto [first, last] = s.equal_range(30);
    for (auto rit = first; rit != last; ++rit)
    {
        cout << "equal_range: " << *rit << "\n";   // equal_range: 30
    }

    // --- 5.7 RANGE QUERY IDIOM — the real reason to prefer set over unordered_set ---
    // "Give me every element in [20, 40]" — impossible in O(log n + k) on
    // unordered_set (which has no ordering to exploit); trivial on set:
    cout << "Elements in [20,40]: ";
    for (auto rit = s.lower_bound(20); rit != s.upper_bound(40); ++rit)
    {
        cout << *rit << " ";   // 20 30 40
    }
    cout << "\n";

    // --- 5.8 Finding predecessor / successor of a value NOT in the set ---
    set<int> s2 = {10, 20, 30, 40, 50};
    int target = 25;

    auto succ = s2.lower_bound(target);   // first elem >= 25 -> points to 30
    auto pred = succ;
    if (pred != s2.begin())
    {
        --pred;                            // step back -> points to 20
        cout << "Predecessor of 25: " << *pred << "\n";   // 20
    }
    if (succ != s2.end())
    {
        cout << "Successor of 25: " << *succ << "\n";     // 30
    }
    // This predecessor/successor pattern is a very common interview
    // building block — "find closest value" problems reduce to exactly this.
}

// ============================================================
// SECTION 6 : ERASING ELEMENTS
// ============================================================

void section6_erase()
{
    set<int> s = {10, 20, 30, 40, 50};

    // --- 6.1 erase by VALUE ---
    // Returns: number of elements removed (0 or 1 for set)
    size_t removed = s.erase(30);
    cout << "Removed: " << removed << "\n";   // 1

    // --- 6.2 erase by ITERATOR ---
    // Returns: iterator to element AFTER the erased one
    // GUARANTEE: erase() invalidates ONLY the iterator to the erased
    // element itself — every OTHER iterator/reference into the set
    // stays valid. This is much stronger than vector's erase.
    auto it = s.find(20);
    if (it != s.end())
    {
        auto next_it = s.erase(it);   // erases 20, returns next-in-order iterator
    }

    // --- 6.3 erase RANGE ---
    set<int> s2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto rlo = s2.lower_bound(3);
    auto rhi = s2.upper_bound(7);
    s2.erase(rlo, rhi);   // removes 3,4,5,6,7 — a genuine RANGE erase, sorted-order aware
    for (int x : s2)
    {
        cout << x << " ";   // 1 2 8 9 10
    }
    cout << "\n";

    // --- 6.4 clear() ---
    s.clear();
    cout << "s empty: " << s.empty() << "\n";   // 1

    // --- 6.5 SAFE erase-during-iteration pattern (identical shape to unordered_set) ---
    set<int> s3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (auto sit = s3.begin(); sit != s3.end(); )
    {
        if (*sit % 2 == 0)
        {
            sit = s3.erase(sit);   // erase returns next valid iterator
        }
        else
        {
            ++sit;
        }
    }
    for (int x : s3)
    {
        cout << x << " ";   // 1 3 5 7 9
    }
    cout << "\n";

    // C++20 alternative:
    // std::erase_if(s3, [](int x) { return x % 2 == 0; });
}

// ============================================================
// SECTION 7 : CAPACITY FUNCTIONS
// ============================================================

void section7_capacity()
{
    set<int> s = {1, 2, 3, 4, 5};

    cout << s.size()     << "\n";   // 5 — O(1), tracked internally
    cout << s.empty()    << "\n";   // 0 (false)
    cout << s.max_size() << "\n";   // very large, allocator/system dependent
}

// ============================================================
// SECTION 8 : ITERATORS
// ============================================================

void section8_iterators()
{
    set<int> s = {50, 10, 40, 20, 30};

    // set provides BidirectionalIterator (NOT RandomAccessIterator) —
    // you can do ++it and --it in O(1), but NOT it + 5 in O(1).
    // This is WHY std::sort doesn't work on a set (it needs random access),
    // and WHY std::lower_bound (the free algorithm) on a set degrades to
    // O(n) even though set::lower_bound (the member) is O(log n) — the
    // free algorithm can only assume bidirectional stepping.

    // --- 8.1 begin() / end() — ALWAYS sorted ascending ---
    cout << "Ascending: ";
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        cout << *it << " ";   // 10 20 30 40 50
    }
    cout << "\n";

    // --- 8.2 rbegin() / rend() — REVERSE iteration ---
    // unordered_set does NOT have these at all — no defined order to reverse!
    // set DOES, precisely because it has a defined (sorted) order.
    cout << "Descending: ";
    for (auto it = s.rbegin(); it != s.rend(); ++it)
    {
        cout << *it << " ";   // 50 40 30 20 10
    }
    cout << "\n";

    // --- 8.3 cbegin()/cend(), crbegin()/crend() ---
    for (auto it = s.cbegin(); it != s.cend(); ++it)
    {
        cout << *it << " ";
    }
    cout << "\n";

    // --- 8.4 Range-based for (sorted order, always) ---
    for (int x : s)
    {
        cout << x << " ";
    }
    cout << "\n";

    // --- 8.5 Elements are ALWAYS read-only through iterators ---
    // Same reasoning as unordered_set: modifying an element in place
    // could break the tree's ordering invariant, silently corrupting it.
    //
    // This does NOT compile:
    //   auto it = s.begin();
    //   *it = 999;    // ERROR: expression is not assignable
    //
    // To "change" a value: erase old, insert new.
    auto it = s.find(20);
    if (it != s.end())
    {
        s.erase(it);
        s.insert(200);
    }

    // --- 8.6 Iterator invalidation rules (STRONGER than unordered_set) ---
    // INVALIDATED when:
    //   - erase() is called on THAT SPECIFIC element's iterator
    //
    // NOT invalidated by:
    //   - insert() of ANY new element, ever (no "rehash" concept exists here)
    //   - erase() of any OTHER element
    // This is a meaningfully stronger guarantee than unordered_set (which
    // invalidates everything on rehash) and much stronger than vector
    // (which invalidates everything on reallocation).
}

// ============================================================
// SECTION 9 : ORDERING & COMPARATORS
// ============================================================
// (This section REPLACES unordered_set's "hash policy" section —
//  set has no hashing at all; everything about its behavior is
//  governed by the Compare template parameter instead.)

void section9_comparators()
{
    // --- 9.1 Default: std::less<Key> — ascending order ---
    set<int> s1 = {3, 1, 4, 1, 5};   // {1, 3, 4, 5}

    // --- 9.2 std::greater<Key> — descending order ---
    set<int, greater<int>> s2 = {3, 1, 4, 1, 5};   // {5, 4, 3, 1}

    // --- 9.3 key_comp() — retrieve the comparator object in use ---
    auto cmp = s1.key_comp();
    cout << "1 < 3 by comparator: " << cmp(1, 3) << "\n";   // 1 (true)

    // --- 9.4 value_comp() ---
    // For set, value_comp() and key_comp() are IDENTICAL (key IS the
    // value in a set — this differs from map, where value_comp()
    // compares pair<Key,Value> by Key only, while key_comp() compares
    // bare keys).
    auto vcmp = s1.value_comp();
    cout << "value_comp same as key_comp: " << (cmp(2,5) == vcmp(2,5)) << "\n";

    // --- 9.5 Custom struct comparator (functor) ---
    struct Employee
    {
        int    id;
        string name;
        double salary;
    };

    struct BySalaryDesc
    {
        bool operator()(const Employee& a, const Employee& b) const
        {
            return a.salary > b.salary;   // descending by salary
        }
    };

    set<Employee, BySalaryDesc> payroll;
    payroll.insert({1, "Alice", 95000});
    payroll.insert({2, "Bob",   72000});
    payroll.insert({3, "Carol", 110000});

    cout << "Payroll, highest first:\n";
    for (const auto& e : payroll)
    {
        cout << "  " << e.name << ": " << e.salary << "\n";
    }
    // Output order: Carol(110000), Alice(95000), Bob(72000)

    // --- 9.6 Multi-field comparator using std::tie (lexicographic order) ---
    // This is the standard trick for "sort by field A, then field B"
    struct ByDeptThenSalary
    {
        bool operator()(const Employee& a, const Employee& b) const
        {
            // NOTE: illustrative — assumes Employee also had a 'dept' field;
            // shown here as the canonical std::tie lexicographic-compare idiom:
            return tie(a.id, a.salary) < tie(b.id, b.salary);
        }
    };

    // --- 9.7 Lambda comparator (needs decltype trick, same as unordered_set) ---
    auto byLength = [](const string& a, const string& b)
    {
        if (a.size() != b.size())
        {
            return a.size() < b.size();
        }
        return a < b;   // tie-break lexicographically — REQUIRED for strict weak ordering!
    };

    set<string, decltype(byLength)> byLen(byLength);
    byLen.insert("banana");
    byLen.insert("fig");
    byLen.insert("kiwi");
    byLen.insert("apple");

    cout << "Sorted by length: ";
    for (const auto& w : byLen)
    {
        cout << w << " ";   // fig kiwi apple banana
    }
    cout << "\n";

    // --- 9.8 THE STRICT WEAK ORDERING REQUIREMENT — a genuine correctness trap ---
    // A comparator MUST satisfy:
    //   1. Irreflexivity: cmp(a,a) is always false
    //   2. Asymmetry: if cmp(a,b) then !cmp(b,a)
    //   3. Transitivity: if cmp(a,b) and cmp(b,c) then cmp(a,c)
    //   4. Transitivity of equivalence
    //
    // VIOLATING THIS IS UNDEFINED BEHAVIOR — not a clean crash, but
    // SILENT TREE CORRUPTION: lost elements, infinite loops, or
    // elements that "vanish" because the tree can no longer find them.
    //
    // Classic broken comparator — using <= instead of < :
    //   auto badCmp = [](int a, int b) { return a <= b; };
    //   cmp(5,5) == true  -> VIOLATES irreflexivity -> UB
    // NEVER use <= or >= as a set/map comparator. Always strict < or >.
}

// ============================================================
// SECTION 10 : CUSTOM STRUCT AS KEY — TWO WAYS
// ============================================================

struct Point
{
    int x, y;
};

// --- 10.1 Method A: operator< as a free function or member ---
bool operator<(const Point& a, const Point& b)
{
    if (a.x != b.x)
    {
        return a.x < b.x;
    }
    return a.y < b.y;   // tie-break on y — lexicographic order
}
// Now: set<Point> works automatically using this operator

// --- 10.2 Method B: explicit comparator functor (no operator< needed) ---
struct PointCompareByDistance
{
    bool operator()(const Point& a, const Point& b) const
    {
        int da = a.x * a.x + a.y * a.y;
        int db = b.x * b.x + b.y * b.y;
        return da < db;   // order by distance from origin instead
    }
};

void section10_custom_key()
{
    // Method A — uses operator
    set<Point> pointsA;
    pointsA.insert({1, 2});
    pointsA.insert({3, 1});
    pointsA.insert({1, 2});   // duplicate — not inserted
    cout << "pointsA size: " << pointsA.size() << "\n";   // 2

    // Method B — explicit comparator, ordering by distance instead
    set<Point, PointCompareByDistance> pointsB;
    pointsB.insert({3, 4});   // distance 5
    pointsB.insert({1, 1});   // distance ~1.41
    pointsB.insert({0, 5});   // distance 5 — SAME distance as {3,4}!

    // IMPORTANT: {3,4} and {0,5} have the SAME distance (5), so under
    // PointCompareByDistance they are EQUIVALENT — only ONE of them
    // survives in the set, even though they are genuinely different points!
    cout << "pointsB size: " << pointsB.size() << "\n";   // 2, not 3
    // This is a direct, concrete illustration of Section 1's rule:
    // "equivalent" (per Compare) means "duplicate", REGARDLESS of operator==.

    // NOTE: unlike unordered_set, NO hash function is needed here at all —
    // this is genuinely simpler to set up correctly for custom types,
    // as long as you can define a meaningful ordering.
}

// ============================================================
// SECTION 11 : set vs unordered_set — DECISION TABLE
// ============================================================
//
//  Feature              set                       unordered_set
//  -------              ---                       -------------
//  Internal structure   Red-black tree            Hash table
//  Element order        Sorted (ascending/custom) No order (arbitrary)
//  insert                O(log n) GUARANTEED       O(1) avg, O(n) worst
//  erase                 O(log n) GUARANTEED       O(1) avg, O(n) worst
//  find/count             O(log n) GUARANTEED       O(1) avg, O(n) worst
//  lower_bound/upper_bound O(log n) — genuinely useful  exists, but only
//                                                    ever spans 0-1 elements
//  Range queries          YES — [lo,hi] in O(log n + k)   NO
//  rbegin/rend             YES                       NO — no order to reverse
//  Iterator category       Bidirectional              Forward only
//  Requires                Compare (operator<)        Hash + KeyEqual
//  Custom type setup       Simpler (just ordering)     Harder (hash quality matters)
//  Memory per element      3 ptrs + color bit          1 "next" ptr + bucket array share
//  Cache locality           Poor (tree — pointer chase) Poor (chaining — pointer chase)
//  Worst-case DoS resistance YES (height is bounded)     NO (bad hash -> O(n))
//
//  WHEN TO USE set:
//   ✓ Need sorted iteration
//   ✓ Need lower_bound/upper_bound/equal_range for RANGE queries
//   ✓ Need predecessor/successor of an arbitrary value
//   ✓ Need a WORST-CASE guarantee (untrusted/adversarial input)
//   ✓ Custom type has a natural ordering but no good hash available
//   ✓ Need rbegin/rend (descending traversal)
//
//  WHEN TO USE unordered_set:
//   ✓ Only need existence checks — no ordering benefit needed
//   ✓ Average-case O(1) matters more than worst-case guarantees
//   ✓ Have (or can write) a high-quality hash function

// ============================================================
// SECTION 12 : COMMON PATTERNS AND USE CASES
// ============================================================

void section12_patterns()
{
    // === PATTERN 1: Maintain a running sorted collection ===
    set<int> live_scores;
    for (int score : {85, 42, 91, 67, 91, 30})
    {
        live_scores.insert(score);   // always stays sorted, duplicates auto-removed
    }
    cout << "Sorted unique scores: ";
    for (int s : live_scores) cout << s << " ";   // 30 42 67 85 91
    cout << "\n";

    // === PATTERN 2: Find the K closest elements to a target ===
    set<int> nums = {1, 3, 8, 10, 15, 20};
    int target = 12;
    auto it = nums.lower_bound(target);   // first elem >= 12 -> 15

    vector<int> closest;
    auto left = it;
    auto right = it;
    bool leftValid = (left != nums.begin());
    if (leftValid) --left;
    bool rightValid = (right != nums.end());

    // walk outward comparing distances (classic "closest elements" merge)
    for (int k = 0; k < 2 && (leftValid || rightValid); ++k)
    {
        if (leftValid && (!rightValid || (target - *left) <= (*right - target)))
        {
            closest.push_back(*left);
            leftValid = (left != nums.begin());
            if (leftValid) --left;
        }
        else if (rightValid)
        {
            closest.push_back(*right);
            ++right;
            rightValid = (right != nums.end());
        }
    }
    cout << "Closest to 12: ";
    for (int x : closest) cout << x << " ";   // 10 15
    cout << "\n";

    // === PATTERN 3: Coordinate compression ===
    vector<int> raw = {100, 5, 100, 42, 5, 999};
    set<int> unique_sorted(raw.begin(), raw.end());   // {5, 42, 100, 999} — auto-sorted
    unordered_map<int, int> rank;
    int r = 0;
    for (int v : unique_sorted)
    {
        rank[v] = r++;
    }
    cout << "Compressed rank of 100: " << rank[100] << "\n";   // 2

    // === PATTERN 4: Set algorithms from <algorithm> — natural fit for sorted data ===
    set<int> A = {1, 2, 3, 4, 5};
    set<int> B = {3, 4, 5, 6, 7};

    vector<int> unionResult, interResult, diffResult, symDiffResult;

    set_union(A.begin(), A.end(), B.begin(), B.end(), back_inserter(unionResult));
    set_intersection(A.begin(), A.end(), B.begin(), B.end(), back_inserter(interResult));
    set_difference(A.begin(), A.end(), B.begin(), B.end(), back_inserter(diffResult));
    set_symmetric_difference(A.begin(), A.end(), B.begin(), B.end(), back_inserter(symDiffResult));

    cout << "Union: ";        for (int x : unionResult)   cout << x << " "; cout << "\n";  // 1 2 3 4 5 6 7
    cout << "Intersection: "; for (int x : interResult)    cout << x << " "; cout << "\n";  // 3 4 5
    cout << "A - B: ";        for (int x : diffResult)     cout << x << " "; cout << "\n";  // 1 2
    cout << "Sym diff: ";     for (int x : symDiffResult)  cout << x << " "; cout << "\n";  // 1 2 6 7

    // === PATTERN 5: includes() — is B a subset of A? ===
    set<int> small = {2, 4};
    bool isSubset = includes(A.begin(), A.end(), small.begin(), small.end());
    cout << "small subset of A: " << isSubset << "\n";   // 1 (true)

    // === PATTERN 6: Sliding-window-style "keep only recent K distinct values, sorted" ===
    // (Contrast with the deque-based Sliding Window Maximum — this variant
    //  needs SORTED distinct tracking, which set gives naturally.)
    set<int> recentUnique;
    vector<int> stream = {5, 3, 5, 8, 1, 3};
    for (int v : stream)
    {
        recentUnique.insert(v);
    }
    cout << "All distinct values seen, sorted: ";
    for (int x : recentUnique) cout << x << " ";   // 1 3 5 8
    cout << "\n";
}

// ============================================================
// SECTION 13 : std::multiset
// ============================================================
//
//  std::multiset — like set but ALLOWS DUPLICATES, still ALWAYS SORTED
//  Header: same — #include <set>
//
//  Key differences from set:
//   - insert() always inserts (returns plain iterator, not pair<iter,bool>)
//   - count(key) can return 0, 1, 2, 3, ...
//   - equal_range(key) can span MULTIPLE consecutive elements
//   - erase(key) removes ALL matching elements
//   - erase(iterator) removes exactly ONE element

void section13_multiset()
{
    multiset<int> ms;

    ms.insert(10);
    ms.insert(20);
    ms.insert(10);   // allowed
    ms.insert(20);   // allowed
    ms.insert(10);   // 10 inserted 3 times

    cout << "ms size: "  << ms.size()     << "\n";   // 5
    cout << "count(10): " << ms.count(10) << "\n";   // 3

    // Multiset ALWAYS keeps duplicates grouped together (sorted order),
    // so equal_range genuinely spans a contiguous run:
    auto [first, last] = ms.equal_range(10);
    cout << "All 10s: ";
    for (auto it = first; it != last; ++it)
    {
        cout << *it << " ";   // 10 10 10
    }
    cout << "\n";

    // erase by VALUE removes ALL matching
    ms.erase(10);
    cout << "After erase(10), size: " << ms.size() << "\n";   // 2

    // erase by ITERATOR removes exactly ONE
    auto it = ms.find(20);
    ms.erase(it);
    cout << "After erase(iter), count(20): " << ms.count(20) << "\n";   // 1

    // --- Classic multiset use case: running median with two multisets ---
    // (or with one multiset + a pivot iterator kept balanced — the
    //  "Sliding Window Median" (LC #480) pattern from earlier in this
    //  conversation uses exactly this container)
    multiset<int> lower, upper;   // lower holds smaller half, upper holds larger half
    auto rebalance = [&]()
    {
        while (lower.size() > upper.size() + 1)
        {
            upper.insert(*lower.rbegin());
            lower.erase(prev(lower.end()));
        }
        while (upper.size() > lower.size())
        {
            lower.insert(*upper.begin());
            upper.erase(upper.begin());
        }
    };

    for (int v : {5, 15, 1, 3})
    {
        if (lower.empty() || v <= *lower.rbegin())
        {
            lower.insert(v);
        }
        else
        {
            upper.insert(v);
        }
        rebalance();
    }
    double median = (lower.size() > upper.size())
        ? *lower.rbegin()
        : (*lower.rbegin() + *upper.begin()) / 2.0;
    cout << "Running median: " << median << "\n";
}

// ============================================================
// SECTION 14 : THREAD SAFETY
// ============================================================
//
//  Same rule as every standard container: set is NOT thread-safe
//  by default.
//
//  Safe concurrently (read-only):
//   - Multiple threads calling find(), count(), lower_bound() etc.
//     SIMULTANEOUSLY is safe, AS LONG AS no thread modifies the tree.
//
//  UNSAFE without synchronization:
//   - Any insert/erase happening concurrently with ANY other access
//     (even another read) — tree rotations touch shared node pointers,
//     so even a "read-only" traversal can observe a half-rotated tree.
//
//  Typical fix (identical pattern to unordered_set):
//   std::shared_mutex mtx;
//   set<int> shared_set;
//
//   void reader(int key)
//   {
//       shared_lock lock(mtx);
//       bool found = shared_set.count(key);
//   }
//
//   void writer(int val)
//   {
//       unique_lock lock(mtx);
//       shared_set.insert(val);
//   }

// ============================================================
// SECTION 15 : MEMORY LAYOUT AND PERFORMANCE TIPS
// ============================================================
//
//  PER-NODE OVERHEAD:
//   left ptr + right ptr + parent ptr + color bit + key
//   = 8 + 8 + 8 + (packed) + sizeof(key)  =>  typically 24-32 bytes
//     of pure overhead PER ELEMENT, before even counting the key itself.
//   unordered_set's per-node overhead is usually smaller (just 1 "next"
//   pointer), but unordered_set ALSO needs a separate bucket array —
//   the total memory picture depends on load factor vs tree overhead,
//   and is genuinely workload-dependent; don't assume either always wins.
//
//  CACHE BEHAVIOR:
//   Both set (tree) and unordered_set (chaining) suffer from pointer-
//   chasing — neither is cache-friendly like a flat vector. Tree
//   traversal additionally has WORSE locality than hash bucket chaining
//   in many real workloads, because in-order traversal jumps all over
//   the tree's memory layout rather than walking a short local chain.
//
//  PERFORMANCE TIPS:
//
//  TIP 1: Use the end()-hint trick for bulk ASCENDING inserts
//   set<int> s;
//   for (int i = 0; i < 1'000'000; ++i)
//       s.insert(s.end(), i);          // O(1) amortized per insert — CORRECT hint
//   // Without the hint: O(log n) per insert -> O(n log n) total. A REAL,
//   // measurable difference for large bulk loads in ascending order.
//
//  TIP 2: Use count()/contains() for existence checks, not find()!=end()
//   if (s.contains(key)) { ... }        // C++20, clearest
//   if (s.count(key))    { ... }        // pre-C++20, equally fast
//   if (s.find(key) != s.end()) { ... } // verbose, only needed if you need the iterator itself
//
//  TIP 3: NEVER use the free std::find()/std::lower_bound() on a set
//   find(s.begin(), s.end(), key);              // O(n) — WRONG choice
//   s.find(key);                                  // O(log n) — CORRECT
//   lower_bound(s.begin(), s.end(), key);         // O(n) on bidirectional iterators!
//   s.lower_bound(key);                            // O(log n) — CORRECT
//   // Both compile silently either way — this is a genuinely easy mistake
//   // to make and a favorite "gotcha" in senior interviews.
//
//  TIP 4: For range queries, ALWAYS reach for set over unordered_set
//   // "count elements in [lo, hi]" — trivial with set:
//   int count = (int)distance(s.lower_bound(lo), s.upper_bound(hi));
//   // genuinely NOT possible in better than O(n) with unordered_set.
//
//  TIP 5: For struct keys, prefer a comparator over defining global operator
//   // if you need MULTIPLE orderings of the same struct in different
//   // contexts (e.g. one set sorted by id, another by name) — you cannot
//   // have two different operator< overloads for the same type, but you
//   // CAN have as many different Compare functors as you like.

// ============================================================
// SECTION 16 : GNU PBDS ordered_set — ORDER STATISTICS
// ============================================================
//
//  std::set CANNOT answer "what is the k-th smallest element?" or
//  "how many elements are strictly less than X?" in better than O(n)
//  using only standard operations (distance(begin(), it) is O(n) on a
//  Bidirectional iterator, since it can't jump).
//
//  GCC's libstdc++ ships a non-standard EXTENSION — Policy-Based Data
//  Structures (PBDS) — that adds exactly this capability in O(log n).
//  This is GCC/libstdc++-SPECIFIC (works on your Linux g++ setup, but
//  is NOT portable to MSVC or a non-libstdc++ Clang build, and is NOT
//  accepted on LeetCode — useful for GFG/Codeforces-style judges and
//  for demonstrating deep STL knowledge in an interview).
//
//  #include <ext/pb_ds/assoc_container.hpp>
//  #include <ext/pb_ds/tree_policy.hpp>
//  using namespace __gnu_pbds;
//
//  typedef tree
//      int,                              // Key type
//      null_type,                        // Mapped type (null_type = "set", not "map")
//      less<int>,                        // Comparator
//      rb_tree_tag,                      // Underlying structure: red-black tree
//      tree_order_statistics_node_update // ENABLES order_of_key / find_by_order
//  > ordered_set;
//
//  ordered_set os;
//  os.insert(5);
//  os.insert(1);
//  os.insert(3);
//  os.insert(9);
//
//  os.order_of_key(3);       // O(log n) — count of elements STRICTLY LESS than 3 -> 1
//  os.order_of_key(9);       // -> 2 (elements 1, 3 are less than 9)
//  *os.find_by_order(0);     // O(log n) — the 0-th smallest element -> 1
//  *os.find_by_order(2);     // -> 9 (0-indexed: 1, 3, 5, 9 -> index 2 is 9... wait, 5 is index 2)
//
//  This gives you a full "order statistics tree" — k-th smallest,
//  rank of an element — entirely in O(log n), something plain
//  std::set fundamentally cannot do without an O(n) linear walk.

// ============================================================
// SECTION 17 : C++17 — extract() AND merge()
// ============================================================

void section17_cpp17()
{
    // --- 17.1 extract(key) — remove WITHOUT destroying, get a movable node handle ---
    set<string> s1 = {"apple", "banana", "cherry"};
    set<string> s2;

    auto node = s1.extract("banana");
    if (!node.empty())
    {
        cout << "Extracted: " << node.value() << "\n";
        s2.insert(move(node));   // moved into s2 — ZERO allocation
    }
    cout << "s1 size: " << s1.size() << "\n";   // 2
    cout << "s2 size: " << s2.size() << "\n";   // 1

    // --- 17.2 merge(other) ---
    // Moves ALL elements from 'other' into 'this' with no allocation.
    // If an element already exists in 'this' (per Compare), it stays in 'other'.
    set<int> A = {1, 2, 3};
    set<int> B = {3, 4, 5};   // 3 exists in both

    A.merge(B);
    cout << "A size: " << A.size() << "\n";   // 5 -> {1,2,3,4,5}
    cout << "B size: " << B.size() << "\n";   // 1 -> {3} (couldn't move, stayed behind)

    // NOTE: this is the CONTAINER'S .merge() MEMBER function — do not
    // confuse it with std::merge() the ALGORITHM from <algorithm>, which
    // merges two sorted RANGES into a third output range/container and
    // does NOT move nodes or mutate either input container.
}

// ============================================================
// SECTION 18 : LEETCODE / INTERVIEW PATTERNS USING set/multiset
// ============================================================

void section18_leetcode_patterns()
{
    // --- Pattern 1: Contains Duplicate III (LC #220) — bounded value AND index gap ---
    auto containsNearbyAlmostDuplicate =
        [](vector<int>& nums, int indexDiff, int valueDiff) -> bool
    {
        set<long long> window;

        for (int i = 0; i < (int)nums.size(); ++i)
        {
            auto it = window.lower_bound((long long)nums[i] - valueDiff);
            if (it != window.end() && *it <= (long long)nums[i] + valueDiff)
            {
                return true;
            }

            window.insert(nums[i]);

            if (i >= indexDiff)
            {
                window.erase(nums[i - indexDiff]);
            }
        }
        return false;
    };
    vector<int> v1 = {1, 5, 9, 1, 5, 9};
    cout << "Nearby almost duplicate: "
         << containsNearbyAlmostDuplicate(v1, 2, 3) << "\n";

    // --- Pattern 2: My Calendar I (LC #729) — interval overlap checking via set<pair> ---
    struct Calendar
    {
        set<pair<int,int>> bookings;   // sorted by start time automatically

        bool book(int start, int end)
        {
            auto it = bookings.lower_bound({start, end});

            // check the booking that starts AT or AFTER 'start'
            if (it != bookings.end() && it->first < end)
            {
                return false;   // overlaps with the one starting right after us
            }
            // check the booking right BEFORE 'start'
            if (it != bookings.begin())
            {
                auto prevIt = prev(it);
                if (prevIt->second > start)
                {
                    return false;   // overlaps with the one just before us
                }
            }
            bookings.insert({start, end});
            return true;
        }
    };
    Calendar cal;
    cout << "Book 10-20: " << cal.book(10, 20) << "\n";   // 1 (true)
    cout << "Book 15-25: " << cal.book(15, 25) << "\n";   // 0 (false — overlaps)
    cout << "Book 20-30: " << cal.book(20, 30) << "\n";   // 1 (true — touching, not overlapping)

    // --- Pattern 3: Exam Room (LC #855) — set maintaining sorted seat positions ---
    // (sketch only — full class omitted for brevity, but the core idea:)
    // set<int> occupiedSeats; using lower_bound/upper_bound to find the
    // largest gap between consecutive occupied seats in O(log n) per seat() call.

    // --- Pattern 4: Longest Consecutive Sequence via set (alternative to unordered_set) ---
    auto longestConsecutive = [](vector<int>& nums) -> int
    {
        set<int> s(nums.begin(), nums.end());   // sorted AND deduplicated in one step
        int best = 0, curLen = 1;

        for (auto it = s.begin(); it != s.end(); ++it)
        {
            auto next_it = next(it);
            if (next_it != s.end() && *next_it == *it + 1)
            {
                ++curLen;
            }
            else
            {
                best = max(best, curLen);
                curLen = 1;
            }
        }
        return best;
    };
    vector<int> v2 = {100, 4, 200, 1, 3, 2};
    cout << "Longest consecutive (via set): " << longestConsecutive(v2) << "\n";   // 4
    // NOTE: the unordered_set version of this (from your earlier practice)
    // is O(n) average; THIS set-based version is O(n log n) due to the
    // tree — worth explicitly stating this trade-off if asked which to prefer.
}

// ============================================================
// SECTION 19 : COMPLETE METHOD REFERENCE
// ============================================================
//
//  CONSTRUCTORS:
//   set()                                     — default, empty, ascending
//   set(comp)                                 — empty, custom comparator
//   set(first, last)                          — range constructor
//   set(first, last, comp)                    — range + custom comparator
//   set(init_list)                            — {1,2,3,...}
//   set(other)                                — copy constructor
//   set(move(other))                          — move constructor
//
//  ASSIGNMENT:
//   s = other                                 — copy assignment
//   s = move(other)                           — move assignment
//   s = {1,2,3}                               — init list assignment
//
//  ITERATORS:
//   begin()   / end()                         — ascending order
//   rbegin()  / rend()                        — descending order (set-only! not in unordered_set)
//   cbegin()  / cend()                        — const ascending
//   crbegin() / crend()                       — const descending
//
//  CAPACITY:
//   empty()                                   — true if no elements
//   size()                                    — O(1)
//   max_size()                                — max possible elements
//
//  MODIFIERS:
//   insert(val)              → pair<iterator,bool>
//   insert(hint, val)        → iterator            (amortized O(1) if hint correct!)
//   insert(first, last)      → void
//   insert({val1,val2,...})  → void
//   emplace(args...)         → pair<iterator,bool>
//   emplace_hint(hint,args)  → iterator
//   erase(val)               → size_t (0 or 1)
//   erase(iterator)          → iterator (next)
//   erase(first, last)       → iterator (after last)
//   clear()                  → void
//   swap(other)              → void
//   extract(val)              [C++17] — extract node
//   merge(other)               [C++17] — merge another set
//
//  LOOKUP (all O(log n)):
//   find(key)                → iterator
//   count(key)                → 0 or 1
//   contains(key)              → bool [C++20]
//   equal_range(key)           → pair<iterator,iterator>
//   lower_bound(key)           → iterator (first >= key)
//   upper_bound(key)           → iterator (first > key)
//
//  OBSERVERS:
//   key_comp()                → Compare (the comparator object)
//   value_comp()               → Compare (identical to key_comp() for set)
//   get_allocator()             → Allocator

// ============================================================
// SECTION 20 : FULL WORKING DEMO
// ============================================================

int main()
{
    cout << "===== std::set Complete Demo =====\n\n";

    set<int> s;

    for (int i = 10; i >= 1; --i)
    {
        s.insert(s.end(), i);   // deliberately DESCENDING insert order,
                                 // hint is WRONG here on purpose — just to
                                 // show the tree still ends up correctly sorted
    }

    cout << "Size: "  << s.size()  << "\n";   // 10
    cout << "Empty: " << s.empty() << "\n";   // 0

    cout << "Ascending: ";
    for (int x : s) cout << x << " ";   // 1 2 3 4 5 6 7 8 9 10
    cout << "\n";

    cout << "Descending (rbegin/rend): ";
    for (auto it = s.rbegin(); it != s.rend(); ++it) cout << *it << " ";
    cout << "\n";

    cout << "lower_bound(5): " << *s.lower_bound(5) << "\n";   // 5
    cout << "upper_bound(5): " << *s.upper_bound(5) << "\n";   // 6

    s.erase(5);
    cout << "After erase(5), contains 5: " << s.count(5) << "\n";   // 0

    s.clear();
    cout << "After clear, size: " << s.size() << "\n";   // 0

    cout << "\n===== Running all sections =====\n\n";

    section3_declaration();
    section4_insert();
    section5_lookup();
    section6_erase();
    section7_capacity();
    section8_iterators();
    section9_comparators();
    section10_custom_key();
    section12_patterns();
    section13_multiset();
    section17_cpp17();
    section18_leetcode_patterns();

    cout << "\n===== Done =====\n";
    return 0;
}

// ============================================================
// SECTION 21 : INTERVIEW QUESTIONS ON std::set
// ============================================================
//
//  Q1: Why is set's O(log n) called a WORST-CASE guarantee, while
//      unordered_set's O(1) is only an AVERAGE?
//  A1: set's red-black tree mathematically bounds height to
//      2*log2(n+1) via its 5 balancing properties — no input can ever
//      violate this. unordered_set's O(1) depends entirely on hash
//      quality; a bad or adversarial hash collapses it to O(n), same
//      as a linked list. set has no equivalent failure mode.
//
//  Q2: Why does std::sort not work on a std::set?
//  A2: set provides only BidirectionalIterator, not RandomAccessIterator.
//      std::sort requires O(1) random access (it + n) to do its
//      partitioning; set's tree structure can only step one node at a
//      time (++/--), which is fundamentally incompatible with sort's
//      algorithm. Also — a set is ALREADY sorted by definition, so
//      "sorting" it is a conceptual non-operation anyway.
//
//  Q3: What happens if you call std::lower_bound (the free algorithm)
//      on a set, instead of set::lower_bound (the member function)?
//  A3: It still compiles and gives the correct ANSWER, but degrades to
//      O(n) — the free algorithm can only assume BidirectionalIterator
//      stepping, so it can't binary-search the underlying tree
//      structure directly. ALWAYS prefer the member function.
//
//  Q4: Can you modify an element stored in a set?
//  A4: No — same reasoning as unordered_set: elements are exposed as
//      const through iterators, because mutating a key in place could
//      silently break the tree's ordering invariant, corrupting future
//      lookups. To "modify": erase the old value, insert the new one.
//
//  Q5: What makes a comparator "invalid," and what happens if you use one?
//  A5: It must satisfy strict weak ordering: irreflexive (cmp(a,a) is
//      always false), asymmetric, and transitive. A common broken
//      example is using <= instead of < — this violates irreflexivity
//      and causes SILENT tree corruption (not a clean crash): elements
//      can become unfindable, or the tree can enter inconsistent states.
//
//  Q6: How do you find the predecessor and successor of a value that
//      may not even be present in the set?
//  A6: successor = s.lower_bound(val) (first element >= val, or first
//      element > val if you specifically want STRICTLY greater — use
//      upper_bound for that case). predecessor = one step back (--it)
//      from that iterator, guarding against s.begin().
//
//  Q7: What is the insertion-HINT optimization, and when does it apply?
//  A7: If the hint iterator points to the position immediately where
//      the new element would go, insertion drops from O(log n) to
//      amortized O(1) — the tree can skip the top-down search entirely.
//      The classic application: inserting values in ascending order,
//      always passing s.end() as the hint — turns an O(n log n) bulk
//      insert into O(n) overall.
//
//  Q8: Why does set have rbegin()/rend() but unordered_set does not?
//  A8: rbegin()/rend() reverse an EXISTING, well-defined order. set has
//      one (ascending, via Compare); unordered_set has NO defined order
//      at all — there is nothing meaningful to "reverse."
//
//  Q9: How would you count the number of elements in a set within a
//      range [lo, hi] efficiently?
//  A9: distance(s.lower_bound(lo), s.upper_bound(hi)) — but note
//      std::distance on a BidirectionalIterator is itself O(k) where k
//      is the number of elements in that range (it must step through
//      them one at a time — no random-access shortcut exists). This is
//      still far better than any equivalent operation on unordered_set,
//      which has no ordering to exploit at all for range queries.
//
//  Q10: What is GNU PBDS ordered_set, and what problem does it solve
//       that plain std::set cannot?
//  A10: A GCC/libstdc++-specific extension providing order_of_key() and
//       find_by_order() — both O(log n) — enabling "k-th smallest
//       element" and "rank of this value" queries. Plain std::set has
//       no way to answer either in better than O(n), since
//       distance(begin(), it) requires physically stepping through
//       every element up to it on a BidirectionalIterator.







// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion 
//          -fsanitize=address,undefined Set_Explanation.cpp -o set_demo
// ============================================================




// ============================================================
// ADD THESE INCLUDES TO THE TOP OF YOUR EXISTING FILE
// ============================================================
#include <chrono>        // benchmarking (Section 29)
#include <cstdint>       // uintptr_t (Section 28)
#include <sys/types.h>   // pid_t (Section 28, Linux-specific)
#include <memory>        // shared_ptr, unique_ptr, make_shared, make_unique (Section 23)
#include <tuple>         // std::tie (Section 22)
#include <utility>       // std::pair (Section 22)

// ============================================================
// SECTION 22 : COMPOSITE KEYS — pair / tuple / vector / struct
//              (this REPLACES unordered_set's "hash combining"
//               section — set needs NO hash function at all,
//               only a correct ORDERING, which is much easier)
// ============================================================
//
// std::pair, std::tuple, and std::vector ALL provide operator
// out of the box — LEXICOGRAPHIC comparison, recursively using
// each element's own operator<. This means set<pair<int,int>>,
// set<tuple<...>>, and set<vector<int>> ALL work with ZERO extra
// code, unlike unordered_set which needed PairHash/TupleHash/
// VectorHash functors before any of the equivalent code would
// even compile. This is the single biggest practical advantage
// of ordered containers over hash containers for composite keys.

void section22_composite_keys()
{
    // pair<int,int> already has operator< (compare .first, then .second)
    set<pair<int,int>> coord_set;
    coord_set.insert({1, 2});
    coord_set.insert({3, 4});
    coord_set.insert({1, 2});          // duplicate, not inserted
    coord_set.insert({1, 5});          // differs in .second -> distinct

    cout << "coord_set size: " << coord_set.size() << "\n";   // 3
    cout << "Sorted order: ";
    for (const auto& [x, y] : coord_set)
    {
        cout << "(" << x << "," << y << ") ";
    }
    cout << "\n";
    // Output: (1,2) (1,5) (3,4) -- lexicographic, NO custom comparator needed!

    // tuple<Ts...> also has operator< built in (lexicographic, all fields)
    set<tuple<int,int,int>> triple_set;
    triple_set.insert({1, 2, 3});
    triple_set.insert({1, 2, 2});      // differs in 3rd field -> sorts BEFORE {1,2,3}
    triple_set.insert({0, 9, 9});      // differs in 1st field -> sorts FIRST

    cout << "Sorted triples: ";
    for (const auto& [a, b, c] : triple_set)
    {
        cout << "(" << a << "," << b << "," << c << ") ";
    }
    cout << "\n";
    // Output: (0,9,9) (1,2,2) (1,2,3)

    // vector<T> ALSO has operator< built in (lexicographic, like string compare)
    set<vector<int>> path_set;
    path_set.insert({1, 2, 3});
    path_set.insert({1, 2, 3});         // exact duplicate, not inserted
    path_set.insert({1, 2});            // shorter prefix -> sorts BEFORE {1,2,3}
    path_set.insert({3, 2, 1});         // sorts LAST

    cout << "path_set size: " << path_set.size() << "\n";   // 3
    for (const auto& p : path_set)
    {
        cout << "[ ";
        for (int x : p) cout << x << " ";
        cout << "] ";
    }
    cout << "\n";
    // Output: [ 1 2 ] [ 1 2 3 ] [ 3 2 1 ]

    // CONTRAST: unordered_set<pair<int,int>> would NOT compile without
    // first writing a PairHash functor (see Unordered_set_Explanation.cpp
    // Section 22). set needs nothing extra, because ordering — not
    // hashing — is the only requirement, and pair/tuple/vector already
    // define it for you.
}

struct Employee
{
    string department;
    int    id;
    double salary;
};

struct EmployeeCompare
{
    bool operator()(const Employee& a, const Employee& b) const
    {
        // lexicographic multi-field compare: department, then id, then salary
        // tie() builds a temporary tuple<const T&...> and reuses tuple's
        // own operator< — same mechanism as the built-in tuple ordering
        // above, applied field-by-field to a custom struct.
        return tie(a.department, a.id, a.salary) < tie(b.department, b.id, b.salary);
    }
};

void section22_struct_comparator()
{
    set<Employee, EmployeeCompare> staff;
    staff.insert({"Engineering", 42, 95000});
    staff.insert({"Engineering", 10, 88000});
    staff.insert({"Sales", 5, 60000});

    for (const auto& e : staff)
    {
        cout << e.department << " #" << e.id << " $" << e.salary << "\n";
    }
    // Output order: Engineering #10, Engineering #42, Sales #5
}

// ============================================================
// SECTION 23 : set OF POINTERS AND SMART POINTERS
// ============================================================
//
// set<T*> uses std::less<T*> BY DEFAULT — not raw operator<.
// This distinction matters more here than it first appears:
//
// Comparing pointers into DIFFERENT arrays/objects with plain `<`
// is UNSPECIFIED behavior in the core language. std::less<T*>,
// however, is SPECIFICALLY REQUIRED by the standard to impose a
// total, consistent order over ALL pointers of that type, even
// pointers to unrelated objects. This is precisely why set<T*>
// (which defaults to less<T*>) is always safe and portable,
// whereas passing your OWN comparator `[](T* a, T* b){return a<b;}`
// explicitly would NOT carry that same guarantee.

void section23_pointers()
{
    int a = 5, b = 5;
    int* pa = &a;
    int* pb = &b;

    set<int*> ptr_set;     // uses std::less<int*> internally — safe, total order
    ptr_set.insert(pa);
    ptr_set.insert(pb);
    cout << "ptr_set size: " << ptr_set.size() << "\n";   // 2 — different addresses

    // --- shared_ptr<T> — default ordering compares get(), NOT the pointee ---
    set<shared_ptr<int>> sp_set;
    auto sp1 = make_shared<int>(10);
    auto sp2 = make_shared<int>(10);   // different object, same VALUE
    sp_set.insert(sp1);
    sp_set.insert(sp2);
    cout << "sp_set size: " << sp_set.size() << "\n";   // 2 — different underlying objects

    auto sp3 = sp1;                      // shares ownership with sp1 — SAME get()
    sp_set.insert(sp3);
    cout << "sp_set size after sp3: " << sp_set.size() << "\n";   // still 2

    // --- unique_ptr<T> — move-only, must MOVE into the set ---
    set<unique_ptr<int>> up_set;
    auto up = make_unique<int>(42);
    up_set.insert(move(up));            // must explicitly move — 'up' is now empty
    cout << "up_set size: " << up_set.size() << "\n";   // 1
    // Ordering for unique_ptr<T> also uses std::less<unique_ptr<T>>,
    // which compares get() — same total-order guarantee as raw pointers.

    // --- Ordering shared_ptr BY POINTED-TO VALUE instead of address ---
    struct DerefCompare
    {
        bool operator()(const shared_ptr<int>& x, const shared_ptr<int>& y) const
        {
            if (!x || !y)
            {
                return (bool)x < (bool)y;   // handle null safely
            }
            return *x < *y;
        }
    };

    set<shared_ptr<int>, DerefCompare> value_set;
    value_set.insert(make_shared<int>(99));
    value_set.insert(make_shared<int>(99));   // DIFFERENT object, SAME value
    cout << "value_set size (by value): " << value_set.size() << "\n";   // 1
}

// ============================================================
// SECTION 24 : DEBUGGING std::set WITH GDB
// ============================================================
//
// Like unordered_set, std::set's raw internal layout (a red-black
// tree of _Rb_tree_node objects, each with color/parent/left/right)
// is not human-readable with a plain 'print' unless pretty-printers
// are active.
//
// --- 24.1 GCC libstdc++ Python pretty-printers (if installed) ---
//   (gdb) print s
//   $1 = std::set with 5 elements = {1, 2, 3, 4, 5}
//
//   Without pretty-printers you'd instead see raw internals like:
//   $1 = {_M_t = {_M_impl = {_M_header = {_M_color = red, ...
//
// --- 24.2 Enabling them (identical setup to your unordered_set notes) ---
//   Add to ~/.gdbinit:
//   python
//   import sys
//   sys.path.insert(0, '/usr/share/gcc/python')
//   from libstdcxx.v6.printers import register_libstdcxx_printers
//   register_libstdcxx_printers(None)
//   end
//
// --- 24.3 Practical fallback: a dump helper you call live in gdb ---

void dump_set(const set<int>& s)
{
    cerr << "[[ set dump: size=" << s.size() << " (sorted ascending) ]]\n";
    for (const auto& elem : s)
    {
        cerr << "  " << elem << "\n";
    }
}

void section24_gdb_debugging_demo()
{
    set<int> s = {30, 10, 20};
    dump_set(s);
    // Inside an actual gdb session at a breakpoint, run:
    //   (gdb) call dump_set(s)
    // Works even WITHOUT pretty-printers — it's just real code walking
    // the tree through its own public begin()/end() API.
    cout << "See stderr output above; try 'call dump_set(s)' in gdb\n";

    // --- 24.4 Inspecting internal node layout is NOT worth doing ---
    // You COULD walk the raw _Rb_tree_node_base parent/left/right chain
    // manually via gdb expressions, but this is fragile and tied to
    // libstdc++'s INTERNAL, non-portable layout, which can change
    // between GCC versions without warning. Prefer a small diagnostic
    // helper (like dump_set above) using ONLY the public API — it
    // survives library upgrades; raw internal-struct gdb expressions
    // do not.
}

// ============================================================
// SECTION 25 : IMPLEMENTATION DIFFERENCES ACROSS COMPILERS
// ============================================================
//
// Unlike unordered_set (whose bucket policy and resulting ITERATION
// ORDER genuinely differ across GCC/Clang/MSVC), std::set's iteration
// order is a PORTABLE, WELL-DEFINED property of the algorithm itself:
// in-order traversal of a tree ordered by Compare ALWAYS produces the
// same sequence, regardless of which balanced-tree implementation
// sits underneath.
//
//   GCC libstdc++, Clang libc++, and MSVC STL all implement set/map
//   using a RED-BLACK TREE specifically. This isn't literally mandated
//   by the standard's wording, but the complexity guarantees the
//   standard DOES mandate — O(log n) worst-case insert/erase/find,
//   plus "erasing one element never invalidates OTHER iterators" —
//   together push every serious implementation toward a self-
//   balancing BST, and red-black trees are the overwhelming choice
//   because their rebalancing needs at most O(1) rotations per
//   insert/erase (see Section 29), unlike some alternatives.
//
// WHAT GENUINELY DOES differ across implementations:
//   - sizeof(std::set<int>): commonly 48 bytes on 64-bit GCC/libstdc++
//     (a sentinel "header" node holding color+parent+left+right, plus
//     a cached element count) — other implementations may differ by
//     a few bytes.
//   - Internal node layout and exact rebalancing code paths.
//   - size() is REQUIRED to be O(1) since C++11 — this one is actually
//     guaranteed, not just "commonly true."
//
// PRACTICAL IMPLICATION: unlike your unordered_set growth-policy
// experiment, there is NO analogous "iteration order changes across
// compilers" test for std::set — ascending (or Compare-defined) order
// is a portable guarantee you can rely on in cross-platform code,
// tests, and even reproducible log output.

void section25_sizeof_check()
{
    cout << "sizeof(set<int>): " << sizeof(set<int>) << " bytes\n";
    // Typically 48 on 64-bit GCC/libstdc++ — try it on your own toolchain.
    // The EXACT byte count is implementation-defined, but the ORDER
    // elements come out in below is NOT:
    set<int> s = {5, 3, 8, 1, 9};
    for (int x : s)
    {
        cout << x << " ";   // ALWAYS 1 3 5 8 9 — on GCC, Clang, MSVC, every time
    }
    cout << "\n";
}

// ============================================================
// SECTION 26 : EXCEPTION SAFETY GUARANTEES
// ============================================================
//
//  WHY set's EXCEPTION SAFETY STORY IS SIMPLER THAN vector's:
//   vector::push_back may need to relocate EVERY existing element on
//   reallocation, which is why the noexcept-move-vs-copy fallback
//   (see your vector interview bank, Q15) exists at all.
//
//   set::insert() creates exactly ONE new node and links it into the
//   tree. EXISTING nodes are NEVER copied, moved, or reconstructed —
//   only POINTERS among them are relinked during rebalancing. So if
//   the new element's constructor throws, or the allocator fails, or
//   the Compare functor throws during the search-for-position phase,
//   the tree is left EXACTLY as it was — STRONG exception guarantee,
//   with no equivalent "does the value type have a safe move ctor"
//   question to ask, because no EXISTING value is ever relocated in
//   memory by an insert.
//
//  ERASE / CLEAR / SWAP:
//   erase() only relinks pointers and recolors nodes during
//   rebalancing — pure pointer manipulation, no allocation, no
//   possibility of an exception (assuming Compare itself doesn't
//   throw, which a well-behaved comparator never should). clear() and
//   swap() are noexcept for the same reason.
//
//  RULE OF THUMB: set/map's node-based design means "insert one thing"
//  and "everything else stays untouched in memory" are the SAME fact —
//  this is what makes both the exception-safety story AND the
//  iterator-stability story (Section 8.6) simpler than vector's.

// ============================================================
// SECTION 27 : noexcept SPECIFICATIONS
// ============================================================
//
//  Move constructor / move assignment / swap on set are conditionally
//  noexcept, based on whether Compare and Allocator have noexcept move
//  operations — true for the defaults (std::less, std::allocator).
//
//  This matters, exactly as with unordered_set, when set<T> is stored
//  as an ELEMENT of an outer container (e.g. vector<set<int>>) — the
//  outer container inspects THIS noexcept-ness to decide move vs copy
//  during ITS OWN reallocation (see your vector bank Q15).

void section27_noexcept_check()
{
    cout << boolalpha;
    cout << "set<int> move ctor noexcept: "
         << is_nothrow_move_constructible<set<int>>::value << "\n";   // true
}

// ============================================================
// SECTION 28 : REAL LINUX SYSTEMS PROGRAMMING USE CASES
// ============================================================

// --- 28.1 Free virtual-memory region tracker with coalescing (mmap-style) ---
// Intervals are [start, end) byte ranges. set<pair<start,end>> keeps
// them sorted by start address, making it O(log n) to find ADJACENT
// free blocks to merge on free() — instead of an O(n) linear scan
// through an unsorted list.

class FreeRegionTracker
{
    set<pair<uintptr_t, uintptr_t>> freeRegions;   // sorted by start address

public:
    void addFreeRegion(uintptr_t start, uintptr_t end)
    {
        uintptr_t newStart = start;
        uintptr_t newEnd = end;

        auto it = freeRegions.lower_bound({newStart, newStart});

        // Try merging with the PRECEDING region (its end might == our start)
        if (it != freeRegions.begin())
        {
            auto prevIt = prev(it);
            if (prevIt->second == newStart)
            {
                newStart = prevIt->first;
                freeRegions.erase(prevIt);
            }
        }

        // Try merging with the FOLLOWING region (its start might == our end)
        it = freeRegions.lower_bound({newStart, newStart});
        if (it != freeRegions.end() && it->first == newEnd)
        {
            newEnd = it->second;
            freeRegions.erase(it);
        }

        freeRegions.insert({newStart, newEnd});
    }

    void printRegions() const
    {
        for (const auto& [s, e] : freeRegions)
        {
            cout << "  [" << s << ", " << e << ") size=" << (e - s) << "\n";
        }
    }
};

void section28_memory_regions_demo()
{
    FreeRegionTracker tracker;
    tracker.addFreeRegion(0x1000, 0x2000);
    tracker.addFreeRegion(0x3000, 0x4000);
    cout << "Before coalescing:\n";
    tracker.printRegions();          // two separate regions

    tracker.addFreeRegion(0x2000, 0x3000);   // fills the gap between them
    cout << "After adding the gap (should coalesce into ONE region):\n";
    tracker.printRegions();          // single region [0x1000, 0x4000)
}

// --- 28.2 Sorted PID tracking for RANGE queries (contrast with unordered_set) ---
// Your Unordered_set_Explanation.cpp Section 28 tracked PIDs with
// unordered_set for O(1) existence checks. If you ALSO need "which
// known PIDs fall in [1000, 2000]?" or "next known PID after 1500?",
// unordered_set cannot do this in better than O(n) — set answers both
// in O(log n + k).

void section28_pid_range_demo()
{
    set<pid_t> knownPids = {101, 205, 340, 512, 890, 1200, 1800, 2500};

    cout << "PIDs in [500, 1500]: ";
    for (auto it = knownPids.lower_bound(500); it != knownPids.upper_bound(1500); ++it)
    {
        cout << *it << " ";   // 512 890 1200
    }
    cout << "\n";

    auto it = knownPids.upper_bound(1000);
    if (it != knownPids.end())
    {
        cout << "Next known PID after 1000: " << *it << "\n";   // 1200
    }
}

// --- 28.3 Lowest-available-fd allocator (mirrors POSIX open()/dup() semantics) ---
// POSIX guarantees open()/dup() always return the LOWEST-numbered
// available file descriptor. A sorted set of in-use fds lets you find
// the first GAP by walking from the front until a value stops matching
// its expected sequential position.

int lowestAvailableFd(const set<int>& inUse)
{
    int expected = 0;
    for (int fd : inUse)
    {
        if (fd != expected)
        {
            return expected;    // found a gap
        }
        ++expected;
    }
    return expected;   // no gaps — next fd is right after the highest one in use
}

void section28_fd_allocator_demo()
{
    set<int> inUseFds = {0, 1, 2, 4, 5};   // fd 3 was closed — there's a gap

    cout << "Lowest available fd: " << lowestAvailableFd(inUseFds) << "\n";   // 3

    inUseFds.insert(3);
    cout << "After allocating fd 3, lowest available: "
         << lowestAvailableFd(inUseFds) << "\n";   // 6

    // NOTE: this walk is O(k) where k is the position of the first gap —
    // worst case O(n) if fds are fully contiguous. A genuinely O(log n)
    // version needs an ORDER-STATISTICS tree (Section 16's GNU PBDS
    // ordered_set) using find_by_order()/order_of_key() to binary-search
    // for the first index where "value != index" — plain std::set
    // cannot do this in better than O(n) on its own.
}

// ============================================================
// SECTION 29 : MATHEMATICAL COMPLEXITY — TREE HEIGHT & ROTATIONS
// ============================================================
//
// HEIGHT BOUND DERIVATION (from the 5 red-black properties in Section 2):
//
//   Let bh(x) = "black-height" of node x = number of BLACK nodes on
//   any path from x down to a leaf, not counting x itself.
//
//   Property 4 (no two reds in a row) means red nodes can never
//   exceed half the nodes on any root-to-leaf path. Property 5 (equal
//   black-height on every path) means every path has EXACTLY the same
//   number of black nodes, b.
//
//   Shortest possible path: all-black, length b.
//   Longest possible path: alternating red-black, length at most 2b.
//
//       height(tree) <= 2 * black-height(root)
//
//   A standard counting argument (a subtree with black-height b has
//   at least 2^b - 1 internal nodes) gives:
//
//       n >= 2^bh(root) - 1   =>   bh(root) <= log2(n + 1)
//
//   Substituting:
//
//       height(tree) <= 2 * log2(n + 1)
//
//   THIS is the precise, provable bound that guarantees set's O(log n)
//   WORST CASE — not an average, a mathematically forced ceiling.
//
// ROTATION COUNT — more precise than "O(log n) rebalancing":
//
//   Finding the insertion point costs O(log n) comparisons. But the
//   REBALANCING FIXUP after that insertion needs AT MOST 2 ROTATIONS —
//   not O(log n) rotations. The fixup can require O(log n) RECOLORING
//   steps (flipping red/black up the tree), but recoloring is O(1)
//   work per node and never restructures the tree's shape — only the
//   final step, if needed at all, performs 1-2 actual rotations.
//   Deletion needs at most 3 rotations.
//
//   This is a well-known, precise fact (see CLRS Ch. 13) worth stating
//   in a senior interview: red-black trees are favored over AVL trees
//   in most STL implementations BECAUSE their rebalancing is cheaper
//   in the worst case — AVL trees are more rigidly balanced (shorter,
//   faster lookups) but can require O(log n) rotations on a single
//   deletion.

void section29_height_benchmark()
{
    // Empirical demonstration: lookup time should grow by roughly a
    // CONSTANT increment every time n DOUBLES — direct evidence of
    // O(log n) growth, same style of proof used for binary search
    // earlier in this conversation.
    for (int n = 100'000; n <= 3'200'000; n *= 2)
    {
        set<int> s;
        for (int i = 0; i < n; ++i)
        {
            s.insert(s.end(), i);   // ascending hint — O(1) amortized build
        }

        auto t0 = chrono::steady_clock::now();
        int found = 0;
        for (int i = 0; i < 10'000; ++i)
        {
            found += s.count(i * (n / 10'000));
        }
        auto t1 = chrono::steady_clock::now();

        auto us = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        cout << "n=" << n << "  10K lookups took " << us << "us"
             << "  (found=" << found << ")\n";
    }
    // Expect the printed 'us' to grow by roughly the SAME small additive
    // amount each time n doubles — NOT double itself — exactly what
    // O(log n) per-lookup growth looks like empirically.
}

// ============================================================
// SECTION 30 : COMPILER WARNINGS & STATIC ANALYSIS
// ============================================================
//
// Compile with:
//   g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//       -Wpedantic -fsanitize=address,undefined Set_Explanation.cpp
//
//  1. "no match for operator<" — custom struct has no operator< AND
//     no Compare functor was supplied. Fix: define operator<, or pass
//     an explicit comparator as the second template argument.
//
//  2. "invalid conversion... discards qualifiers" on *it = x; — set
//     elements are always const through iterators. Erase and
//     re-insert instead.
//
//  3. clang-tidy: modernize-use-emplace — s.insert(Foo(a,b)) should
//     become s.emplace(a,b) to avoid a temporary.
//
//  4. cppcheck/clang-tidy: "possible strict-weak-ordering violation" —
//     a comparator using <= or >= instead of strict < or >. This is
//     UB, not a style nitpick — fix it immediately.
//
//  5. Never call std::find()/std::lower_bound() (the FREE algorithms)
//     on a set's iterators — they compile silently and degrade to
//     O(n). No compiler warning catches this; it's a code-review catch.
//
//  6. cppcheck: "iterator may be end(), dereferenced without check" —
//     always guard: if (it != s.end()) before *it.

// ============================================================
// SECTION 31 : CONSOLIDATED PITFALLS AND FIXES
// ============================================================
//
//  1. Using std::find()/std::lower_bound() (free algorithms) instead
//     of the MEMBER functions — silently degrades O(log n) to O(n).
//  2. Writing a comparator with <= or >= — violates strict weak
//     ordering, causes SILENT tree corruption, not a clean crash.
//  3. Assuming set has operator[] — it does not. Use insert()/
//     emplace() instead.
//  4. Modifying an element through an iterator — a compile error by
//     design. Erase the old value, insert the new one.
//  5. Forgetting the ascending-insert end()-hint optimization for
//     known sorted bulk loads — costs an unnecessary O(log n) factor.
//  6. Assuming set<T*> and a hand-written `a < b` pointer comparator
//     are equivalent — only std::less<T*> (set's DEFAULT) is
//     guaranteed to impose a total order across unrelated objects.
//  7. Expecting "k-th smallest" or "rank of X" to be fast — plain
//     std::set cannot do either in better than O(n); you need GNU PBDS
//     ordered_set (Section 16) for genuine O(log n) order statistics.
//  8. Forgetting that equal_range on a PLAIN set (not multiset) only
//     ever spans 0 or 1 elements — rarely worth preferring over find().

// ============================================================
// SECTION 32 : MODERN ALTERNATIVES TO std::set
// ============================================================
//
//  absl::btree_set (Google Abseil):
//   - A genuine B-TREE — each node holds MULTIPLE keys and children,
//     dramatically improving cache locality versus red-black's
//     one-key-per-node, pointer-chasing structure.
//   - Typically 2-3x faster than std::set for lookups/iteration in
//     real benchmarks, at the cost of slightly more memory per node.
//   - Iterator/reference STABILITY is weaker than std::set's.
//
//  boost::container::flat_set:
//   - Backed by a SORTED std::vector, not a tree at all.
//   - O(log n) lookup (binary search) but O(n) insert/erase (must
//     shift elements, like vector::insert in the middle).
//   - WINS when the set is built once (or rarely modified) and then
//     queried heavily — vector's cache locality, none of the tree's
//     pointer-chasing.
//   - LOSES badly on insert/erase-heavy workloads.
//
//  std::set still wins when:
//   - You need GUARANTEED iterator/reference stability across
//     insertions/erasures happening ANYWHERE else (Section 8.6) —
//     both alternatives above weaken this for their performance gains.
//   - Insert/erase are frequent and interleaved with lookups.
//   - You want zero extra dependencies.
//
//  RULE OF THUMB: start with std::set. Reach for btree_set only after
//  profiling shows tree pointer-chasing is a measured bottleneck;
//  reach for flat_set only when the workload is genuinely build-once,
//  query-many.

// ============================================================
// SECTION 33 : ADDITIONAL INTERVIEW QUESTIONS (ADVANCED)
// ============================================================
//
//  Q11: Why does set<pair<int,int>> work with zero setup, while
//       unordered_set<pair<int,int>> requires a custom hash functor?
//  A11: pair already provides operator< as part of the standard
//       library — ordering composes "for free" by comparing .first
//       then .second. Hashing has no equivalent free composition
//       rule — std::hash is never specialized for pair/tuple/vector,
//       so unordered_set needs an explicit combining functor before
//       it even compiles for a composite key.
//
//  Q12: Is `a < b` on two unrelated int* pointers well-defined? Does
//       it matter for set<int*>?
//  A12: Comparing pointers into DIFFERENT objects with raw `<` is
//       unspecified by the core language. It matters for set<int*>
//       because it uses std::less<int*> by default, and std::less<T*>
//       IS explicitly required to impose a total, consistent order
//       across ALL pointers of that type — precisely why you should
//       never replace it with a hand-written `a < b` lambda.
//
//  Q13: Why is set's exception safety fundamentally simpler than
//       vector's, in one sentence?
//  A13: Inserting into set only ever creates ONE new node and relinks
//       pointers — it never relocates existing elements the way
//       vector's reallocation does, so there's no "does the value
//       type have a safe move constructor" question to even ask.
//
//  Q14: Does std::set's iteration order vary between GCC, Clang, and
//       MSVC, the way unordered_set's does?
//  A14: No — set's order is a DEFINED property of the algorithm (an
//       in-order tree walk under Compare), not an implementation
//       detail of a bucket/hash policy. It's portable and reproducible
//       across every conforming implementation.
//
//  Q15: You need "k-th smallest element seen so far," updated online.
//       What are your options, worst to best?
//  A15: (1) re-sort a vector each query — O(n log n), terrible.
//       (2) std::set + distance(begin(), it) — O(k) per query (k =
//       the rank itself). (3) GNU PBDS ordered_set's find_by_order() —
//       true O(log n) per query, the correct tool for this shape.
//
//  Q16: When would boost::container::flat_set beat std::set in a real
//       benchmark, and when would it lose badly?
//  A16: Wins when data is loaded once and queried heavily — contiguous
//       memory beats pointer-chasing for reads. Loses badly once
//       insert/erase become frequent, since each costs O(n) shifting
//       versus std::set's O(log n) tree update.

// ============================================================
// ADD THESE CALLS INTO YOUR EXISTING main(), just before
// the "===== Done =====" line
// ============================================================
//
//    section22_composite_keys();
//    section22_struct_comparator();
//    section23_pointers();
//    section24_gdb_debugging_demo();
//    section25_sizeof_check();
//    section27_noexcept_check();
//    section28_memory_regions_demo();
//    section28_pid_range_demo();
//    section28_fd_allocator_demo();
//    section29_height_benchmark();
//
// (Sections 26, 30, 31, 32, 33 are reference-only — no runnable demo,
//  same as several sections in the first half of this file.)

// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//          -fsanitize=address,undefined Set_Explanation.cpp -o set_demo
// ============================================================



// ============================================================
// SECTION 34 : COMPLETE FUNCTION REFERENCE TABLE
//              Function | Description | Return value | How to use
//              (mirrors the format used in your Unordered_set_
//               Explanation.cpp Section 34, adapted for set's
//               actual member list — no bucket interface, no hash
//               policy, but WITH rbegin/rend, lower_bound/upper_bound,
//               and key_comp/value_comp in their place)
// ============================================================

// ---------------- MODIFIERS ----------------

// FUNCTION      : insert(value)
// DESCRIPTION   : Inserts a single element if no equivalent element
//                 is already present (per Compare, not operator==)
// RETURN VALUE  : pair<iterator,bool> — iterator to element,
//                 bool true if newly inserted, false if equivalent
//                 element already existed
// HOW TO USE    :
//     auto [it, ok] = s.insert(42);
//     if (ok) {
//         std::cout << "Inserted: " << *it << '\n';
//     }

// FUNCTION      : insert(hint, value)
// DESCRIPTION   : Inserts with a positional hint. UNLIKE unordered_set,
//                 a CORRECT hint here genuinely drops cost from
//                 O(log n) to amortized O(1) — the tree can skip the
//                 top-down search entirely
// RETURN VALUE  : iterator to the inserted (or existing) element
// HOW TO USE    :
//     // classic bulk-ascending-insert optimization:
//     for (int i = 0; i < 1000; ++i) {
//         s.insert(s.end(), i);   // O(1) amortized per insert
//     }

// FUNCTION      : insert(first, last)
// DESCRIPTION   : Inserts a range of elements; each is placed in
//                 its correct sorted position, duplicates dropped
// RETURN VALUE  : void
// HOW TO USE    :
//     std::vector<int> v = {3, 1, 2};
//     s.insert(v.begin(), v.end());

// FUNCTION      : insert(init_list)
// DESCRIPTION   : Inserts multiple elements from a braced list
// RETURN VALUE  : void
// HOW TO USE    :
//     s.insert({10, 20, 30});

// FUNCTION      : emplace(args...)
// DESCRIPTION   : Constructs the element in-place, avoiding a
//                 temporary object (matters for non-trivial types)
// RETURN VALUE  : pair<iterator,bool> — same semantics as insert
// HOW TO USE    :
//     auto [it, ok] = s.emplace(42);

// FUNCTION      : emplace_hint(hint, args...)
// DESCRIPTION   : Same as emplace, with a positional hint — same
//                 amortized O(1) benefit as insert(hint, value)
// RETURN VALUE  : iterator to the inserted (or existing) element
// HOW TO USE    :
//     auto it = s.emplace_hint(s.end(), 42);

// FUNCTION      : erase(key)
// DESCRIPTION   : Removes the element matching key, if present
// RETURN VALUE  : size_t — number of elements removed (0 or 1)
// HOW TO USE    :
//     size_t removed = s.erase(42);
//     if (removed) {
//         std::cout << "Removed\n";
//     }

// FUNCTION      : erase(iterator)
// DESCRIPTION   : Removes the element at the given iterator position.
//                 ONLY that iterator is invalidated — every other
//                 iterator/reference into the set remains valid
// RETURN VALUE  : iterator to the element AFTER the one removed
// HOW TO USE    :
//     auto it = s.find(42);
//     if (it != s.end()) {
//         it = s.erase(it);
//     }

// FUNCTION      : erase(first, last)
// DESCRIPTION   : Removes all elements in the range [first, last) —
//                 a genuine SORTED-RANGE erase, e.g. "erase everything
//                 between these two values"
// RETURN VALUE  : iterator following the last element removed
// HOW TO USE    :
//     s.erase(s.lower_bound(10), s.upper_bound(50));  // erase [10,50]

// FUNCTION      : clear()
// DESCRIPTION   : Removes all elements
// RETURN VALUE  : void
// HOW TO USE    :
//     s.clear();

// FUNCTION      : swap(other)
// DESCRIPTION   : Exchanges contents with another set of the same
//                 type — O(1), no element copies
// RETURN VALUE  : void
// HOW TO USE    :
//     std::set<int> a = {1, 2}, b = {3, 4};
//     a.swap(b);

// FUNCTION      : extract(key)                              [C++17]
// DESCRIPTION   : Removes the element WITHOUT destroying it,
//                 returning a movable node handle you can re-insert
//                 elsewhere with zero extra allocation
// RETURN VALUE  : node_type (empty if key was not found)
// HOW TO USE    :
//     auto node = s.extract(42);
//     if (!node.empty()) {
//         other_set.insert(std::move(node));
//     }

// FUNCTION      : merge(other)                              [C++17]
// DESCRIPTION   : Moves all elements from other into *this without
//                 reallocating. Elements already present (per
//                 Compare) stay behind in other instead of merging
// RETURN VALUE  : void
// HOW TO USE    :
//     std::set<int> a = {1, 2}, b = {2, 3};
//     a.merge(b);
//     // a = {1, 2, 3},  b = {2}   (2 stayed in b — already in a)


// ---------------- LOOKUP (all O(log n), worst case) ----------------

// FUNCTION      : find(key)
// DESCRIPTION   : Locates the element equivalent to key. ALWAYS
//                 prefer this MEMBER function over the free
//                 std::find() algorithm, which degrades to O(n)
//                 on set's bidirectional iterators
// RETURN VALUE  : iterator to element, or end() if not found
// HOW TO USE    :
//     auto it = s.find(42);
//     if (it != s.end()) {
//         std::cout << "Found: " << *it << '\n';
//     }

// FUNCTION      : count(key)
// DESCRIPTION   : Checks how many elements match key (always 0 or 1
//                 in a plain set — never more)
// RETURN VALUE  : size_t
// HOW TO USE    :
//     if (s.count(42)) {
//         std::cout << "Exists\n";
//     }

// FUNCTION      : contains(key)                             [C++20]
// DESCRIPTION   : Checks existence directly, clearest membership test
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (s.contains(42)) {
//         std::cout << "Exists\n";
//     }

// FUNCTION      : equal_range(key)
// DESCRIPTION   : Returns the sub-range of elements matching key
//                 (at most one element for a plain set — use on
//                 multiset for a genuinely multi-element range)
// RETURN VALUE  : pair<iterator,iterator>
// HOW TO USE    :
//     auto [first, last] = s.equal_range(42);
//     for (auto it = first; it != last; ++it) {
//         std::cout << *it << '\n';
//     }

// FUNCTION      : lower_bound(key)
// DESCRIPTION   : Finds the first element >= key. THIS is set's
//                 headline advantage over unordered_set — genuine
//                 O(log n) range-query support
// RETURN VALUE  : iterator
// HOW TO USE    :
//     auto it = s.lower_bound(25);
//     if (it != s.end()) {
//         std::cout << "First >= 25: " << *it << '\n';
//     }

// FUNCTION      : upper_bound(key)
// DESCRIPTION   : Finds the first element > key
// RETURN VALUE  : iterator
// HOW TO USE    :
//     auto it = s.upper_bound(25);
//     // [s.lower_bound(lo), s.upper_bound(hi)) = every element in [lo,hi]


// ---------------- CAPACITY ----------------

// FUNCTION      : size()
// DESCRIPTION   : Number of elements currently stored — O(1),
//                 guaranteed since C++11
// RETURN VALUE  : size_t
// HOW TO USE    :
//     std::cout << "Size: " << s.size() << '\n';

// FUNCTION      : empty()
// DESCRIPTION   : Checks whether the set has zero elements
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (s.empty()) {
//         std::cout << "Set is empty\n";
//     }

// FUNCTION      : max_size()
// DESCRIPTION   : Theoretical maximum number of elements the set
//                 could ever hold (allocator + system dependent)
// RETURN VALUE  : size_t
// HOW TO USE    :
//     std::cout << "Max size: " << s.max_size() << '\n';

// NOTE: set has NO reserve() and NO capacity() — see Section 35.1
//       for why this is a structural fact, not an oversight.


// ---------------- ITERATORS ----------------

// FUNCTION      : begin() / end()
// DESCRIPTION   : Iterate in ASCENDING order (or Compare-defined
//                 order) — this is DEFINED behavior, not arbitrary
//                 the way unordered_set's iteration order is
// RETURN VALUE  : iterator (BidirectionalIterator — no random access)
// HOW TO USE    :
//     for (auto it = s.begin(); it != s.end(); ++it) {
//         std::cout << *it << ' ';
//     }

// FUNCTION      : rbegin() / rend()
// DESCRIPTION   : Iterate in DESCENDING order. Only possible because
//                 set has a well-defined order to reverse in the
//                 first place — unordered_set has no equivalent
// RETURN VALUE  : reverse_iterator
// HOW TO USE    :
//     for (auto it = s.rbegin(); it != s.rend(); ++it) {
//         std::cout << *it << ' ';
//     }

// FUNCTION      : cbegin() / cend(), crbegin() / crend()
// DESCRIPTION   : Same as above but explicitly const
// RETURN VALUE  : const_iterator / const_reverse_iterator
// HOW TO USE    :
//     for (auto it = s.cbegin(); it != s.cend(); ++it) {
//         std::cout << *it << ' ';
//     }


// ---------------- OBSERVERS ----------------

// FUNCTION      : key_comp()
// DESCRIPTION   : Returns a copy of the Compare functor used
//                 internally to order elements
// RETURN VALUE  : Compare (e.g. std::less<int>)
// HOW TO USE    :
//     auto cmp = s.key_comp();
//     std::cout << cmp(1, 2) << '\n';   // true

// FUNCTION      : value_comp()
// DESCRIPTION   : For set specifically, IDENTICAL to key_comp() —
//                 the "value" and the "key" are the same thing here
//                 (this differs from map, where value_comp compares
//                 pair<Key,Value> by Key only)
// RETURN VALUE  : Compare
// HOW TO USE    :
//     auto vcmp = s.value_comp();

// FUNCTION      : get_allocator()
// DESCRIPTION   : Returns a copy of the allocator used internally
// RETURN VALUE  : Allocator (e.g. std::allocator<int>)
// HOW TO USE    :
//     auto alloc = s.get_allocator();


// ---------------- NON-MEMBER / FREE FUNCTIONS ----------------

// FUNCTION      : operator== / != / < / <= / > / >=
// DESCRIPTION   : LEXICOGRAPHIC comparison of two sets' CONTENTS in
//                 sorted order — a genuinely different result from
//                 unordered_set's == (which is content-equality with
//                 NO notion of <, since there's no defined order to
//                 compare against). This is what makes set<set<T>>
//                 possible at all — see Section 35.3
// RETURN VALUE  : bool
// HOW TO USE    :
//     std::set<int> a = {1, 2, 3};
//     std::set<int> b = {1, 2, 4};
//     std::cout << (a < b) << '\n';   // true — element-by-element,
//                                     // first difference is 3 < 4

// FUNCTION      : std::erase_if(set, pred)                   [C++20]
// DESCRIPTION   : Removes every element for which pred returns true —
//                 replaces the manual erase-in-a-loop idiom
// RETURN VALUE  : size_t — number of elements erased
// HOW TO USE    :
//     std::erase_if(s, [](int x) { return x % 2 == 0; });

// ============================================================
// END OF SECTION 34
// ============================================================


// ============================================================
// SECTION 35 : FACTS NOT YET COVERED
// ============================================================

// --- 35.1 : Why set has NO reserve() and NO capacity() ---
//
// reserve()/capacity() are meaningful ONLY for containers with a
// notion of "pre-allocated but unused space" — vector (contiguous
// buffer with slack), unordered_set (bucket array sized ahead of
// need). set has neither: every element gets its OWN individually
// heap-allocated node the moment it's inserted. There is no
// "buffer" to pre-size — the closest analogous concept, allocator
// pooling, is something YOU would layer on top via a custom
// allocator (Section 15's TIP pattern, borrowed from your vector
// bank's PMR example), not something the container tracks itself.
//
// PRACTICAL IMPLICATION: unlike vector or unordered_set, there is
// NO equivalent of "call this one function before a bulk insert to
// avoid repeated reallocation." The closest lever you actually have
// is the end()-hint trick (Section 4.2 / Section 15 TIP 1) — it
// doesn't pre-size anything, it just skips the O(log n) search
// portion of each individual insert.

// --- 35.2 : set does NOT get C++17's incomplete-type support ---
//
// C++17 (via paper N4510) specifically extended std::vector,
// std::list, and std::forward_list to allow an INCOMPLETE type as
// their element type, as long as the type is complete by the time
// certain member functions are actually instantiated — this enables
// patterns like a self-referential struct holding a
// vector<SelfType> as a member, without needing a pointer/unique_ptr
// indirection.
//
// std::set, std::map, std::unordered_set, and std::unordered_map
// were NOT included in that extension. Using an incomplete type as
// a set's Key is NOT standard-guaranteed to work, even though a
// given implementation might happen to compile it in some cases.
// This is a genuinely easy fact to get wrong if you only remember
// "C++17 added incomplete type support to containers" without the
// specific list of which three containers that applied to.

struct Node
{
    int value;
    // std::vector<Node> children;   // OK since C++17 (N4510) — vector<Node> as a
                                     // member of Node itself, self-referential
    // std::set<Node> siblings;      // NOT guaranteed by the standard to work —
                                     // set/map were excluded from N4510's extension
};

// --- 35.3 : set<set<T>> and nested ordered containers — a genuine "aha" ---
//
// Every standard container that provides begin()/end() and whose
// element type supports the relevant comparisons ALSO gets its OWN
// full set of relational operators (==, !=, <, <=, >, >=), performing
// a LEXICOGRAPHIC comparison over its own contents in iteration
// order — exactly the same rule that gives pair/tuple/vector their
// built-in operator< (Section 22). Since set<T> itself qualifies,
// set<set<T>> "just works" with ZERO extra code — the outer set uses
// each inner set's own content-based ordering to decide equivalence
// and position.

void section35_nested_sets()
{
    set<set<int>> collection;

    collection.insert({1, 2, 3});
    collection.insert({4, 5});
    collection.insert({1, 2, 3});   // SAME CONTENT as the first insert -> not
                                    // added again, because set<int>::operator
                                    // and operator== both see them as equivalent
    collection.insert({1, 2});     // different content -> distinct entry

    cout << "collection size: " << collection.size() << "\n";   // 3

    cout << "Sorted order of inner sets:\n";
    for (const auto& innerSet : collection)
    {
        cout << "  { ";
        for (int x : innerSet)
        {
            cout << x << " ";
        }
        cout << "}\n";
    }
    // Output order: {1 2} then {1 2 3} then {4 5} — lexicographic,
    // shorter-but-equal-prefix sorts first, exactly like Section 22's
    // set<vector<int>> example.

    // CONTRAST: unordered_set<unordered_set<int>> does NOT compile at
    // all without first writing a custom hash functor for the OUTER
    // unordered_set — there is no std::hash<unordered_set<T>>, and
    // there never will be, because hash tables have no defined
    // iteration order to hash consistently against. Ordered nesting
    // is fundamentally easier than hashed nesting for exactly this
    // reason.
}

// --- 35.4 : set has NO SSO-style optimization, and structurally never can ---
//
// Recall from your std::string notes: Small String Optimization
// stores short strings INLINE inside the string object itself,
// avoiding a heap allocation entirely for strings under the SSO
// threshold. There is NO analogous optimization for std::set, for a
// structural reason: a tree's shape is defined ENTIRELY by pointers
// between individually-allocated nodes. There is no "small number of
// elements" special case where those pointers could instead point
// into inline storage within the set object itself — the moment you
// have 2+ elements, you need distinct, independently addressable node
// objects for the tree structure (left/right/parent pointers) to
// mean anything at all. This is a genuine, structural limitation, not
// a missed optimization opportunity — it's WHY flat_set (Section 32,
// backed by a plain sorted vector) exists as a different container
// entirely for workloads where this matters.

// ============================================================
// END OF SECTION 35
// ============================================================


// ============================================================
// SECTION 36 : PRACTICE PROBLEM BANK — BY PATTERN,
//              ACROSS ALL 5 PLATFORMS
// (LC = LeetCode, GFG = GeeksforGeeks, STV = Striver A2Z/SDE Sheet,
//  CN = Coding Ninjas Studio, NKR = Naukri Coding 360)
// ============================================================

// --- Pattern 1 — Predecessor / Successor & Range Queries ---
//  Platform | Problem                                        | Difficulty
//  LC       | #220 Contains Duplicate III                    | Hard
//  LC       | #1146 Snapshot Array                            | Medium
//  LC       | #911 Online Election                            | Medium
//  GFG      | "Find Floor and Ceil of a Number in a Set"      | Easy
//  GFG      | "Closest number in an array/set to a given value" | Easy
//  STV      | "Ceiling and Floor using set"                    | Medium
//  CN       | "Closest Number Problems"                        | Easy
//  NKR      | Predecessor/successor practice set                | Medium

// --- Pattern 2 — Interval Scheduling / Calendar-Style Problems ---
//  Platform | Problem                                        | Difficulty
//  LC       | #729 My Calendar I                              | Medium
//  LC       | #731 My Calendar II                             | Medium
//  LC       | #732 My Calendar III                            | Hard
//  LC       | #715 Range Module                               | Hard
//  LC       | #759 Employee Free Time                         | Hard
//  GFG      | "Merge Overlapping Intervals"                    | Medium
//  STV      | "Interval Scheduling using Set"                  | Hard
//  CN       | "Calendar Booking Problems"                       | Medium

// --- Pattern 3 — Multiset-Based Median / Order Statistics ---
//  Platform | Problem                                        | Difficulty
//  LC       | #295 Find Median from Data Stream                | Hard
//  LC       | #480 Sliding Window Median                       | Hard
//  LC       | #1834 Single-Threaded CPU                        | Medium
//  LC       | #1801 Number of Orders in the Backlog             | Medium
//  GFG      | "Order Statistics Tree using PBDS"                | Hard
//  STV      | "Running Median" Day                              | Hard
//  CN       | "Median in a Data Stream"                          | Hard

// --- Pattern 4 — Sorted Deduplication & Set Algorithms ---
//  Platform | Problem                                        | Difficulty
//  LC       | #349 Intersection of Two Arrays                  | Easy
//  LC       | #128 Longest Consecutive Sequence (set variant)  | Medium
//  LC       | #217 Contains Duplicate                          | Easy
//  GFG      | "Union and Intersection of Two Sorted Sets"       | Easy
//  STV      | "Set Operations" Day                              | Easy
//  CN       | "Deduplication using Set"                          | Easy

// --- Pattern 5 — Advanced / Mixed ---
//  Platform | Problem                                        | Difficulty
//  LC       | #855 Exam Room                                  | Medium
//  LC       | #1157 Online Majority Element In Subarray        | Hard
//  LC       | #1943 Describe the Painting                      | Hard
//  LC       | #352 Data Stream as Disjoint Intervals            | Hard
//  GFG      | "Design a Data Structure using Set"               | Medium
//  CN       | "Exam Room Design"                                 | Medium
//  NKR      | Advanced set/multiset assignment set               | Hard

// ============================================================
// END OF SECTION 36
// ============================================================


// ============================================================
// SECTION 37 : FINAL QUICK-REFERENCE CHEAT SHEET
// ============================================================
//
// Structure                -> red-black tree, height <= 2*log2(n+1)
// Complexity                -> O(log n) WORST CASE for insert/erase/find
//                              (contrast: unordered_set's O(1) is AVERAGE only)
// Ordering requirement      -> Compare must be a STRICT WEAK ORDERING
//                              (never <=, never >=, always strict < or >)
// operator[]                -> DOES NOT EXIST on set at all — use insert/emplace
// Modifying an element       -> compile error by design — erase old, insert new
// find()/lower_bound()       -> ALWAYS use the MEMBER function, never the free
//                              std::find()/std::lower_bound() algorithm on a
//                              set's iterators — the free version silently
//                              degrades to O(n)
// Insert-with-hint           -> genuinely useful here (unlike unordered_set) —
//                              s.insert(s.end(), x) for ascending bulk loads
//                              turns O(n log n) into O(n)
// rbegin()/rend()            -> exists because set HAS a defined order to
//                              reverse; unordered_set has none
// Range queries               -> s.lower_bound(lo) .. s.upper_bound(hi) — THE
//                              headline reason to pick set over unordered_set
// k-th smallest / rank        -> plain set CANNOT do this in better than O(n) —
//                              need GNU PBDS ordered_set (Section 16) for
//                              genuine O(log n) order statistics
// reserve()/capacity()        -> DO NOT EXIST — every node is individually
//                              heap-allocated on demand, nothing to pre-size
// Incomplete type support     -> NOT extended to set/map in C++17 (only
//                              vector/list/forward_list got that, per N4510)
// Nested containers           -> set<set<T>>, set<vector<T>>, set<pair<T,T>>
//                              all work with ZERO extra code — built-in
//                              lexicographic operator< on every std container
// No SSO-equivalent            -> structurally impossible; tree nodes must be
//                              independently addressable for pointers to work
// Exception safety             -> STRONGER than vector's by construction —
//                              insert only ever touches ONE new node, existing
//                              elements are never relocated in memory
// Iterator stability            -> STRONGEST guarantee among ordered/hashed STL
//                              containers — insert invalidates nothing; erase
//                              invalidates ONLY the erased element's iterator
// set vs unordered_set          -> sorted iteration + range queries + worst-case
//                              guarantee + easy composite keys  vs  raw average
//                              speed + lower per-element setup for simple keys
// set vs flat_set (Boost)        -> set wins on insert/erase-heavy + iterator
//                              stability; flat_set wins on build-once-query-many

// ============================================================
// ADD THIS CALL INTO YOUR EXISTING main(), just before
// the "===== Done =====" line
// ============================================================
//
//    section35_nested_sets();
//
// (Sections 34, 36, 37 are reference-only — tables and a cheat
//  sheet, no runnable demo, matching the style of Sections 2, 11,
//  15, 16, 19, 26, 30, 31, 32, 33 earlier in this file.)

// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//          -fsanitize=address,undefined Set_Explanation.cpp -o set_demo
// ============================================================

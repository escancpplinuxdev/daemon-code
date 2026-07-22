// ============================================================
//  FILE  : Unordered_set_Explanation.cpp
//  TOPIC : std::unordered_set — Complete Reference
//  LEVEL : Intermediate to Advanced (C++ Linux Programmer)
//  STD   : C++11 and later
// ============================================================

#include <unordered_set>
#include <iostream>
#include <string>
#include <vector>
#include <functional>   // std::hash
#include <algorithm>    // std::for_each
using namespace std;

// ============================================================
// SECTION 1 : WHAT IS unordered_set ?
// ============================================================
//
//  std::unordered_set<T> is an ASSOCIATIVE CONTAINER that:
//
//   - Stores UNIQUE elements (no duplicates allowed)
//   - Has NO defined order  (elements appear in arbitrary order)
//   - Provides AVERAGE O(1) for insert / erase / find
//   - Uses a HASH TABLE internally (not a tree like std::set)
//
//  Header required:
//   #include <unordered_set>
//
//  Declared in namespace std.
//
//  Full template signature:
//   template
//       class Key,
//       class Hash      = std::hash<Key>,
//       class KeyEqual  = std::equal_to<Key>,
//       class Allocator = std::allocator<Key>
//   > class unordered_set;
//
//  Parameters:
//   Key       — type of stored elements
//   Hash      — hash function for Key  (default: std::hash<Key>)
//   KeyEqual  — equality comparator    (default: std::equal_to<Key>)
//   Allocator — memory allocator       (default: std::allocator<Key>)

// ============================================================
// SECTION 2 : INTERNAL IMPLEMENTATION (HASH TABLE)
// ============================================================
//
//  STRUCTURE:
//   - Array of "buckets" (each bucket is a linked list of nodes)
//   - bucket_count() = number of buckets in the array
//   - Each element is stored in bucket index = hash(key) % bucket_count
//
//  VISUAL LAYOUT:
//
//   Bucket[0] → node(42) → nullptr
//   Bucket[1] → nullptr
//   Bucket[2] → node(17) → node(97) → nullptr   ← collision!
//   Bucket[3] → node(5)  → nullptr
//   Bucket[4] → nullptr
//   ...
//
//  COLLISION:
//   When two different keys hash to the same bucket index.
//   GCC libstdc++ handles collisions by CHAINING (linked list per bucket).
//   Clang libc++ uses the same approach.
//
//  LOAD FACTOR:
//   load_factor() = size() / bucket_count()
//   Measures average number of elements per bucket.
//   Default max_load_factor() = 1.0
//
//  REHASH (RESIZE):
//   When load_factor() > max_load_factor(), the table grows:
//     1. Allocate new bucket array (typically 2x bucket_count)
//     2. Re-hash ALL existing elements into new buckets  ← O(n)
//     3. Deallocate old bucket array
//   All iterators are invalidated during rehash.
//
//  TIME COMPLEXITIES:
//   Operation       Average     Worst Case
//   ----------      -------     ----------
//   insert          O(1)        O(n)   ← worst: all keys same bucket
//   erase           O(1)        O(n)
//   find/count      O(1)        O(n)
//   clear           O(n)        O(n)
//   begin/end       O(1)        O(1)
//   size            O(1)        O(1)
//   bucket_count    O(1)        O(1)
//
//  SPACE COMPLEXITY:
//   O(n) where n = number of elements stored.
//   Actual memory = bucket_array + node_allocations
//   Memory overhead higher than std::vector but gives O(1) lookup.
//
//  IMPORTANT:
//   Bad hash function (e.g., always returns 0) degrades
//   ALL operations from O(1) to O(n) because all elements
//   land in the same bucket → linear search per bucket.

// ============================================================
// SECTION 3 : DECLARATION AND INITIALIZATION
// ============================================================

void section3_declaration()
{
	// --- 3.1 Default constructor (empty set) ---
	unordered_set<int>    us1;          // empty, int keys
	unordered_set<string> us2;          // empty, string keys

	// --- 3.2 With initial bucket count hint ---
	// Avoids rehashing if you know approximate element count
	unordered_set<int> us3(100);        // at least 100 buckets pre-allocated

	// --- 3.3 Initializer list ---
	unordered_set<int> us4 = {10, 20, 30, 40, 50};
	unordered_set<int> us5 {1, 2, 3, 4, 5};         // same thing

	// --- 3.4 Copy constructor ---
	unordered_set<int> us6 = us4;       // deep copy

	// --- 3.5 Move constructor ---
	unordered_set<int> us7 = move(us5); // steals us5's internal buffer

	// --- 3.6 Range constructor (from any iteratable) ---
	vector<int> v = {5, 10, 15, 10, 5};  // duplicates will be removed!
	unordered_set<int> us8(v.begin(), v.end()); // us8 = {5, 10, 15}

// --- 3.7 With custom initial bucket count AND max_load_factor ---
	unordered_set<int> us9(200, hash<int>(), equal_to<int>());

// --- 3.8 Assignment ---
	us1 = us4;           // copy assignment

//    us2 = move(us4);     // move assignment //error: no match for ‘operator=’ (operand types are int to std::string)
//    us2 = std::move(us4);     // move assignment

// Convert ints to strings and insert for move assignment
for (int i : us4) 
{
	us2.insert(std::to_string(i));
}



cout << "us8 size: " << us8.size() << "\n";  // 3 (duplicates removed)
}

// ============================================================
// SECTION 4 : INSERTING ELEMENTS
// ============================================================

void section4_insert()
{
	unordered_set<int> us;

	// --- 4.1 insert(value) ---
	// Returns: pair<iterator, bool>
	//   iterator → points to inserted element (or existing one)
	//   bool     → true if inserted, false if already existed
	auto [it1, ok1] = us.insert(10);   // ok1 = true  (new element)
	auto [it2, ok2] = us.insert(10);   // ok2 = false (already exists)
	cout << *it1 << " " << ok1 << "\n"; // 10 1
	cout << *it2 << " " << ok2 << "\n"; // 10 0

	// --- 4.2 insert with hint (iterator hint for performance) ---
	// Hint is advisory only for unordered containers (unlike ordered)
	// Effectively the same as insert(value) — hint ignored in practice
	auto it_hint = us.begin();
	us.insert(it_hint, 20);            // returns iterator to 20

	// --- 4.3 insert range ---
	vector<int> v = {30, 40, 50, 40}; // 40 is duplicate
	us.insert(v.begin(), v.end());     // inserts 30, 40, 50 (40 once)

	// --- 4.4 insert initializer list ---
	us.insert({60, 70, 80, 60});       // 60 inserted once

	// --- 4.5 emplace(args...) ---
	// Constructs element IN-PLACE — avoids copy/move of a temp object
	// For int this is same as insert, but for structs it matters
	auto [it3, ok3] = us.emplace(90);  // returns pair<iterator,bool>

	// --- 4.6 emplace_hint ---
	us.emplace_hint(us.begin(), 100);  // hint + in-place construction

	cout << "Size after inserts: " << us.size() << "\n";

	// NOTE: After ANY insert that triggers rehash,
	//       ALL existing iterators are INVALIDATED.
	//       Do not store iterators across insert calls on large sets.
}

// ============================================================
// SECTION 5 : ACCESSING / LOOKING UP ELEMENTS
// ============================================================

void section5_lookup()
{
	unordered_set<int> us = {10, 20, 30, 40, 50};

	// --- 5.1 find(key) ---
	// Returns: iterator to element if found, us.end() if not found
	// Average O(1)
	auto it = us.find(30);
	if (it != us.end())
		cout << "Found: " << *it << "\n";   // Found: 30
	else
		cout << "Not found\n";

	auto it2 = us.find(99);
	if (it2 == us.end())
		cout << "99 not in set\n";           // 99 not in set

	// --- 5.2 count(key) ---
	// Returns 0 or 1 (unordered_set has no duplicates)
	// Use for existence check — slightly simpler than find()
	cout << us.count(20) << "\n";   // 1 (exists)
	cout << us.count(99) << "\n";   // 0 (not exists)

	// IDIOM: existence check
	if (us.count(40))
		cout << "40 exists\n";

	// --- 5.3 contains(key) [C++20] ---
	// Clearest way to check existence
	// Returns bool directly
#if __cplusplus >= 202002L
	if (us.contains(50))
		cout << "50 exists (C++20)\n";
#endif

	// --- 5.4 equal_range(key) ---
	// Returns pair<iterator, iterator>
	// For unordered_set: at most 1 element in range
	// [first, second) range contains at most 1 element
	auto [first, last] = us.equal_range(30);
	for (auto it = first; it != last; ++it)
		cout << "equal_range: " << *it << "\n";  // equal_range: 30

	// NOTE: Unlike std::set, you CANNOT use operator[] on unordered_set.
	//       unordered_set only has find/count/contains for lookup.
	//       There is no way to get a reference to an element
	//       for modification (because modification would change hash).
}

// ============================================================
// SECTION 6 : ERASING ELEMENTS
// ============================================================

void section6_erase()
{
	unordered_set<int> us = {10, 20, 30, 40, 50};

	// --- 6.1 erase by VALUE ---
	// Returns: number of elements removed (0 or 1 for unordered_set)
	size_t removed = us.erase(30);          // removes 30
	cout << "Removed: " << removed << "\n"; // 1
	removed = us.erase(99);                 // 99 not in set
	cout << "Removed: " << removed << "\n"; // 0

	// --- 6.2 erase by ITERATOR ---
	// Returns: iterator to element AFTER the erased one
	auto it = us.find(20);
	if (it != us.end())
	{
		auto next_it = us.erase(it);  // erases 20, returns next iterator
					      // 'it' is now INVALIDATED — do not use it after erase
	}

	// --- 6.3 erase RANGE ---
	// Returns: iterator to element after last erased
	// WARNING: erasing a range in unordered_set erases ALL elements
	//          between first and last in iteration order (arbitrary order!)
	unordered_set<int> us2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	us2.erase(us2.begin(), us2.end());  // clear-like: removes all
	cout << "us2 size after range erase: " << us2.size() << "\n"; // 0

	// --- 6.4 clear() ---
	// Removes ALL elements. Keeps bucket count unchanged.
	// O(n) — calls destructor on each element
	us.clear();
	cout << "us empty: " << us.empty() << "\n";  // 1

	// IMPORTANT: erase by value is the safest and most common.
	//            Erase by iterator: 'it' is invalid after the call.
	//            Erase during iteration: use the returned iterator.

	// SAFE PATTERN: erase elements matching a condition during iteration
	unordered_set<int> us3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	for (auto it = us3.begin(); it != us3.end(); )
	{
		if (*it % 2 == 0)
			it = us3.erase(it);   // erase returns next valid iterator
		else
			++it;                  // move to next
	}
	// us3 now contains only odd numbers
	for (int x : us3)
		cout << x << " ";         // 1 3 5 7 9 (arbitrary order)
	cout << "\n";

	// C++20 alternative: std::erase_if
	// std::erase_if(us3, [](int x){ return x % 2 == 0; });
}

// ============================================================
// SECTION 7 : CAPACITY FUNCTIONS
// ============================================================

void section7_capacity()
{
	unordered_set<int> us = {1, 2, 3, 4, 5};

	// --- 7.1 size() ---
	// Number of elements currently stored
	cout << us.size() << "\n";           // 5

	// --- 7.2 empty() ---
	// Returns true if size() == 0
	cout << us.empty() << "\n";          // 0 (false)

	// --- 7.3 max_size() ---
	// Maximum number of elements the container can ever hold
	// Depends on allocator + system memory
	cout << us.max_size() << "\n";       // very large number

	// NOTE: size() is O(1) — tracked internally.
	//       Never use count(begin,end) to get size — that's O(n).
}

// ============================================================
// SECTION 8 : ITERATORS
// ============================================================

void section8_iterators()
{
	unordered_set<int> us = {10, 20, 30, 40, 50};

	// unordered_set provides:
	//   iterator         — forward iterator (read-only in unordered_set)
	//   const_iterator   — const forward iterator
	//   local_iterator   — iterator within a single bucket
	//   const_local_iterator

	// NOTE: unordered_set does NOT provide:
	//   reverse_iterator (rbegin/rend) — no defined order
	//   random access — cannot do it[3]

	// --- 8.1 begin() / end() ---
	cout << "All elements: ";
	for (auto it = us.begin(); it != us.end(); ++it)
		cout << *it << " ";              // arbitrary order
	cout << "\n";

	// --- 8.2 cbegin() / cend() (const iterators) ---
	for (auto it = us.cbegin(); it != us.cend(); ++it)
		cout << *it << " ";
	cout << "\n";

	// --- 8.3 Range-based for (most common) ---
	for (const int& x : us)
		cout << x << " ";
	cout << "\n";

	// --- 8.4 Elements are ALWAYS read-only through iterators ---
	// You CANNOT modify an element through an iterator.
	// Reason: modifying an element could change its hash value,
	// corrupting the hash table structure.
	//
	// This does NOT compile:
	//   auto it = us.begin();
	//   *it = 999;    // ERROR: expression is not assignable
	//
	// To "change" an element: erase old, insert new.
	auto it = us.find(20);
	if (it != us.end())
	{
		us.erase(it);    // remove 20
		us.insert(200);  // insert 200
	}

	// --- 8.5 Iterator invalidation rules ---
	// INVALIDATED when:
	//   - rehash occurs (any insert that exceeds load factor)
	//   - reserve() or rehash() is called
	//   - erase() on that specific element
	//
	// NOT invalidated by:
	//   - erase() on OTHER elements (other iterators remain valid)
	//   - Any operation that does NOT cause rehash
}

// ============================================================
// SECTION 9 : HASH POLICY (CRITICAL FOR PERFORMANCE)
// ============================================================

void section9_hash_policy()
{
	unordered_set<int> us;

	// --- 9.1 bucket_count() ---
	// Current number of buckets in the hash table
	cout << "Initial bucket_count: " << us.bucket_count() << "\n";  // impl-defined

	// --- 9.2 load_factor() ---
	// load_factor = size() / bucket_count()
	// Measures average elements per bucket
	us = {1, 2, 3, 4, 5};
	cout << "load_factor: " << us.load_factor() << "\n";

	// --- 9.3 max_load_factor() ---
	// Threshold that triggers rehash when exceeded.
	// Default is 1.0 (GCC and Clang)
	cout << "max_load_factor: " << us.max_load_factor() << "\n";   // 1.0

	// You CAN set it lower (more memory, fewer collisions)
	// or higher (less memory, more collisions)
	us.max_load_factor(0.5f);  // rehash sooner — better perf, more memory
	us.max_load_factor(2.0f);  // rehash later  — worse perf, less memory

	// --- 9.4 rehash(n) ---
	// Forces bucket_count to be at least n.
	// Triggers a rehash of all existing elements.
	// Use to PRE-SIZE the table before bulk inserts.
	// After rehash: bucket_count >= n AND load_factor <= max_load_factor
	us.rehash(1000);
	cout << "After rehash(1000): " << us.bucket_count() << "\n"; // >= 1000

	// --- 9.5 reserve(n) ---
	// Pre-sizes so that n elements can be inserted WITHOUT triggering rehash.
	// Internally calls: rehash(ceil(n / max_load_factor()))
	// BEST PRACTICE: call reserve() before bulk inserts
	unordered_set<int> us2;
	us2.reserve(10000);   // no rehash until 10000 elements inserted
	for (int i = 0; i < 10000; ++i)
		us2.insert(i);    // zero reallocations!

	// WITHOUT reserve: log2(10000) ≈ 13 rehashes during bulk insert
	// WITH reserve:    0 rehashes — significantly faster

	// --- 9.6 bucket(key) ---
	// Returns the bucket index for a given key
	unordered_set<int> us3 = {10, 20, 30};
	cout << "Bucket for 10: " << us3.bucket(10) << "\n";
	cout << "Bucket for 20: " << us3.bucket(20) << "\n";

	// --- 9.7 bucket_size(n) ---
	// Number of elements in bucket n
	// High bucket_size means hash collisions — performance degrades
	for (size_t i = 0; i < us3.bucket_count(); ++i)
	{
		if (us3.bucket_size(i) > 0)
			cout << "Bucket " << i << " has " << us3.bucket_size(i) << " element(s)\n";
	}

	// --- 9.8 Local iterators (iterate within a single bucket) ---
	unordered_set<int> us4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	for (size_t i = 0; i < us4.bucket_count(); ++i)
	{
		if (us4.bucket_size(i) > 0)
		{
			cout << "Bucket " << i << ": ";
			for (auto it = us4.begin(i); it != us4.end(i); ++it)
				cout << *it << " ";
			cout << "\n";
		}
	}
}

// ============================================================
// SECTION 10 : CUSTOM HASH FUNCTION
// ============================================================

// std::hash is defined for: integral types, float, double,
// pointer types, std::string, std::string_view, nullptr_t.
// For custom types, you MUST provide your own hash.

// --- 10.1 Custom struct ---
struct Point
{
	int x, y;

	bool operator==(const Point& other) const   // required for KeyEqual
	{
		return x == other.x && y == other.y;
	}
};

// --- 10.2 Method A: Specialize std::hash ---
namespace std
{
	template<>
		struct hash<Point>
		{
			size_t operator()(const Point& p) const noexcept
			{
				// Combine x and y hashes
				// Classic bit-mixing technique:
				size_t h1 = hash<int>{}(p.x);
				size_t h2 = hash<int>{}(p.y);
				return h1 ^ (h2 << 1);         // XOR + shift

				// Better: use boost::hash_combine pattern:
				// return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
			}
		};
}
// Now: unordered_set<Point> works automatically

// --- 10.3 Method B: Functor (explicit in template arg) ---
struct PointHash
{
	size_t operator()(const Point& p) const noexcept
	{
		size_t h1 = hash<int>{}(p.x);
		size_t h2 = hash<int>{}(p.y);
		return h1 ^ (h2 << 1);
	}
};

struct PointEqual
{
	bool operator()(const Point& a, const Point& b) const
	{
		return a.x == b.x && a.y == b.y;
	}
};

// --- 10.4 Method C: Lambda hash (C++20 simplest) ---
void section10_custom_hash()
{
	// Method A — specialized std::hash
	unordered_set<Point> set_A;
	set_A.insert({1, 2});
	set_A.insert({3, 4});
	set_A.insert({1, 2});  // duplicate — not inserted
	cout << "set_A size: " << set_A.size() << "\n";  // 2

	// Method B — explicit functor
	unordered_set<Point, PointHash, PointEqual> set_B;
	set_B.insert({5, 6});
	set_B.insert({7, 8});

	// Method C — lambda (C++20)
	auto lambda_hash = [](const Point& p) -> size_t
	{
		return hash<int>{}(p.x) ^ (hash<int>{}(p.y) << 1);
	};
	auto lambda_eq = [](const Point& a, const Point& b)
	{
		return a.x == b.x && a.y == b.y;
	};
	unordered_set<Point, decltype(lambda_hash), decltype(lambda_eq)>
		set_C(0, lambda_hash, lambda_eq);
	set_C.insert({1, 2});
	cout << "set_C contains {1,2}: " << set_C.count({1, 2}) << "\n"; // 1

	// --- Hash quality matters ---
	// BAD hash — all points land in same bucket → O(n) everything
	struct BadHash
	{
		size_t operator()(const Point&) const { return 42; }
	};
	// Don't do this in production — just shows the principle

	// GOOD hash properties:
	//   1. Deterministic: same key always gives same hash
	//   2. Uniform: keys spread evenly across buckets
	//   3. Fast: computed in O(1) ideally
	//   4. Avalanche: small key change → big hash change
}

// ============================================================
// SECTION 11 : HASH FUNCTION FOR STRING KEYS
// ============================================================

void section11_string_hash()
{
	// std::hash<string> is built-in — no custom hash needed for strings
	unordered_set<string> words;
	words.insert("hello");
	words.insert("world");
	words.insert("hello");  // duplicate

	cout << "words size: " << words.size() << "\n";        // 2
	cout << "has 'hello': " << words.count("hello") << "\n"; // 1
	cout << "has 'foo':   " << words.count("foo")   << "\n"; // 0

	// Case-insensitive unordered_set
	auto ci_hash = [](const string& s) -> size_t
	{
		size_t h = 0;
		for (char c : s)
			h = h * 31 + tolower(c);   // simple case-insensitive hash
		return h;
	};
	auto ci_equal = [](const string& a, const string& b) -> bool
	{
		if (a.size() != b.size()) return false;
		for (size_t i = 0; i < a.size(); ++i)
			if (tolower(a[i]) != tolower(b[i])) return false;
		return true;
	};
	unordered_set<string, decltype(ci_hash), decltype(ci_equal)>
		ci_set(0, ci_hash, ci_equal);
	ci_set.insert("Hello");
	ci_set.insert("HELLO");   // treated as duplicate
	cout << "ci_set size: " << ci_set.size() << "\n";  // 1
}

// ============================================================
// SECTION 12 : COMMON PATTERNS AND USE CASES
// ============================================================

void section12_patterns()
{
	// === PATTERN 1: Duplicate detection ===
	vector<int> v = {1, 2, 3, 2, 4, 3, 5};
	unordered_set<int> seen;
	vector<int> duplicates;
	for (int x : v)
	{
		if (!seen.insert(x).second)  // insert returns false if duplicate
			duplicates.push_back(x);
	}
	cout << "Duplicates: ";
	for (int x : duplicates) cout << x << " ";  // 2 3
	cout << "\n";

	// === PATTERN 2: Fast membership test ===
	unordered_set<string> banned = {"spam", "hack", "malware"};
	vector<string> emails = {"hello", "spam", "test", "hack"};
	for (const string& e : emails)
		if (banned.count(e))
			cout << e << " is banned\n";

	// === PATTERN 3: Remove duplicates from vector ===
	vector<int> data = {5, 3, 1, 3, 5, 2, 1, 4};
	unordered_set<int> unique_data(data.begin(), data.end());
	vector<int> deduped(unique_data.begin(), unique_data.end());
	// NOTE: original order is NOT preserved

	// === PATTERN 4: Two Sum — O(n) solution ===
	vector<int> nums = {2, 7, 11, 15};
	int target = 9;
	unordered_set<int> complement_set;
	for (int n : nums)
	{
		if (complement_set.count(target - n))
		{
			cout << "Pair found: " << n << " + " << (target - n) << "\n";
			break;
		}
		complement_set.insert(n);
	}

	// === PATTERN 5: Intersection of two arrays ===
	vector<int> arr1 = {1, 2, 3, 4, 5};
	vector<int> arr2 = {3, 4, 5, 6, 7};
	unordered_set<int> set1(arr1.begin(), arr1.end());
	vector<int> intersection;
	for (int x : arr2)
		if (set1.count(x))
			intersection.push_back(x);
	cout << "Intersection: ";
	for (int x : intersection) cout << x << " ";  // 3 4 5
	cout << "\n";

	// === PATTERN 6: Count distinct elements ===
	vector<int> data2 = {1, 2, 2, 3, 3, 3, 4};
	unordered_set<int> distinct(data2.begin(), data2.end());
	cout << "Distinct count: " << distinct.size() << "\n";  // 4

	// === PATTERN 7: Cycle detection in linked list ===
	// Use unordered_set<Node*> to track visited nodes
	// if (visited.count(node)) → cycle detected

	// === PATTERN 8: Word frequency — unique words ===
	vector<string> text = {"the", "cat", "sat", "on", "the", "mat"};
	unordered_set<string> unique_words(text.begin(), text.end());
	cout << "Unique words: " << unique_words.size() << "\n";  // 5
}

// ============================================================
// SECTION 13 : unordered_set vs std::set COMPARISON
// ============================================================
//
//  Feature              unordered_set         set
//  -------              -------------         ---
//  Internal structure   Hash table            Red-black tree
//  Element order        No order (arbitrary)  Sorted (ascending)
//  insert               O(1) avg, O(n) worst  O(log n) guaranteed
//  erase                O(1) avg, O(n) worst  O(log n) guaranteed
//  find/count           O(1) avg, O(n) worst  O(log n) guaranteed
//  Space per element    Higher (bucket ptr)   Higher (3 ptrs + color)
//  Iterator             Forward only          Bidirectional
//  Sorted iteration     No                    Yes
//  Range queries        No (no ordering)      Yes (lower_bound etc.)
//  Custom comparison    Hash + Equal          operator< or comparator
//  Header              <unordered_set>        <set>
//  C++ version          C++11+                C++98+
//
//  WHEN TO USE unordered_set:
//   ✓ Fast O(1) lookup is the priority
//   ✓ Order of elements does not matter
//   ✓ No range queries needed (no "all elements between X and Y")
//   ✓ Keys have a good hash function available
//   ✓ Large datasets where O(log n) becomes costly
//
//  WHEN TO USE set:
//   ✓ Need sorted order during iteration
//   ✓ Need lower_bound / upper_bound / equal_range
//   ✓ Worst-case O(log n) guarantee required (no hash degrade)
//   ✓ Keys do not have a natural hash function
//   ✓ Memory is constrained (set has lower overhead per element)
//   ✓ Range queries: "find all elements between 10 and 50"

// ============================================================
// SECTION 14 : unordered_multiset
// ============================================================
//
//  std::unordered_multiset — like unordered_set but ALLOWS DUPLICATES
//  Header: same — #include <unordered_set>
//
//  Key differences from unordered_set:
//   - insert() always inserts (returns iterator, not pair<iter,bool>)
//   - count(key) can return 0, 1, 2, 3, ... (not just 0 or 1)
//   - equal_range(key) may span multiple elements
//   - find(key) returns iterator to ONE of the matching elements
//   - erase(key) removes ALL elements with that key
//   - erase(iterator) removes exactly ONE element

void section14_multiset()
{
	unordered_multiset<int> ums;

	ums.insert(10);
	ums.insert(20);
	ums.insert(10);  // allowed — duplicate
	ums.insert(20);  // allowed — duplicate
	ums.insert(10);  // 10 inserted 3 times total

	cout << "ums size: "        << ums.size()    << "\n";  // 5
	cout << "count(10): "       << ums.count(10) << "\n";  // 3
	cout << "count(20): "       << ums.count(20) << "\n";  // 2

	// equal_range returns all elements with key=10
	auto [first, last] = ums.equal_range(10);
	cout << "All 10s: ";
	for (auto it = first; it != last; ++it)
		cout << *it << " ";  // 10 10 10
	cout << "\n";

	// erase by VALUE removes ALL matching elements
	ums.erase(10);
	cout << "After erase(10), size: " << ums.size() << "\n";  // 2

	// erase by ITERATOR removes exactly ONE
	auto it = ums.find(20);
	ums.erase(it);   // removes one 20
	cout << "After erase(iter), count(20): " << ums.count(20) << "\n"; // 1
}

// ============================================================
// SECTION 15 : THREAD SAFETY
// ============================================================
//
//  unordered_set is NOT thread-safe by default.
//
//  Safe concurrent operations (read-only):
//   - Multiple threads calling find(), count(), contains() SIMULTANEOUSLY
//     is safe AS LONG AS no thread modifies the set.
//
//  UNSAFE without synchronization:
//   - One thread inserting while another thread reads/inserts/erases
//   - Any modification from multiple threads simultaneously
//
//  How to make it thread-safe:
//   Option 1: std::mutex + std::lock_guard (simple, safe)
//   Option 2: std::shared_mutex (C++17) — multiple readers, one writer
//   Option 3: Use a concurrent hash set library (TBB, folly)
//
//  Example with shared_mutex (read-heavy workload):
//
//  #include <shared_mutex>
//  std::shared_mutex mtx;
//  unordered_set<int> shared_set;
//
//  // Reader thread (multiple allowed simultaneously):
//  void reader(int key)
//  {
//      std::shared_lock lock(mtx);    // shared (read) lock
//      bool found = shared_set.count(key);
//  }
//
//  // Writer thread (exclusive):
//  void writer(int val)
//  {
//      std::unique_lock lock(mtx);    // exclusive (write) lock
//      shared_set.insert(val);
//  }

// ============================================================
// SECTION 16 : MEMORY LAYOUT AND PERFORMANCE TIPS
// ============================================================
//
//  MEMORY OVERHEAD:
//   Each element node typically contains:
//    - The element value itself
//    - A pointer to the next node (for chaining)
//    - Possible alignment padding
//   Plus the bucket array: bucket_count * sizeof(pointer)
//
//  PERFORMANCE TIPS:
//
//  TIP 1: Always call reserve() before bulk insert
//   unordered_set<int> us;
//   us.reserve(1'000'000);           // pre-size for 1M elements
//   for (int i = 0; i < 1'000'000; ++i)
//       us.insert(i);                // zero rehashes!
//
//  TIP 2: Use count() or contains() for existence check, NOT find()
//   if (us.count(key))    { ... }    // cleaner
//   if (us.contains(key)) { ... }    // C++20, clearest
//   if (us.find(key) != us.end()) {} // verbose, only needed if you need the iterator
//
//  TIP 3: emplace() instead of insert() for complex types
//   unordered_set<string> us;
//   us.emplace("hello");             // constructs string in-place
//   us.insert("hello");              // creates temp string, then inserts
//   // For string literals, difference is minor but emplace is better style
//
//  TIP 4: Avoid bad hash functions
//   A hash that maps many keys to the same value degrades O(1) to O(n).
//   Test your hash distribution:
//   size_t max_bucket = 0;
//   for (size_t i = 0; i < us.bucket_count(); ++i)
//       max_bucket = max(max_bucket, us.bucket_size(i));
//   if (max_bucket > 10)
//       cout << "WARNING: hash collisions — " << max_bucket << " in one bucket\n";
//
//  TIP 5: For integer keys, std::hash<int> is identity on most implementations
//   hash<int>{}(42) == 42 on GCC/Clang
//   This means consecutive integers cluster in adjacent buckets
//   Not a problem for random keys, but be aware
//
//  TIP 6: std::unordered_set has high cache miss rate
//   Each node is a separate heap allocation — pointer-chasing.
//   For very small sets (< ~20 elements), std::set or sorted vector
//   + binary_search can be faster due to cache effects.
//   Benchmark your actual use case before assuming unordered is faster.

// ============================================================
// SECTION 17 : COMPLETE METHOD REFERENCE
// ============================================================
//
//  CONSTRUCTORS:
//   unordered_set()                          — default empty
//   unordered_set(n)                         — n = initial bucket count hint
//   unordered_set(first, last)               — range constructor
//   unordered_set(first, last, n)            — range + bucket hint
//   unordered_set(init_list)                 — {1,2,3,...}
//   unordered_set(other)                     — copy constructor
//   unordered_set(move(other))               — move constructor
//
//  ASSIGNMENT:
//   us = other                               — copy assignment
//   us = move(other)                         — move assignment
//   us = {1,2,3}                             — init list assignment
//
//  ITERATORS:
//   begin()   / end()                        — forward iteration
//   cbegin()  / cend()                       — const forward iteration
//   begin(n)  / end(n)                       — local (bucket n) iteration
//   cbegin(n) / cend(n)                      — const local
//
//  CAPACITY:
//   empty()                                  — true if no elements
//   size()                                   — number of elements  O(1)
//   max_size()                               — max possible elements
//
//  MODIFIERS:
//   insert(val)              → pair<iterator,bool>
//   insert(hint, val)        → iterator
//   insert(first, last)      → void
//   insert({val1,val2,...})  → void
//   emplace(args...)         → pair<iterator,bool>
//   emplace_hint(hint,args)  → iterator
//   erase(val)               → size_t (0 or 1)
//   erase(iterator)          → iterator (next)
//   erase(first, last)       → iterator (after last)
//   clear()                  → void
//   swap(other)              → void
//   extract(val)             [C++17] — extract node
//   merge(other)             [C++17] — merge another set
//
//  LOOKUP:
//   find(key)                → iterator
//   count(key)               → 0 or 1
//   contains(key)            → bool [C++20]
//   equal_range(key)         → pair<iterator,iterator>
//
//  HASH POLICY:
//   load_factor()            → float
//   max_load_factor()        → float
//   max_load_factor(ml)      → void (set new max)
//   rehash(n)                → void (at least n buckets)
//   reserve(n)               → void (for n elements without rehash)
//
//  BUCKET INTERFACE:
//   bucket_count()           → size_t
//   max_bucket_count()       → size_t
//   bucket_size(n)           → size_t
//   bucket(key)              → size_t (which bucket key is in)
//
//  OBSERVERS:
//   hash_function()          → Hash   (the hash function object)
//   key_eq()                 → KeyEqual (the equality function object)
//   get_allocator()          → Allocator

// ============================================================
// SECTION 18 : C++17 — extract() AND merge()
// ============================================================

void section18_cpp17()
{
	// --- 18.1 extract(key) [C++17] ---
	// Removes element from set WITHOUT destroying it.
	// Returns a "node handle" — you can examine/modify the key, then re-insert.
	// Use case: move elements between sets WITHOUT any allocation.

	unordered_set<string> us1 = {"apple", "banana", "cherry"};
	unordered_set<string> us2;

	// Extract "banana" from us1 (no deallocation)
	auto node = us1.extract("banana");
	if (!node.empty())
	{
		cout << "Extracted: " << node.value() << "\n";  // banana
								// node.value() returns reference to the element
								// You can modify it before re-inserting:
								// node.value() = "BANANA";  // modify the key
		us2.insert(move(node));    // move into us2 (no allocation!)
	}
	cout << "us1 size: " << us1.size() << "\n";  // 2
	cout << "us2 size: " << us2.size() << "\n";  // 1

	// --- 18.2 merge(other) [C++17] ---
	// Moves ALL elements from 'other' into 'this' without any allocation.
	// If element already exists in 'this', it stays in 'other'.
	unordered_set<int> A = {1, 2, 3};
	unordered_set<int> B = {3, 4, 5};  // 3 exists in both

	A.merge(B);
	// A now has {1,2,3,4,5}, B has {3} (3 couldn't be moved)
	cout << "A size: " << A.size() << "\n";  // 5
	cout << "B size: " << B.size() << "\n";  // 1 (3 stayed in B)
}

// ============================================================
// SECTION 19 : LEETCODE PATTERNS USING unordered_set
// ============================================================

void section19_leetcode_patterns()
{
	// --- Pattern 1: Contains Duplicate (LC#217) ---
	// O(n) time, O(n) space
	auto containsDuplicate = [](vector<int>& nums) -> bool
	{
		unordered_set<int> seen;
		for (int n : nums)
			if (!seen.insert(n).second)  // false = already existed
				return true;
		return false;
	};
	vector<int> v1 = {1,2,3,1};
	cout << "Has duplicate: " << containsDuplicate(v1) << "\n";  // 1

	// --- Pattern 2: Intersection of Two Arrays (LC#349) ---
	auto intersection = [](vector<int>& a, vector<int>& b) -> vector<int>
	{
		unordered_set<int> set_a(a.begin(), a.end());
		vector<int> result;
		for (int x : b)
			if (set_a.erase(x))   // erase returns 1 if found
				result.push_back(x);
		return result;
	};

	// --- Pattern 3: Longest Consecutive Sequence (LC#128) O(n) ---
	auto longestConsecutive = [](vector<int>& nums) -> int
	{
		unordered_set<int> num_set(nums.begin(), nums.end());
		int best = 0;
		for (int n : num_set)
		{
			// Only start a sequence from the BEGINNING
			if (!num_set.count(n - 1))
			{
				int len = 1;
				while (num_set.count(n + len))
					++len;
				best = max(best, len);
			}
		}
		return best;
	};
	vector<int> v2 = {100,4,200,1,3,2};
	cout << "Longest consecutive: " << longestConsecutive(v2) << "\n"; // 4

	// --- Pattern 4: Happy Number (LC#202) ---
	auto isHappy = [](int n) -> bool
	{
		unordered_set<int> seen;
		while (n != 1)
		{
			if (!seen.insert(n).second) return false; // cycle detected
			int sum = 0;
			while (n > 0) { int d = n % 10; sum += d*d; n /= 10; }
			n = sum;
		}
		return true;
	};
	cout << "19 is happy: " << isHappy(19)  << "\n";  // 1
	cout << "2  is happy: " << isHappy(2)   << "\n";  // 0

	// --- Pattern 5: Jewels and Stones (LC#771) ---
	auto numJewelsInStones = [](string& jewels, string& stones) -> int
	{
		unordered_set<char> jewel_set(jewels.begin(), jewels.end());
		int count = 0;
		for (char c : stones)
			count += jewel_set.count(c);
		return count;
	};
}

// ============================================================
// SECTION 20 : FULL WORKING DEMO
// ============================================================

int main()
{
	cout << "===== unordered_set Complete Demo =====\n\n";

	// --- Basic usage ---
	unordered_set<int> us;
	us.reserve(20);                           // pre-size for 20 elements

	for (int i = 1; i <= 10; ++i)
		us.insert(i);

	cout << "Size: "         << us.size()        << "\n";  // 10
	cout << "Empty: "        << us.empty()        << "\n";  // 0
	cout << "bucket_count: " << us.bucket_count() << "\n";
	cout << "load_factor: "  << us.load_factor()  << "\n";

	// Insert duplicate — silent no-op
	auto [it, inserted] = us.insert(5);
	cout << "Inserted 5 again: " << inserted << "\n";  // 0 (false)

	// Lookup
	cout << "Contains 7: " << us.count(7)    << "\n";  // 1
	cout << "Contains 99: " << us.count(99)  << "\n";  // 0

	// Erase
	us.erase(5);
	cout << "After erase(5), count(5): " << us.count(5) << "\n"; // 0

	// Iterate (order is arbitrary)
	cout << "Elements: ";
	for (int x : us)
		cout << x << " ";
	cout << "\n";

	// Clear
	us.clear();
	cout << "After clear, size: " << us.size() << "\n";  // 0

	cout << "\n===== Running all sections =====\n\n";

	section3_declaration();
	section4_insert();
	section5_lookup();
	section6_erase();
	section7_capacity();
	section8_iterators();
	section9_hash_policy();
	section10_custom_hash();
	section11_string_hash();
	section12_patterns();
	section14_multiset();
	section18_cpp17();
	section19_leetcode_patterns();

	cout << "\n===== Done =====\n";
	return 0;
}

// ============================================================
// SECTION 21 : INTERVIEW QUESTIONS ON unordered_set
// ============================================================
//
//  Q1: What is the average and worst-case time complexity for
//      insert, erase, and find in unordered_set?
//  A1: Average O(1), worst O(n). Worst case when all keys hash
//      to the same bucket (bad hash function).
//
//  Q2: What happens when load_factor exceeds max_load_factor?
//  A2: Rehash is triggered. Bucket count doubles. All elements
//      are re-hashed. All iterators are invalidated. O(n) cost.
//
//  Q3: Can you store a custom struct in unordered_set without
//      any changes?
//  A3: No. You must provide: (1) hash function and
//      (2) equality operator. std::hash is not specialized for
//      user-defined types by default.
//
//  Q4: What is the difference between erase(key) and erase(iterator)?
//  A4: erase(key) removes all elements with that key (returns count).
//      erase(iterator) removes exactly one element (returns next iter).
//      For unordered_set, erase(key) removes 0 or 1 element.
//      For unordered_multiset, erase(key) can remove multiple.
//
//  Q5: How do you iterate and erase elements from unordered_set safely?
//  A5: Use the iterator returned by erase():
//      for (auto it = us.begin(); it != us.end();)
//          if (condition(*it)) it = us.erase(it);
//          else ++it;
//
//  Q6: How is unordered_set different from unordered_map?
//  A6: unordered_set stores KEYS only.
//      unordered_map stores KEY-VALUE pairs.
//      unordered_set: O(1) membership test.
//      unordered_map: O(1) key-to-value lookup.
//
//  Q7: Can you modify an element stored in unordered_set?
//  A7: No. All elements are stored as const internally. Modification
//      would change the hash value, corrupting the table structure.
//      To "modify": erase old + insert new.
//
//  Q8: What is the worst hash function you could write?
//  A8: One that returns a constant: return 42;
//      All elements land in the same bucket. Every operation
//      degrades to O(n) linear search.
//
//  Q9: When would you prefer std::set over unordered_set?
//  A9: When you need: sorted iteration, lower_bound/upper_bound,
//      guaranteed O(log n) in worst case, no hash function available,
//      or memory constraints (unordered has higher overhead per element).
//
//  Q10: What is reserve() in unordered_set and why is it important?
//  A10: reserve(n) pre-sizes the hash table so n elements can be
//       inserted without triggering rehash.
//       Without reserve(): O(log n) rehashes during bulk insert (slow).
//       With reserve(n):   0 rehashes (fast).
//       Call reserve() BEFORE any bulk insertion.

// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra Unordered_set_Explanation.cpp -o us
// g++ -std=c++20 -Wall -Wextra claude_unordered_set.cpp  -o claude_unordered_set && ./claude_unordered_set
// ============================================================

// ============================================================
// ADD THESE INCLUDES TO THE TOP OF YOUR EXISTING FILE
// ============================================================
#include <utility>       // std::pair
#include <tuple>         // std::tuple, std::get, std::tuple_size
#include <memory>        // shared_ptr, unique_ptr, make_shared, make_unique
#include <unordered_map> // used inside the WatchManager example
#include <cstdint>       // uint32_t
#include <sys/types.h>   // pid_t
#include <sys/inotify.h> // inotify_add_watch/rm_watch (Linux-specific)
#include <chrono>        // benchmarking
#include <random>        // mt19937, uniform_int_distribution

// ============================================================
// SECTION 22 : ADVANCED HASH COMBINING TECHNIQUES
// ============================================================
//
// std::hash is NOT defined for std::pair, std::tuple, std::vector,
// or any composite/aggregate type. You must combine the hashes of
// the individual members yourself.
//
// THE HASH COMBINE PROBLEM:
// Naive XOR combine (h1 ^ h2) is WEAK:
//   - combine(a,b) == combine(b,a)  — symmetric, loses ordering info
//   - combine(a,a) == 0             — any equal pair collapses to 0
//
// BETTER: the boost::hash_combine pattern
//   seed ^= hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//   0x9e3779b9 is derived from the golden ratio (2^32 / phi).
//   It breaks symmetry and spreads bits far better than plain XOR.

// --- 22.1 Generic hash_combine helper ---
template <typename T>
void hash_combine(size_t& seed, const T& val)
{
    seed ^= hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// --- 22.2 Hashing std::pair<A,B> ---
struct PairHash
{
    template <typename A, typename B>
    size_t operator()(const pair<A, B>& p) const noexcept
    {
        size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);
        return seed;
    }
};

// --- 22.3 Hashing std::tuple<Ts...> (compile-time recursion) ---
template <typename Tuple, size_t Index = 0>
struct TupleHashImpl
{
    static void apply(size_t& seed, const Tuple& t)
    {
        hash_combine(seed, get<Index>(t));
        TupleHashImpl<Tuple, Index + 1>::apply(seed, t);
    }
};

// Base case: stops recursion when Index reaches tuple_size
template <typename Tuple>
struct TupleHashImpl<Tuple, tuple_size<Tuple>::value>
{
    static void apply(size_t&, const Tuple&)
    {
    }
};

struct TupleHash
{
    template <typename... Ts>
    size_t operator()(const tuple<Ts...>& t) const noexcept
    {
        size_t seed = 0;
        TupleHashImpl<tuple<Ts...>>::apply(seed, t);
        return seed;
    }
};

// --- 22.4 Hashing std::vector<T> (order-sensitive) ---
struct VectorHash
{
    template <typename T>
    size_t operator()(const vector<T>& v) const noexcept
    {
        size_t seed = v.size();
        for (const auto& elem : v)
            hash_combine(seed, elem);
        return seed;
    }
};

void section22_hash_combining()
{
    // unordered_set of pairs — e.g. grid coordinates
    unordered_set<pair<int,int>, PairHash> coord_set;
    coord_set.insert({1, 2});
    coord_set.insert({3, 4});
    coord_set.insert({1, 2});          // duplicate
    cout << "coord_set size: " << coord_set.size() << "\n";     // 2

    // unordered_set of tuples
    unordered_set<tuple<int,int,int>, TupleHash> triple_set;
    triple_set.insert({1, 2, 3});
    triple_set.insert({4, 5, 6});
    cout << "triple_set size: " << triple_set.size() << "\n";   // 2

    // unordered_set of vectors — e.g. deduplicating sequences/paths
    unordered_set<vector<int>, VectorHash> path_set;
    path_set.insert({1, 2, 3});
    path_set.insert({1, 2, 3});         // duplicate (same order+content)
    path_set.insert({3, 2, 1});         // different order = different key
    cout << "path_set size: " << path_set.size() << "\n";       // 2

    // NOTE: pair/tuple/vector already provide operator== by default,
    // so KeyEqual can stay at its default (std::equal_to) — you
    // only need to supply the Hash functor, as shown above.
}

// ============================================================
// SECTION 23 : unordered_set OF POINTERS AND SMART POINTERS
// ============================================================
//
// std::hash<T*> is defined by default for ANY pointer type.
// It hashes the POINTER VALUE (the address), NOT what it points to.

void section23_pointers()
{
    int a = 5, b = 5;
    int* pa = &a;
    int* pb = &b;

    unordered_set<int*> ptr_set;
    ptr_set.insert(pa);
    ptr_set.insert(pb);
    // *pa == *pb (both 5), but pa != pb as addresses.
    cout << "ptr_set size: " << ptr_set.size() << "\n";          // 2

    // --- 23.1 shared_ptr — hashes get(), not the pointee ---
    unordered_set<shared_ptr<int>> sp_set;
    auto sp1 = make_shared<int>(10);
    auto sp2 = make_shared<int>(10);   // different object, same value
    sp_set.insert(sp1);
    sp_set.insert(sp2);
    cout << "sp_set size: " << sp_set.size() << "\n";            // 2

    auto sp3 = sp1;                     // shares ownership with sp1
    sp_set.insert(sp3);
    cout << "sp_set size after sp3: " << sp_set.size() << "\n";  // still 2

    // --- 23.2 unique_ptr — move-only, must MOVE into the set ---
    unordered_set<unique_ptr<int>> up_set;
    auto up = make_unique<int>(42);
    up_set.insert(move(up));            // must explicitly move
    // 'up' is now empty (nullptr) — never use it again after this
    cout << "up_set size: " << up_set.size() << "\n";            // 1
    // NOTE: unordered_set<unique_ptr<T>> is rarely useful in practice
    // since elements are const inside the set — you can't easily get
    // ownership back out except via extract() (Section 18).

    // --- 23.3 Hashing shared_ptr BY VALUE instead of by address ---
    struct DerefHash
    {
        size_t operator()(const shared_ptr<int>& p) const noexcept
        {
            return p ? hash<int>{}(*p) : 0;
        }
    };
    struct DerefEqual
    {
        bool operator()(const shared_ptr<int>& x, const shared_ptr<int>& y) const
        {
            if (!x || !y) return x == y;
            return *x == *y;
        }
    };
    unordered_set<shared_ptr<int>, DerefHash, DerefEqual> value_set;
    value_set.insert(make_shared<int>(99));
    value_set.insert(make_shared<int>(99));   // different object, SAME value
    cout << "value_set size (by value): " << value_set.size() << "\n"; // 1
}

// ============================================================
// SECTION 24 : DEBUGGING unordered_set WITH GDB
// ============================================================
//
// unordered_set's raw internal layout (bucket array + chained nodes)
// is not human-readable with a plain 'print' unless pretty-printers
// are active.
//
// --- 24.1 GCC libstdc++ Python pretty-printers (if installed) ---
//   (gdb) print us
//   $1 = std::unordered_set with 5 elements = {1, 2, 3, 4, 5}
//
//   Without pretty-printers you'd instead see raw internals like:
//   $1 = {_M_h = {_M_bucket_count = 13, _M_buckets = 0x..., ...}}
//
// --- 24.2 Enable them manually if missing — add to ~/.gdbinit:
//   python
//   import sys
//   sys.path.insert(0, '/usr/share/gcc/python')
//   from libstdcxx.v6.printers import register_libstdcxx_printers
//   register_libstdcxx_printers(None)
//   end
//
// --- 24.3 Practical fallback: a dump helper you call live in gdb ---

void dump_set(const unordered_set<int>& us)
{
    cerr << "[[ unordered_set dump: size=" << us.size()
         << " buckets=" << us.bucket_count()
         << " load_factor=" << us.load_factor() << " ]]\n";
    for (const auto& elem : us)
        cerr << "  " << elem << "\n";
}

void section24_gdb_debugging_demo()
{
    unordered_set<int> us = {10, 20, 30};
    dump_set(us);
    // Inside an actual gdb session at a breakpoint, run:
    //   (gdb) call dump_set(us)
    // This works even WITHOUT pretty-printers, since it's just
    // real code iterating the set through its public API.
    cout << "See stderr output above; try 'call dump_set(us)' in gdb\n";
}

// ============================================================
// SECTION 25 : IMPLEMENTATION DIFFERENCES ACROSS COMPILERS
// ============================================================
//
// The standard specifies BEHAVIOR (complexity guarantees, interface)
// but NOT the exact internal implementation.
//
//  GCC libstdc++ (default on most Linux distros):
//   - Bucket counts are chosen from a precomputed list of PRIME
//     numbers (not powers of 2) — spreads hash values more evenly,
//     especially for hash functions weak in their low bits.
//   - Each node caches its own computed hash, so rehashing reuses
//     the cached hash instead of recomputing it for every element.
//   - Singly-linked chaining per bucket.
//
//  LLVM libc++ (clang, always on macOS):
//   - Also chaining-based historically, but its own bucket growth
//     policy and exact bucket counts can differ from libstdc++.
//
//  PRACTICAL IMPLICATION:
//   - bucket_count() after inserting N elements can differ between
//     g++ and clang++ builds of the SAME program.
//   - Iteration ORDER can differ across compilers, versions, and
//     even between runs of the same binary if the implementation
//     randomizes string hashing per-process (a hardening measure
//     against hash-flooding DoS). NEVER rely on iteration order
//     being stable across runs, builds, or platforms.

void section25_growth_policy()
{
    unordered_set<int> us;
    size_t last_bc = us.bucket_count();
    cout << "Initial bucket_count: " << last_bc << "\n";

    for (int i = 1; i <= 200; ++i)
    {
        us.insert(i);
        if (us.bucket_count() != last_bc)
        {
            cout << "size=" << i
                 << " triggered rehash -> bucket_count="
                 << us.bucket_count() << "\n";
            last_bc = us.bucket_count();
        }
    }
    // Run this on g++ and on clang++ and compare the printed
    // bucket_count sequence — expect a different progression.
}

// ============================================================
// SECTION 26 : EXCEPTION SAFETY GUARANTEES
// ============================================================
//
//  ERASE:
//   - erase(key), erase(iterator), erase(first,last), clear()
//     do NOT throw, provided Hash and KeyEqual don't throw.
//     Removing elements never allocates.
//
//  INSERT / EMPLACE:
//   - If insertion does NOT trigger a rehash: STRONG guarantee —
//     on exception, the container is left exactly as it was.
//   - If insertion DOES trigger a rehash: the container remains
//     valid, though its bucket layout may differ if the rehash
//     itself is interrupted partway through.
//
//  RULE OF THUMB:
//   Prefer element types with a noexcept move constructor. This
//   lets rehashing MOVE elements into new buckets instead of
//   copying them — faster and safer.

// ============================================================
// SECTION 27 : noexcept SPECIFICATIONS
// ============================================================
//
//  Move constructor / swap / move assignment on unordered_set are
//  conditionally noexcept, based on whether Hash, KeyEqual, and
//  Allocator themselves have noexcept move operations. With the
//  default template parameters, this is true — you rarely need to
//  do anything extra here, unlike with your own hand-written classes
//  where you must explicitly mark the move constructor noexcept.
//
//  This matters when unordered_set<T> is itself stored inside
//  another container (e.g. vector<unordered_set<int>>) — the outer
//  container checks this noexcept-ness to decide move vs copy
//  during ITS OWN reallocation.

void section27_noexcept_check()
{
    cout << boolalpha;
    cout << "unordered_set<int> move ctor noexcept: "
         << is_nothrow_move_constructible<unordered_set<int>>::value
         << "\n";
}

// ============================================================
// SECTION 28 : REAL LINUX SYSTEMS PROGRAMMING USE CASES
// ============================================================

// --- 28.1 Deduplicating watched paths (inotify) ---
// The same real path can be reached twice during a recursive
// directory scan (symlinks, repeated scans). unordered_set<string>
// guarantees each path is watched exactly once.

class WatchManager
{
    int inotify_fd;
    unordered_set<string> watched_paths;       // dedup by path — O(1)
    unordered_map<int, string> wd_to_path;      // wd -> path

public:
    WatchManager(int fd) : inotify_fd(fd)
    {
    }

    bool add_watch(const string& path, uint32_t mask)
    {
        if (watched_paths.count(path))
            return false;               // already watched, skip

        int wd = inotify_add_watch(inotify_fd, path.c_str(), mask);
        if (wd == -1)
            return false;

        watched_paths.insert(path);
        wd_to_path[wd] = path;
        return true;
    }

    void remove_watch(int wd)
    {
        auto it = wd_to_path.find(wd);
        if (it != wd_to_path.end())
        {
            watched_paths.erase(it->second);
            inotify_rm_watch(inotify_fd, wd);
            wd_to_path.erase(it);
        }
    }

    bool is_watched(const string& path) const
    {
        return watched_paths.count(path) > 0;
    }
};
// NOTE: this class is illustrative — it needs a real fd from
// inotify_init() to actually run, so it isn't invoked in main()
// below. Wire it up against your existing inotify project's fd.

// --- 28.2 Tracking "seen" PIDs across /proc scan cycles ---
void section28_pid_tracking_demo()
{
    unordered_set<pid_t> known_pids;

    vector<pid_t> scan1 = {101, 102, 103};
    for (pid_t p : scan1)
        if (known_pids.insert(p).second)
            cout << "New process detected: PID " << p << "\n";

    // Next scan cycle: 101,102 still running, 103 exited, 104 is new
    vector<pid_t> scan2 = {101, 102, 104};
    unordered_set<pid_t> current_scan(scan2.begin(), scan2.end());

    for (pid_t p : current_scan)
        if (!known_pids.count(p))
            cout << "New process detected: PID " << p << "\n";   // 104

    for (pid_t p : known_pids)
        if (!current_scan.count(p))
            cout << "Process exited: PID " << p << "\n";         // 103

    known_pids = move(current_scan);   // carry forward to next cycle
}

// --- 28.3 O(1) fd-registration check for an epoll-based server ---
// epoll_ctl has no "query" mode, so track registered fds separately.
class FdRegistry
{
    unordered_set<int> registered_fds;
public:
    bool try_register(int fd)
    {
        return registered_fds.insert(fd).second;
    }
    void unregister(int fd)
    {
        registered_fds.erase(fd);
    }
    bool is_registered(int fd) const
    {
        return registered_fds.count(fd) > 0;
    }
    size_t count_registered() const
    {
        return registered_fds.size();
    }
};

void section28_fd_registry_demo()
{
    FdRegistry reg;
    cout << "register fd 5: " << reg.try_register(5) << "\n";        // 1
    cout << "register fd 5 again: " << reg.try_register(5) << "\n";  // 0
    cout << "is 5 registered: " << reg.is_registered(5) << "\n";      // 1
    reg.unregister(5);
    cout << "is 5 registered after unregister: "
         << reg.is_registered(5) << "\n";                            // 0
}

// ============================================================
// SECTION 29 : MATHEMATICAL COMPLEXITY — LOAD FACTOR & PROBABILITY
// ============================================================
//
// For SEPARATE CHAINING (libstdc++'s design), with n keys spread
// over b buckets by a reasonably uniform hash:
//
//   load_factor (alpha) = n / b
//
// Expected elements examined for a SUCCESSFUL search:
//   E[successful]   ≈ 1 + alpha/2
//
// Expected elements examined for an UNSUCCESSFUL search (this is
// what happens on every insert of a brand-new key, since the table
// must walk the whole chain to confirm the key is absent):
//   E[unsuccessful] ≈ alpha
//
// CONCRETE EXAMPLE:
//   n = 1,000,000, max_load_factor = 1.0 (default)
//   => average chain length ≈ 1.0 => ~1.5 comparisons per find()
//
//   If you manually relaxed max_load_factor to 10.0 to save some
//   bucket-array memory:
//   => average chain length ≈ 10 => ~6 comparisons per find()
//   => roughly 6x slower lookups for that memory saving

void section29_benchmark()
{
    const int N = 1'000'000;
    mt19937 rng(42);
    uniform_int_distribution<int> dist(0, 10'000'000);

    vector<int> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i)
        keys.push_back(dist(rng));

    for (float max_lf : {0.5f, 1.0f, 4.0f, 10.0f})
    {
        unordered_set<int> us;
        us.max_load_factor(max_lf);
        us.reserve(N);
        for (int k : keys)
            us.insert(k);

        auto t0 = chrono::steady_clock::now();
        size_t found = 0;
        for (int k : keys)
            found += us.count(k);
        auto t1 = chrono::steady_clock::now();

        auto us_time = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        cout << "max_load_factor=" << max_lf
             << " actual_load_factor=" << us.load_factor()
             << " bucket_count=" << us.bucket_count()
             << " lookup_time=" << us_time << "us"
             << " found=" << found << "\n";
    }
    // Expect lookup_time to climb as max_load_factor climbs —
    // a direct, measurable demonstration of E[unsuccessful] ≈ alpha.
}

// ============================================================
// SECTION 30 : COMPILER WARNINGS & STATIC ANALYSIS
// ============================================================
//
// Compile with:
//   g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//       -Wpedantic -fsanitize=address,undefined Unordered_set_Explanation.cpp
//
//  1. "no matching function for call to insert"
//     -> your custom struct is missing operator== required by the
//        default KeyEqual (std::equal_to).
//
//  2. "call to implicitly-deleted copy constructor"
//     -> inserting a move-only type (unique_ptr) by copy instead of
//        move. Fix: us.insert(std::move(ptr));
//
//  3. clang-tidy: modernize-use-emplace
//     -> replace us.insert(Foo(a,b)) with us.emplace(a,b) to avoid
//        constructing a temporary Foo first.
//
//  4. clang-tidy: performance-unnecessary-value-param
//     -> a read-only function should take const unordered_set<T>&,
//        not unordered_set<T> by value (copies the entire table).
//
//  5. cppcheck: "iterator may be end(), dereferenced without check"
//     -> always guard: if (it != us.end()) before *it.
//
//  6. -Wunused-variable on a structured binding
//     auto [it, ok] = us.insert(x);   // if 'it' goes unused
//     -> use just: bool ok = us.insert(x).second;
//
//  7. Never return an iterator/reference into a LOCAL unordered_set
//     from a function — it's destroyed at function exit.

// ============================================================
// SECTION 31 : CONSOLIDATED PITFALLS AND FIXES
// ============================================================
//
//  1. Assuming any iteration order — there isn't one. Use std::set
//     if you need sorted order, or track insertion order separately.
//  2. No operator[] — unordered_set has no "value" to assign, only
//     the key itself. us[5] = 10; is a compile error, not a bug.
//  3. Modifying through an iterator — *it = x; is a compile error.
//     Erase the old value and insert the new one instead.
//  4. Skipping reserve() before a bulk insert — costs ~20 rehashes
//     for 1M elements. Always us.reserve(expected_count) first.
//  5. Holding an iterator across a rehash-triggering insert — it may
//     be silently invalidated. Re-find() after bulk modifications.
//  6. No hash specialization for a custom struct — compile error.
//     Specialize std::hash<T> or pass a hash functor explicitly.
//  7. A degenerate custom hash (e.g. always returns 1) collapses
//     every operation to O(n). Use hash_combine on all fields.
//  8. Expecting operator== to care about insertion order — it
//     doesn't. {1,2,3} == {3,2,1} is TRUE by design (content-equal).

void section31_operator_equality_demo()
{
    unordered_set<int> a = {1, 2, 3};
    unordered_set<int> b = {3, 2, 1};   // same content, different order
    cout << "a == b: " << (a == b) << "\n";   // 1 — content-equality

    unordered_set<int> c = {1, 2, 4};
    cout << "a == c: " << (a == c) << "\n";   // 0 — different content
}

// ============================================================
// SECTION 32 : MODERN ALTERNATIVES TO std::unordered_set
// ============================================================
//
//  absl::flat_hash_set (Google Abseil):
//   - Open addressing (SwissTable) instead of chaining — one
//     contiguous array, no pointer-chasing per lookup.
//   - Typically 2-4x faster than std::unordered_set in benchmarks.
//   - STRICTER invalidation: erase invalidates ALL iterators.
//
//  boost::unordered_flat_set (Boost 1.81+):
//   - Also open-addressing, similar performance profile to Abseil.
//
//  tsl::robin_map / tsl::robin_set (header-only):
//   - Robin Hood hashing — open addressing with low probe-length
//     variance.
//
//  std::unordered_set still wins when:
//   - You need reference/pointer STABILITY across insert/erase
//     (it never invalidates existing element references except on
//     rehash — open-addressing tables often invalidate on every
//     insert/erase since elements physically move).
//   - You want zero extra dependencies.
//
//  RULE OF THUMB: start with std::unordered_set. Reach for
//  flat_hash_set/robin_map only after profiling shows hash-table
//  lookup is a measured bottleneck in YOUR program.

// ============================================================
// SECTION 33 : ADDITIONAL INTERVIEW QUESTIONS (ADVANCED)
// ============================================================
//
//  Q11: Why is XOR (^) alone a poor way to combine two hash values?
//  A11: XOR is symmetric — combine(a,b) == combine(b,a) — and
//       combine(a,a) == 0 for any a. Unrelated key pairs collide
//       more than a good combiner allows. hash_combine avoids this
//       with a golden-ratio-derived odd constant plus bit shifts.
//
//  Q12: Does std::hash<shared_ptr<T>> hash the pointee or the pointer?
//  A12: The pointer address (get()) — same as std::hash<T*>. Two
//       shared_ptr managing DIFFERENT objects with EQUAL values are
//       different keys unless you supply a dereferencing hash/equal.
//
//  Q13: If Hash and KeyEqual never throw, which operations are
//       guaranteed not to throw?
//  A13: erase() (all forms) and clear() — they never allocate.
//       insert()/emplace() can still throw from allocation failure
//       or the Key's own constructor.
//
//  Q14: Why might two runs of the SAME program show different
//       iteration order for identical input?
//  A14: Implementation-defined bucket policy, plus some hardened
//       standard libraries randomize string hashing per-process to
//       defend against hash-flooding DoS — shifting bucket
//       assignment between runs even with identical insert order.
//
//  Q15: When would you choose an open-addressing set (e.g.
//       absl::flat_hash_set) over std::unordered_set?
//  A15: When lookup-heavy performance matters more than reference
//       stability — open addressing avoids per-node heap allocation
//       and pointer-chasing, giving much better cache locality.

// ============================================================
// ADD THESE CALLS INTO YOUR EXISTING main(), just before
// the "===== Done =====" line
// ============================================================
//
//    section22_hash_combining();
//    section23_pointers();
//    section24_gdb_debugging_demo();
//    section25_growth_policy();
//    section27_noexcept_check();
//    section28_pid_tracking_demo();
//    section28_fd_registry_demo();
//    section29_benchmark();
//    section31_operator_equality_demo();
//
// (Sections 26, 30, 32, 33 are reference-only — no runnable demo,
//  same as Sections 1, 2, 13, 15, 16, 17 in the first half.)

// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//          -fsanitize=address,undefined Unordered_set_Explanation.cpp -o us
// ============================================================

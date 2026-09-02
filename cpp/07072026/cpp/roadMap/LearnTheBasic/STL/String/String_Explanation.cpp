// ============================================================
//  FILE  : String_Explanation.cpp
//  TOPIC : std::string — Complete Reference + Interview Bank
//  LEVEL : Intermediate to Advanced (C++ Linux Programmer, 5 YOE)
//  STD   : C++11 and later (C++17/20/23 features marked explicitly)
// ============================================================

#include <string>
#include <string_view>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <climits>
#include <charconv>
#include <chrono>
#include <fstream>
using namespace std;

// ============================================================
// SECTION 1 : WHAT IS std::string ?
// ============================================================
//
//  std::string is a SPECIALIZATION of the class template
//  std::basic_string<char>. It manages a DYNAMIC, GROWABLE
//  sequence of characters — conceptually similar to vector<char>,
//  but with string-specific behavior layered on top:
//
//   - ALWAYS null-terminated internally since C++11 (guaranteed)
//   - Small String Optimization (SSO) — short strings avoid the
//     heap entirely (implementation detail, not standard-mandated,
//     but present in every major implementation)
//   - Rich search/comparison/conversion API tailored to text
//   - operator+ for concatenation, operator<< for streaming
//
//  Header required:
//   #include <string>
//
//  Full template signature (the thing 'string' is an alias for):
//   template
//       class CharT,
//       class Traits    = std::char_traits<CharT>,
//       class Allocator = std::allocator<CharT>
//   > class basic_string;
//
//   using string    = basic_string<char>;      // what we cover here
//   using wstring   = basic_string<wchar_t>;
//   using u8string  = basic_string<char8_t>;   // C++20
//   using u16string = basic_string<char16_t>;
//   using u32string = basic_string<char32_t>;
//
//  Traits parameter — controls character-level operations
//  (compare, copy, length, eq) — you can swap in a custom
//  char_traits for e.g. CASE-INSENSITIVE strings (Section 22.4)
//  WITHOUT touching a single line of your string-handling code.

// ============================================================
// SECTION 2 : INTERNAL IMPLEMENTATION — SSO & GROWTH POLICY
// ============================================================
//
//  sizeof(std::string) on 64-bit GCC libstdc++: 32 bytes.
//
//  SSO (Small String Optimization): strings up to a threshold
//  length are stored INLINE inside the string object itself —
//  NO heap allocation at all. Beyond that threshold, the string
//  falls back to a heap-allocated buffer, same as vector.
//
//  SSO THRESHOLD BY IMPLEMENTATION (differs — never hardcode this):
//   GCC libstdc++ (Linux g++)     : 15 chars inline, sizeof = 32
//   Clang libc++ (macOS/Linux)    : 22 chars inline, sizeof = 24
//   MSVC STL (Windows)            : 15 chars inline, sizeof = 32
//
//  INLINE (SSO) LAYOUT — conceptually:
//   [ char buf[15] ][ 1 length byte (also encodes SSO-vs-heap flag) ]
//   total 16 bytes... but GCC's actual object is 32 bytes because
//   it uses a UNION: the same 24 bytes that would hold
//   {pointer, capacity} in heap mode instead hold the 15-char
//   inline buffer + length byte when in SSO mode. The extra 8
//   bytes (making 32 total) hold the cached size(), which is
//   ALWAYS a separate field regardless of SSO/heap mode.
//
//  HEAP LAYOUT (string exceeds SSO threshold):
//   { char* data_ptr; size_t size; size_t capacity; }
//   = 8 + 8 + 8 = 24 bytes of BOOKKEEPING, plus a SEPARATE heap
//   allocation holding the actual characters + null terminator.
//
//  GROWTH FACTOR when capacity is exceeded (implementation-defined,
//  same caveat as vector):
//   GCC libstdc++  : doubles capacity (2x)
//   Clang libc++   : doubles capacity (2x)
//   MSVC STL       : grows by 1.5x
//
//  DETECTING SSO AT RUNTIME:
//   bool isSSO = (s.data() == reinterpret_cast<const char*>(&s));
//   // true  -> data lives INSIDE the string object (SSO active)
//   // false -> data() points to a SEPARATE heap allocation

void section2_sso_detection()
{
    string shortStr = "hello";                          // 5 chars — SSO on GCC
    string longStr  = "this is definitely over fifteen"; // >15 chars — heap on GCC

    bool shortIsSSO = (shortStr.data() == reinterpret_cast<const char*>(&shortStr));
    bool longIsSSO  = (longStr.data()  == reinterpret_cast<const char*>(&longStr));

    cout << "sizeof(string): " << sizeof(string) << " bytes\n";   // 32 on GCC
    cout << "shortStr SSO active: " << boolalpha << shortIsSSO << "\n";  // true
    cout << "longStr  SSO active: " << longIsSSO << "\n";                 // false

    // NULL TERMINATOR GUARANTEE (since C++11) — always readable:
    string s = "hello";
    cout << "s[s.size()] is null: " << (s[s.size()] == '\0') << "\n";   // true

    // MODIFYING the null terminator position through this read is UB:
    // const_cast<char&>(s[s.size()]) = 'X';   // NEVER do this
}

// ============================================================
// SECTION 3 : DECLARATION AND INITIALIZATION — ALL CONSTRUCTORS
// ============================================================

void section3_construction()
{
    // --- 3.1 Default — empty, no allocation ---
    string s1;

    // --- 3.2 From a C-string literal ---
    string s2 = "hello";
    string s3("hello");

    // --- 3.3 From C-string + explicit length — can hold embedded NULs ---
    string s4("hel\0lo", 6);          // size() == 6, contains a literal '\0' at index 3
    cout << "s4.size() with embedded null: " << s4.size() << "\n";   // 6
    // Contrast: string s5 = "hel\0lo"; would give size() == 3 — the
    // const char* constructor stops at the FIRST '\0', because it has
    // to call strlen() internally to find the length (no length known
    // up front). The explicit-length constructor bypasses that.

    // --- 3.4 Fill constructor — n copies of one char ---
    string s6(10, 'x');               // "xxxxxxxxxx"

    // --- 3.5 Copy constructor — deep copy ---
    string s7 = s2;

    // --- 3.6 Move constructor — O(1) for heap strings, steals the buffer ---
    string s8 = move(s2);             // s2 now valid-but-unspecified

    // --- 3.7 Substring constructor ---
    string s9 = s3.substr(1, 3);      // "ell" — pos=1, len=3

    // --- 3.8 From a pair of iterators ---
    vector<char> v = {'h', 'i'};
    string s10(v.begin(), v.end());   // "hi"

    // --- 3.9 From an initializer_list<char> ---
    string s11({'a', 'b', 'c'});      // "abc"

    // --- 3.10 From a string_view (C++17) ---
    string_view sv = "world";
    string s12(sv);
    string s13(sv.substr(1, 3));      // "orl"

    // --- 3.11 From std::to_string ---
    string s14 = to_string(42);       // "42"

    cout << "All constructors demonstrated. s6=" << s6 << "\n";
}

// ============================================================
// SECTION 4 : ELEMENT ACCESS
// ============================================================

void section4_element_access()
{
    string s = "hello world";

    // --- 4.1 operator[] — NO bounds check, UB if out of range ---
    cout << s[0] << "\n";              // 'h'

    // --- 4.2 at() — bounds-checked, throws std::out_of_range ---
    try
    {
        char c = s.at(100);
        (void)c;
    }
    catch (const out_of_range& e)
    {
        cout << "at() threw: " << e.what() << "\n";
    }

    // --- 4.3 front() / back() — UB on an empty string ---
    cout << "front: " << s.front() << "  back: " << s.back() << "\n";

    // --- 4.4 data() — always null-terminated since C++11 ---
    const char* p = s.data();
    cout << "data(): " << p << "\n";

    // --- 4.5 data() NON-CONST overload (C++17) — write directly through it ---
    char* mp = s.data();
    mp[0] = 'H';
    cout << "after data()[0]='H': " << s << "\n";   // Hello world

    // --- 4.6 c_str() — same guarantee as data(), const-only, C-API boundary ---
    printf("via c_str(): %s\n", s.c_str());

    // --- 4.7 operator[] AT size() is legal to READ (returns '\0'), never write ---
    cout << "s[s.size()] as char (should be 0): " << (int)s[s.size()] << "\n";
}

// ============================================================
// SECTION 5 : MODIFIERS
// ============================================================

void section5_modifiers()
{
    string s = "hello world";

    // --- 5.1 insert / erase / replace — positional editing ---
    s.erase(5, 6);              // "hello"          (pos=5, len=6)
    s.insert(5, " there");      // "hello there"
    s.replace(6, 5, "C++");     // "hello C++"
    cout << "after insert/erase/replace: " << s << "\n";

    // --- 5.2 append / operator+= / push_back ---
    string t = "abc";
    t += "def";                  // append literal
    t.append("ghi");              // append via method
    t.append(3, '!');              // append 3 copies of '!'
    t.push_back('?');               // append single char
    cout << "t: " << t << "\n";     // abcdefghi!!!?

    // --- 5.3 pop_back() — UB if empty ---
    t.pop_back();
    cout << "t after pop_back: " << t << "\n";   // abcdefghi!!!

    // --- 5.4 resize() — extend with '\0' or truncate ---
    string r = "hello";
    r.resize(3);                  // "hel" — truncate
    r.resize(8, '!');              // "hel!!!!!"
    cout << "r resized: " << r << "\n";

    // --- 5.5 clear() — size becomes 0, capacity UNCHANGED ---
    string c = "a very long string that forces a heap allocation to occur";
    size_t capBefore = c.capacity();
    c.clear();
    cout << "capacity before clear: " << capBefore
         << "  after clear: " << c.capacity() << "\n";   // capacity typically unchanged

    // --- 5.6 swap() — O(1), just exchanges internal pointers/buffers ---
    string a = "aaa", b = "bbbbbbbbbbbbbbbbbbbbbbbbb";
    a.swap(b);
    cout << "after swap, a=" << a << "\n";

    // --- 5.7 assign() — multiple forms ---
    string as;
    as.assign("hello", 3);       // "hel" — first 3 chars
    as.assign(5, 'z');            // "zzzzz"
    cout << "assign demo: " << as << "\n";
}

// ============================================================
// SECTION 6 : SEARCH OPERATIONS — THE find() FAMILY
// ============================================================

void section6_search()
{
    string s = "the quick brown fox jumps over the lazy dog";

    // All return size_t. On failure: std::string::npos (== SIZE_MAX)

    cout << "find(\"quick\"): "        << s.find("quick")            << "\n";   // 4
    cout << "find(\"quick\", 5): "     << s.find("quick", 5)          << "\n";   // npos
    cout << "find('o'): "               << s.find('o')                  << "\n";   // 12
    cout << "rfind('o'): "              << s.rfind('o')                 << "\n";   // last 'o'
    cout << "find_first_of(\"aeiou\"): " << s.find_first_of("aeiou")    << "\n";   // first vowel
    cout << "find_last_of(\"aeiou\"): "  << s.find_last_of("aeiou")     << "\n";   // last vowel
    cout << "find_first_not_of(\"the \"): " << s.find_first_not_of("the ") << "\n"; // first char not t/h/e/space

    // THE #1 TRAP with npos:
    size_t pos = s.find("xyz");
    if (pos != string::npos)          // CORRECT — compare against npos, unsigned
    {
        cout << "found\n";
    }
    else
    {
        cout << "not found (correct check)\n";
    }

    // WRONG — narrowing npos into int and comparing to -1:
    int posInt = (int)s.find("xyz");   // npos truncated — DANGEROUS, avoid this pattern
    if (posInt != -1)
    {
        cout << "this branch may fire incorrectly on some platforms\n";
    }

    // --- Substring extraction ALWAYS allocates a new string ---
    string sub = s.substr(4, 5);       // "quick" — new heap/SSO allocation
    cout << "substr: " << sub << "\n";

    // --- starts_with / ends_with (C++20) ---
    cout << "starts_with(\"the\"): " << s.starts_with("the") << "\n";
    cout << "ends_with(\"dog\"): "    << s.ends_with("dog")    << "\n";

    // --- contains (C++23) ---
#if __cplusplus > 202002L
    cout << "contains(\"fox\"): " << s.contains("fox") << "\n";
#endif
}

// ============================================================
// SECTION 7 : COMPARISON
// ============================================================

void section7_comparison()
{
    string a = "abc", b = "abd";

    cout << (a == b) << " " << (a != b) << " " << (a < b) << "\n";   // 0 1 1

    // compare() — like strcmp, returns <0, 0, >0
    cout << a.compare(b) << "\n";              // negative

    // Partial compare: compare(pos, len, other)
    cout << a.compare(1, 2, b) << "\n";        // compares a[1..2] ("bc") with b entirely

    // Case-insensitive compare — NOT built in, classic idiom:
    auto iequal = [](const string& x, const string& y) -> bool
    {
        return x.size() == y.size() &&
               equal(x.begin(), x.end(), y.begin(), [](char p, char q)
               {
                   return tolower((unsigned char)p) == tolower((unsigned char)q);
               });
    };
    cout << "iequal(\"ABC\",\"abc\"): " << iequal("ABC", "abc") << "\n";   // 1
}

// ============================================================
// SECTION 8 : CONCATENATION PERFORMANCE
// ============================================================

void section8_concatenation_perf()
{
    // --- WORST: repeated operator+ builds intermediate temporaries ---
    string a = "aaa", b = "bbb", c = "ccc";
    string worst = a + " " + b + " " + c;
    // Creates: (a+" "), then (that+b), then (that+" "), then (that+c)
    // multiple heap allocations for one logical concatenation

    // --- BETTER: operator+= with reserve() upfront ---
    string better;
    better.reserve(a.size() + b.size() + c.size() + 2);
    better += a; better += ' '; better += b; better += ' '; better += c;
    // ZERO reallocations — the reserve() eliminated every intermediate growth

    cout << worst << " | " << better << "\n";

    // --- Measured proof: concatenation WITHOUT reserve does O(log n) reallocs ---
    auto t0 = chrono::steady_clock::now();
    string grown;
    for (int i = 0; i < 100000; ++i)
    {
        grown += 'x';
    }
    auto t1 = chrono::steady_clock::now();

    string reserved;
    reserved.reserve(100000);
    for (int i = 0; i < 100000; ++i)
    {
        reserved += 'x';
    }
    auto t2 = chrono::steady_clock::now();

    auto us1 = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    auto us2 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    cout << "without reserve: " << us1 << "us   with reserve: " << us2 << "us\n";
}

// ============================================================
// SECTION 9 : ITERATORS
// ============================================================

void section9_iterators()
{
    string s = "hello";

    // Range-for — most common
    for (char c : s)
    {
        cout << c;
    }
    cout << "\n";

    // Mutating via reference in range-for
    for (char& c : s)
    {
        c = toupper((unsigned char)c);
    }
    cout << s << "\n";   // HELLO

    // Reverse iteration
    for (auto it = s.rbegin(); it != s.rend(); ++it)
    {
        cout << *it;
    }
    cout << "\n";   // OLLEH

    // STL algorithms work directly on string (it's just a char container)
    sort(s.begin(), s.end());
    cout << "sorted: " << s << "\n";

    reverse(s.begin(), s.end());
    cout << "reversed: " << s << "\n";

    int lCount = (int)count(s.begin(), s.end(), 'L');
    cout << "count of 'L': " << lCount << "\n";
}

// ============================================================
// SECTION 10 : CAPACITY
// ============================================================

void section10_capacity()
{
    string s = "hi";
    s.reserve(1000);

    cout << "size: " << s.size() << "  capacity: " << s.capacity() << "\n";
    // size=2, capacity>=1000 — reserve does NOT change size, only capacity

    s.resize(5);
    cout << "after resize(5), size: " << s.size() << "\n";   // 5

    // shrink_to_fit() — NON-BINDING request, not guaranteed
    s.shrink_to_fit();
    cout << "capacity after shrink_to_fit (implementation-defined): " << s.capacity() << "\n";

    // GUARANTEED release idiom — same swap-with-temporary trick as vector/set:
    string big(10000, 'x');
    string().swap(big);
    cout << "big capacity after swap-release: " << big.capacity() << "\n";

    cout << "max_size(): " << s.max_size() << "\n";
    cout << "empty(): " << s.empty() << "\n";
}

// ============================================================
// SECTION 11 : STRING <-> NUMBER CONVERSIONS
// ============================================================

void section11_conversions()
{
    // --- string -> number (C++11): stoi/stol/stoll/stoul/stoull/stof/stod/stold ---
    int i1 = stoi("42");
    int i2 = stoi("0xFF", nullptr, 16);   // 255 — hex
    int i3 = stoi("077", nullptr, 8);      // 63  — octal
    int i4 = stoi("11", nullptr, 2);        // 3   — binary
    cout << i1 << " " << i2 << " " << i3 << " " << i4 << "\n";

    // second param = pos AFTER the parsed number
    size_t pos;
    double d = stod("3.14 rest", &pos);
    string remainder = string("3.14 rest").substr(pos);
    cout << "d=" << d << " remainder='" << remainder << "'\n";

    // Throws std::invalid_argument (no valid conversion) or
    // std::out_of_range (value overflows the target type)
    try
    {
        stoi("9999999999999");
    }
    catch (const out_of_range&)
    {
        cout << "stoi overflow caught\n";
    }

    // --- number -> string (C++11): to_string — simple, locale-independent-ish ---
    cout << to_string(42) << " " << to_string(3.14) << "\n";   // "42" "3.140000"
    // NOTE: to_string(double) ALWAYS gives 6 decimal places — no precision control

    // --- C++17 from_chars / to_chars — FASTEST, no locale, no allocation ---
    const char* src = "12345";
    int value;
    auto [ptr, ec] = from_chars(src, src + 5, value);
    if (ec == errc{})
    {
        cout << "from_chars parsed: " << value << "\n";
    }

    char buf[32];
    auto [end, ec2] = to_chars(buf, buf + 32, 3.14159, chars_format::fixed, 2);
    string_view result(buf, end - buf);   // "3.14" — NOT null-terminated, use size explicitly
    cout << "to_chars: " << result << "\n";
}

// ============================================================
// SECTION 12 : STRING LITERALS & USER-DEFINED LITERALS
// ============================================================

void section12_literals()
{
    // --- Raw string literals (C++11) — no escape processing ---
    const char* path = R"(C:\Users\foo\bar)";      // backslashes are LITERAL
    const char* regexPattern = R"(\d+\.\d{2})";     // no double-escaping needed
    cout << path << "  " << regexPattern << "\n";

    // Custom delimiter — needed when the content itself contains ")"
    const char* tricky = R"delim(contains ) parens)delim";
    cout << tricky << "\n";

    // --- Adjacent literal concatenation (compile-time) ---
    const char* s = "Hello, " "world" "!";   // -> "Hello, world!"
    cout << s << "\n";

    // --- User-defined literals (C++14) ---
    using namespace string_literals;
    using namespace string_view_literals;

    auto str1 = "hello"s;     // type: std::string (allocates/SSO immediately)
    auto sv1  = "hello"sv;    // type: std::string_view (ZERO allocation, ever)

    // Embedded-null distinction — THIS is why ""s matters:
    const char* p2 = "hel\0lo";     // strlen-based -> only sees "hel" (3 chars)
    auto s3 = "hel\0lo"s;             // ""s KNOWS the full literal length -> 6 chars
    cout << "raw ptr len (via strlen): " << strlen(p2) << "\n";       // 3
    cout << "\"\"s literal len: " << s3.size() << "\n";                  // 6
}

// ============================================================
// SECTION 13 : std::string_view — ZERO-COPY STRING HANDLING
// ============================================================

void section13_string_view()
{
    // string_view = {const char* data, size_t size} — 16 bytes, NO ownership,
    // NO allocation, NO guaranteed null termination.

    string_view sv1 = "literal";              // from literal, zero alloc
    string_view sv2 = string("dynamic");        // from a temporary string — DANGLES after this line!
    // sv2 is now UB to use — the temporary string was destroyed at the
    // semicolon. This is THE classic string_view lifetime bug.

    char arr[] = "array";
    string_view sv3(arr, 3);                    // "arr" — first 3 chars, zero copy
    string_view sv4 = sv1.substr(1, 4);          // "iter" — zero-copy slice of sv1

    cout << sv3 << " " << sv4 << "\n";

    // Function parameter accepting ANY string type without allocating:
    auto process = [](string_view sv)
    {
        cout << "processing: " << sv << " (" << sv.size() << " chars)\n";
    };
    process("literal");                    // no allocation
    process(string("dynamic string"));       // no EXTRA allocation beyond the temp itself
    process(sv3);                            // no allocation

    // --- THE data() NULL-TERMINATION TRAP ---
    string_view sv5 = "hello";
    // open(sv5.data(), O_RDONLY);   // WRONG if sv5 came from a substr() slice —
                                     // data() is NOT guaranteed null-terminated
                                     // for an arbitrary string_view. Convert to
                                     // a real std::string first for any C API call:
    string safeCopy(sv5);
    // open(safeCopy.c_str(), O_RDONLY);   // correct
}

// ============================================================
// SECTION 14 : ENCODING — std::string IS A BYTE CONTAINER
// ============================================================

void section14_encoding()
{
    // std::string knows NOTHING about Unicode. It stores raw bytes.
    // On Linux with a UTF-8 locale (virtually always), those bytes
    // ARE typically UTF-8, but the class doesn't enforce or track this.

    string s = "café";     // Source encoded as UTF-8 on Linux
    cout << "s.size(): " << s.size() << "\n";   // 5, NOT 4!
    // 4 ASCII bytes + 'é' encoded as 2 UTF-8 bytes (0xC3 0xA9) = 5 total

    // s.size() is a BYTE count, never a CHARACTER (code point) count.
    // s[i] indexes a BYTE, not necessarily a full character.

    // Manual UTF-8 code-point counting (skip continuation bytes):
    auto utf8CodepointCount = [](string_view str) -> size_t
    {
        size_t count = 0;
        for (unsigned char c : str)
        {
            if ((c & 0xC0) != 0x80)   // NOT a continuation byte
            {
                ++count;
            }
        }
        return count;
    };
    cout << "code points in 'café': " << utf8CodepointCount(s) << "\n";   // 4
}

// ============================================================
// SECTION 15 : MOVE SEMANTICS WITH std::string
// ============================================================

void section15_move_semantics()
{
    string heapStr = "this string is definitely long enough to live on the heap";
    string moved = move(heapStr);
    // O(1) — just steals the pointer/size/capacity. heapStr is now
    // valid-but-unspecified — DO NOT read it without reassigning first.

    // SSO strings: move is EFFECTIVELY a copy — no heap buffer to steal
    string shortStr = "hi";
    string movedShort = move(shortStr);   // just copies 15 inline bytes — no real speedup

    cout << "moved: " << moved << "  movedShort: " << movedShort << "\n";

    // NEVER disable NRVO:
    auto makeGood = []() -> string
    {
        string s = "hello";
        return s;                 // NRVO — zero copy/move, constructed in caller's slot
    };
    auto makeBad = []() -> string
    {
        string s = "hello";
        return move(s);            // ANTI-PATTERN — forces a move, disables NRVO
    };
    cout << makeGood() << " " << makeBad() << "\n";
}

// ============================================================
// SECTION 16 : MODERN C++17 / C++20 / C++23 FEATURES
// ============================================================

void section16_modern_features()
{
    string s = "hello world";

    // C++17: non-const data() — write directly into the buffer
    char* mp = s.data();
    mp[0] = 'H';

    // C++20: starts_with / ends_with
    cout << s.starts_with("Hello") << " " << s.ends_with("world") << "\n";

    // C++20: std::format — safer, faster than sprintf/ostringstream
    // (requires <format> header + compiler support)
    // string msg = format("{} has {} items", "cart", 5);

    // C++20: constexpr string (limited — usable in constexpr CONTEXT only)
    // constexpr string cs = "hello";   // only valid where a constexpr context exists

    // C++23: contains()
#if __cplusplus > 202002L
    cout << s.contains("world") << "\n";
#endif

    cout << s << "\n";
}

// ============================================================
// SECTION 17 : std::string AS A KEY IN map / unordered_map
// ============================================================

void section17_as_key()
{
    // string ALREADY has both operator< (lexicographic) AND std::hash
    // specialized — unlike pair/vector/tuple (set-notes Section 22) or
    // your own structs (unordered_set-notes Section 10), you get BOTH
    // ordered AND hashed containers of string with ZERO extra setup.

    map<string, int> ordered = {{"banana", 2}, {"apple", 1}, {"cherry", 3}};
    for (const auto& [k, v] : ordered)
    {
        cout << k << "=" << v << " ";   // apple=1 banana=2 cherry=3 — sorted!
    }
    cout << "\n";

    unordered_map<string, int> hashed = {{"banana", 2}, {"apple", 1}};
    cout << "hashed lookup: " << hashed["apple"] << "\n";

    // Custom transparent hash for HETEROGENEOUS lookup (C++20) — avoids
    // constructing a temporary std::string just to look up a string_view:
    struct StringHash
    {
        using is_transparent = void;
        size_t operator()(string_view sv) const { return hash<string_view>{}(sv); }
    };
    unordered_map<string, int, StringHash, equal_to<>> transparentMap;
    transparentMap["key"] = 42;
    cout << transparentMap.find("key"sv)->second << "\n";   // no string construction for lookup
}

// ============================================================
// SECTION 18 : THREAD SAFETY
// ============================================================
//
//  Same rule as every standard container: std::string is NOT
//  thread-safe for concurrent modification.
//
//  Safe: multiple threads calling const methods (size(), c_str(),
//  operator[] for READ) on the SAME string simultaneously.
//
//  UNSAFE: any thread WRITING (operator+=, resize, clear, etc.)
//  while another thread reads or writes the SAME string object,
//  without external synchronization (mutex/shared_mutex).
//
//  A subtle historical trap: pre-C++11, some implementations used
//  COPY-ON-WRITE (COW) strings with reference counting, where even
//  "read-only" operations like operator[] on a COPY could silently
//  mutate SHARED internal state (the refcount). The C++11 standard
//  explicitly BANS this implementation strategy — every std::string
//  since C++11 has real, independent value semantics on copy. If
//  you ever work with legacy pre-C++11 codebases, this is why old
//  COW strings caused mysterious multi-threaded bugs that modern
//  std::string architecturally cannot reproduce.

// ============================================================
// SECTION 19 : MEMORY LAYOUT AND PERFORMANCE TIPS
// ============================================================
//
//  TIP 1: ALWAYS reserve() before a known bulk-append loop —
//  eliminates every intermediate reallocation (Section 8 proof).
//
//  TIP 2: Prefer string_view parameters for read-only functions —
//  accepts literals/strings/char-arrays with ZERO allocation,
//  UNLESS you need c_str()/null-termination, in which case take
//  const string&.
//
//  TIP 3: Avoid std::endl in hot logging loops — it flushes the
//  stream every call; '\n' does not. This is a string/IO-adjacent
//  but extremely common real-world performance mistake.
//
//  TIP 4: For known-short keys/tags/identifiers, exploit SSO —
//  keep them under the threshold (15 chars on GCC) and you get
//  zero heap pressure automatically, no extra code required.
//
//  TIP 5: snprintf into a stack char[] buffer, THEN construct one
//  std::string, is often faster than building via ostringstream
//  for numeric-heavy formatting in hot paths:
void section19_perf_tip5()
{
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "pid=%d status=%s", 1234, "running");
    string msg(buf, n);   // ONE allocation total (or zero, if under SSO threshold)
    cout << msg << "\n";
}

// ============================================================
// SECTION 20 : GDB DEBUGGING
// ============================================================
//
//  With libstdc++ Python pretty-printers active (same ~/.gdbinit
//  setup as your Set_Explanation.cpp Section 24):
//
//   (gdb) print s
//   $1 = "hello world"
//
//  Without pretty-printers, you'd see raw internal union fields —
//  fragile and version-dependent. Prefer a small diagnostic helper
//  using ONLY the public API, exactly as recommended for set/
//  unordered_set:

void dump_string(const string& s)
{
    cerr << "[[ string dump: size=" << s.size()
         << " capacity=" << s.capacity()
         << " sso=" << (s.data() == reinterpret_cast<const char*>(&s))
         << " content='" << s << "' ]]\n";
}

// ============================================================
// SECTION 21 : LINUX SYSTEMS PROGRAMMING WITH std::string
// ============================================================

string readFile(const string& path)
{
    ifstream f(path);
    string result;
    char buf[4096];

    while (f.read(buf, sizeof(buf)) || f.gcount())
    {
        result.append(buf, (size_t)f.gcount());   // append(ptr,count) handles embedded nulls
    }
    return result;   // NRVO — no copy
}

void section21_linux_patterns()
{
    // --- getenv() null-check-then-wrap pattern ---
    const char* homeEnv = getenv("HOME");
    string home = homeEnv ? homeEnv : "/root";
    cout << "HOME: " << home << "\n";

    // --- /proc/self/comm — process name ---
    ifstream commFile("/proc/self/comm");
    string comm;
    getline(commFile, comm);   // getline strips the trailing newline automatically
    cout << "comm: " << comm << "\n";

    // --- rfind(key, 0) == 0 idiom — starts_with equivalent pre-C++20 ---
    string line = "VmRSS:\t  12345 kB";
    string key = "VmRSS";
    if (line.rfind(key, 0) == 0)   // search for key ONLY starting at position 0
    {
        cout << "line starts with " << key << "\n";
    }
}

// ============================================================
// SECTION 22 : COMMON PATTERNS AND IDIOMS
// ============================================================

void ltrim(string& s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char c)
    {
        return !isspace(c);
    }));
}

void rtrim(string& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char c)
    {
        return !isspace(c);
    }).base(), s.end());
}

void trim(string& s)
{
    ltrim(s);
    rtrim(s);
}

vector<string> split(const string& s, char delim)
{
    vector<string> parts;
    string part;
    for (char c : s)
    {
        if (c == delim)
        {
            parts.push_back(move(part));
            part.clear();
        }
        else
        {
            part += c;
        }
    }
    parts.push_back(move(part));
    return parts;
}

string join(const vector<string>& parts, const string& delim)
{
    string result;
    for (size_t i = 0; i < parts.size(); ++i)
    {
        if (i > 0)
        {
            result += delim;
        }
        result += parts[i];
    }
    return result;
}

void toUpperInPlace(string& s)
{
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
    {
        return (char)toupper(c);
    });
}

bool isPalindrome(const string& s)
{
    int lo = 0, hi = (int)s.size() - 1;
    while (lo < hi)
    {
        if (s[lo] != s[hi])
        {
            return false;
        }
        ++lo;
        --hi;
    }
    return true;
}

void section22_idioms()
{
    string s = "  hello world  ";
    trim(s);
    cout << "trimmed: '" << s << "'\n";

    vector<string> parts = split("a,b,c,d", ',');
    string joined = join(parts, " | ");
    cout << "split+join: " << joined << "\n";

    string upper = "hello";
    toUpperInPlace(upper);
    cout << "upper: " << upper << "\n";

    cout << "isPalindrome(\"racecar\"): " << isPalindrome("racecar") << "\n";
}

// ============================================================
// SECTION 23 : 5-YEAR EXPERIENCE INTERVIEW QUESTION BANK
// ============================================================
//
// Same depth as the vector/binary-search/two-pointer/sliding-window
// banks earlier in this conversation — proofs, code, bug analysis.

// --- Q1. What is SSO, what is GCC's threshold, and how do you detect it at runtime?
// ANSWER: Small String Optimization stores short strings INLINE inside
// the string object (no heap allocation). GCC libstdc++'s threshold is
// 15 chars (object size 32 bytes). Detect via:
//     bool sso = (s.data() == reinterpret_cast<const char*>(&s));
// See section2_sso_detection() above for a full runnable proof.

// --- Q2. Why does string("hel\0lo") give size()==3 but "hel\0lo"s give size()==6?
// ANSWER: const char* constructors must call strlen() to find the length
// (they have no length parameter), and strlen stops at the FIRST '\0'.
// The ""s user-defined literal (C++14) is built from the LITERAL's known
// compile-time length, which includes everything up to the closing quote —
// embedded nulls and all. See section12_literals() for the runnable proof.

void interview_q3_capacity_growth()
{
    // --- Q3. Prove push_back / operator+= is amortized O(1) for string,
    // exactly as for vector. Show the capacity doubling empirically.
    string s;
    size_t lastCap = s.capacity();
    for (int i = 0; i < 40; ++i)
    {
        s += 'x';
        if (s.capacity() != lastCap)
        {
            cout << "len=" << s.size() << " new capacity=" << s.capacity() << "\n";
            lastCap = s.capacity();
        }
    }
    // ANSWER: identical aggregate-analysis proof as vector::push_back —
    // capacity doublings happen at 1,2,4,8,16,32... reallocations, and
    // the total copying cost across n appends is bounded by O(n), giving
    // O(1) amortized per append. The ONE difference from a raw vector<char>
    // is that a string ALSO stays under the SSO threshold for its first
    // few chars, meaning the FIRST reallocation (from inline buffer to
    // heap) only happens once size() first exceeds 15 (GCC) — everything
    // before that point costs literally zero allocation at all.
}

// --- Q4. Why is std::string::npos dangerous to store in an int, and what's the fix?
void interview_q4_npos_trap()
{
    string s = "hello";
    // npos is std::string::size_type (an UNSIGNED size_t), defined as the
    // maximum possible value of that type — effectively SIZE_MAX.
    // Assigning it to a SIGNED int silently narrows/wraps it (commonly to -1
    // on typical platforms, but this is technically implementation-defined
    // narrowing behavior, not a portable guarantee).
    int posInt = (int)s.find("xyz");    // narrowing conversion — AVOID this pattern
    size_t posCorrect = s.find("xyz");   // CORRECT — keep it as size_t
    cout << (posCorrect == string::npos) << "\n";
    // FIX: never store a find() result in int. Always keep it as size_t
    // (or auto) and compare directly against string::npos.
    (void)posInt;
}

// --- Q5. What's wrong with using strlen()/strcmp()/strcpy() interchangeably
// with std::string, and where does this bite in real Linux code?
void interview_q5_cstring_interop()
{
    string s = "hello\0world"s;   // ""s literal -> size() == 11, embedded null preserved
    cout << "s.size() (with embedded null): " << s.size() << "\n";       // 11
    cout << "strlen(s.c_str()): " << strlen(s.c_str()) << "\n";           // 5 — STOPS at the null!
    // ANSWER: c_str()/data() give you a null-terminated view, but any
    // C function that itself relies on strlen() to find the "end" will
    // see ONLY up to the first embedded '\0' — even though std::string
    // itself correctly tracks the FULL length including bytes after
    // that null. This is a genuine, real interview trap: std::string
    // supports embedded nulls; the entire C string ecosystem (strlen,
    // strcpy, printf %s) fundamentally does not.
}

// --- Q6. Why is std::string_view dangerous with temporaries? Show the exact
// UB and the fix.
string_view interview_q6_dangling_view_BAD()
{
    string local = "temporary";
    return local;   // ANTI-PATTERN: local is destroyed at return, string_view
                     // implicitly constructed from it now DANGLES — using the
                     // returned view is undefined behavior, may print garbage
                     // or crash depending on what reuses that stack memory
}
void interview_q6_dangling_view_demo()
{
    // ANSWER: the fix is either (a) return string (owning) instead of
    // string_view, or (b) if the intent is to view into an ALREADY-
    // existing, longer-lived string, accept THAT string (or a
    // string_view over it) as input rather than constructing a new
    // temporary inside the function.
    // string_view badView = interview_q6_dangling_view_BAD();   // DO NOT use badView
}

// --- Q7. Concatenation cost: prove operator+ chains cost more allocations
// than reserve()+operator+= for the same final string.
void interview_q7_concat_cost()
{
    string a(20, 'a'), b(20, 'b'), c(20, 'c');   // each individually still SSO-sized... actually 20>15, heap

    // operator+ chain: (a+" ") creates temp1, (temp1+b) creates temp2, etc.
    // — potentially SEVERAL heap allocations for what is logically ONE result.
    string chained = a + " " + b + " " + c;

    // reserve()+= : exactly ONE allocation (from reserve), zero more after.
    string built;
    built.reserve(a.size() + b.size() + c.size() + 2);
    built += a; built += ' '; built += b; built += ' '; built += c;

    cout << chained << " == " << built << " : " << (chained == built) << "\n";
    // ANSWER: same FINAL content, meaningfully different ALLOCATION COUNT —
    // the exact same "expensive middle" argument used in your vector bank's
    // push_back-without-reserve analysis.
}

// --- Q8. Why does std::string have NO reserve()-equivalent memory pooling
// benefit the way a custom allocator gives vector, and what's the closest
// lever you actually have?
// ANSWER: string DOES have reserve() (unlike std::set, per your Set notes
// Section 35.1) — because string's heap-mode layout is a flat contiguous
// buffer, structurally identical to vector's. The lever you have is
// EXACTLY reserve() itself; there is no additional pooling trick beyond
// what a custom Allocator template parameter (basic_string<char, traits,
// MyAllocator>) would already give any other contiguous-buffer container.

// --- Q9. Explain how std::hash<std::string> works well enough to combine
// it manually if you needed a hash for pair<string,string>.
void interview_q9_hash_combine_string()
{
    struct PairStringHash
    {
        size_t operator()(const pair<string,string>& p) const noexcept
        {
            size_t h1 = hash<string>{}(p.first);
            size_t h2 = hash<string>{}(p.second);
            // golden-ratio hash_combine, same technique as your
            // Unordered_set_Explanation.cpp Section 22
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
    unordered_map<pair<string,string>, int, PairStringHash> m;
    m[{"a","b"}] = 1;
    cout << m[{"a","b"}] << "\n";
    // ANSWER: std::hash<string> is a well-tested, well-distributed hash
    // over the string's bytes (implementation-specific algorithm — often
    // a variant of FNV or MurmurHash-style mixing on GCC/Clang), but
    // there is NO std::hash for pair<string,string> — same rule as
    // pair<int,int> from your unordered_set notes: you must combine
    // manually, string being no exception to that composability gap.
}

// --- Q10. Design a case-insensitive std::string using char_traits — the
// most senior-level string question in this bank.
struct CaseInsensitiveTraits : char_traits<char>
{
    static bool eq(char a, char b)
    {
        return tolower((unsigned char)a) == tolower((unsigned char)b);
    }
    static bool lt(char a, char b)
    {
        return tolower((unsigned char)a) < tolower((unsigned char)b);
    }
    static int compare(const char* a, const char* b, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            char ca = (char)tolower((unsigned char)a[i]);
            char cb = (char)tolower((unsigned char)b[i]);
            if (ca < cb) return -1;
            if (ca > cb) return  1;
        }
        return 0;
    }
    static const char* find(const char* s, size_t n, char a)
    {
        char target = (char)tolower((unsigned char)a);
        for (size_t i = 0; i < n; ++i)
        {
            if (tolower((unsigned char)s[i]) == target)
            {
                return s + i;
            }
        }
        return nullptr;
    }
};
using ci_string = basic_string<char, CaseInsensitiveTraits>;

ostream& operator<<(ostream& os, const ci_string& s)
{
    return os.write(s.data(), (streamsize)s.size());
}

void interview_q10_case_insensitive_string()
{
    ci_string a = "Hello";
    ci_string b = "HELLO";
    cout << "a == b (case-insensitive): " << (a == b) << "\n";   // true

    ci_string haystack = "Hello World";
    auto pos = haystack.find("WORLD");
    cout << "case-insensitive find pos: " << pos << "\n";   // 6, found despite case mismatch

    cout << haystack << "\n";
    // ANSWER: this is THE canonical demonstration that ALL of string's
    // find/compare/== behavior is entirely delegated to the Traits
    // template parameter — you never touch string's actual member
    // function implementations to get this behavior; you customize the
    // comparison primitives it calls internally. Very few candidates
    // know this is even possible, which is exactly why it's a strong
    // signal question.
}

// --- Q11. Why can string be used as a map/set key with zero setup, while
// a custom struct needs operator< (set) or a hash functor (unordered_set)?
// ANSWER: std::string already provides BOTH operator< (lexicographic,
// byte-by-byte via Traits::compare) AND a std::hash<string> specialization
// in the standard library — it's a "batteries included" type for both
// ordered and hashed containers, unlike a user-defined struct which gets
// neither for free (see section17_as_key() above for the runnable proof).

// --- Q12. Explain exactly why substr() always allocates, and how string_view
// avoids that for read-only slicing.
void interview_q12_substr_vs_view()
{
    string s = "the quick brown fox";
    string sub = s.substr(4, 5);          // "quick" — ALLOCATES a brand new string,
                                            // even if the result is short enough for SSO
                                            // (construction still runs; SSO just means
                                            // that construction skips the HEAP, not that
                                            // it skips work entirely)
    string_view sv = string_view(s).substr(4, 5);   // "quick" — ZERO allocation, just adjusts
                                                       // the {pointer,length} pair
    cout << sub << " vs " << sv << "\n";
    // ANSWER: substr() on string is REQUIRED to return a new, independently-
    // owned string object — it cannot alias the original's storage, because
    // the original might be modified or destroyed afterward while the
    // substring should remain valid and independent. string_view's substr()
    // has no such requirement — it's non-owning BY DESIGN, so slicing is
    // just pointer-and-length arithmetic, genuinely free.

    (void)sub;
}

// ============================================================
// SECTION 24 : COMPLETE FUNCTION REFERENCE TABLE
//              Function | Description | Return value | How to use
// ============================================================

// ---------------- ELEMENT ACCESS ----------------

// FUNCTION      : operator[](pos)
// DESCRIPTION   : Access character at pos. NO bounds check. Reading
//                 s[s.size()] is legal and returns '\0' (guaranteed
//                 since C++11); any other out-of-range access is UB
// RETURN VALUE  : char& (non-const) or const char& (const)
// HOW TO USE    :
//     char c = s[0];
//     s[0] = 'X';   // legal on non-const string

// FUNCTION      : at(pos)
// DESCRIPTION   : Bounds-checked access
// RETURN VALUE  : char& — throws std::out_of_range if pos >= size()
// HOW TO USE    :
//     try { char c = s.at(100); }
//     catch (const std::out_of_range& e) { /* handle */ }

// FUNCTION      : front() / back()
// DESCRIPTION   : First / last character. UB if the string is empty
// RETURN VALUE  : char&
// HOW TO USE    :
//     char first = s.front();
//     char last  = s.back();

// FUNCTION      : data()
// DESCRIPTION   : Pointer to the internal buffer. Always null-terminated
//                 since C++11. Non-const overload (write-through)
//                 available since C++17
// RETURN VALUE  : const char* (or char* since C++17)
// HOW TO USE    :
//     const char* p = s.data();
//     char* mp = s.data();       // C++17 — mp[0] = 'X'; is legal

// FUNCTION      : c_str()
// DESCRIPTION   : Same guarantee as data() — always null-terminated.
//                 Use at C API boundaries expecting a C string
// RETURN VALUE  : const char*
// HOW TO USE    :
//     open(path.c_str(), O_RDONLY);


// ---------------- MODIFIERS ----------------

// FUNCTION      : insert(pos, str)
// DESCRIPTION   : Insert str at position pos, shifting the rest right
// RETURN VALUE  : string& (*this)
// HOW TO USE    :
//     s.insert(5, " there");

// FUNCTION      : erase(pos, len)
// DESCRIPTION   : Remove len characters starting at pos
// RETURN VALUE  : string& (*this)
// HOW TO USE    :
//     s.erase(5, 6);

// FUNCTION      : replace(pos, len, str)
// DESCRIPTION   : Replace len characters at pos with str — handles
//                 the underlying resize automatically
// RETURN VALUE  : string& (*this)
// HOW TO USE    :
//     s.replace(6, 5, "C++");

// FUNCTION      : append(str) / operator+=
// DESCRIPTION   : Append str/char/n-copies-of-char to the end
// RETURN VALUE  : string& (*this)
// HOW TO USE    :
//     s += " world";
//     s.append(3, '!');

// FUNCTION      : push_back(c) / pop_back()
// DESCRIPTION   : Append/remove a single character at the end.
//                 pop_back() is UB if the string is empty
// RETURN VALUE  : void
// HOW TO USE    :
//     s.push_back('!');
//     s.pop_back();

// FUNCTION      : resize(n) / resize(n, c)
// DESCRIPTION   : Truncate or extend to size n. Extension fills with
//                 '\0' (or char c if given)
// RETURN VALUE  : void
// HOW TO USE    :
//     s.resize(5);
//     s.resize(8, '!');

// FUNCTION      : clear()
// DESCRIPTION   : Sets size to 0. Does NOT release capacity
// RETURN VALUE  : void
// HOW TO USE    :
//     s.clear();

// FUNCTION      : swap(other)
// DESCRIPTION   : O(1) exchange of internal buffers/state
// RETURN VALUE  : void
// HOW TO USE    :
//     a.swap(b);

// FUNCTION      : assign(...)
// DESCRIPTION   : Replace the entire content — several overloads
//                 (from a C-string+len, n-copies-of-char, a range, etc.)
// RETURN VALUE  : string& (*this)
// HOW TO USE    :
//     s.assign("hello", 3);   // "hel"
//     s.assign(5, 'z');        // "zzzzz"


// ---------------- SEARCH (all return npos on failure) ----------------

// FUNCTION      : find(str, pos=0)
// DESCRIPTION   : First occurrence of str at or after pos
// RETURN VALUE  : size_t (or std::string::npos)
// HOW TO USE    :
//     size_t p = s.find("quick");
//     if (p != std::string::npos) { /* found */ }

// FUNCTION      : rfind(str, pos=npos)
// DESCRIPTION   : LAST occurrence of str at or before pos — searches
//                 backward from the end (or from pos) toward the start
// RETURN VALUE  : size_t (or npos)
// HOW TO USE    :
//     size_t p = s.rfind('o');

// FUNCTION      : find_first_of(chars) / find_last_of(chars)
// DESCRIPTION   : First/last position of ANY character in the given set
// RETURN VALUE  : size_t (or npos)
// HOW TO USE    :
//     size_t vowel = s.find_first_of("aeiou");

// FUNCTION      : find_first_not_of(chars) / find_last_not_of(chars)
// DESCRIPTION   : First/last position of a character NOT in the given set
// RETURN VALUE  : size_t (or npos)
// HOW TO USE    :
//     size_t p = s.find_first_not_of(" \t\n");   // skip leading whitespace

// FUNCTION      : substr(pos=0, len=npos)
// DESCRIPTION   : Extract a NEW string — ALWAYS allocates (or SSO-
//                 constructs) a fresh object, never aliases the original
// RETURN VALUE  : std::string (new object)
// HOW TO USE    :
//     std::string word = s.substr(4, 5);

// FUNCTION      : starts_with(str) / ends_with(str)          [C++20]
// DESCRIPTION   : Prefix / suffix check
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (s.starts_with("http")) { /* ... */ }

// FUNCTION      : contains(str)                              [C++23]
// DESCRIPTION   : Substring existence check
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (s.contains("error")) { /* ... */ }


// ---------------- COMPARISON ----------------

// FUNCTION      : compare(other)
// DESCRIPTION   : Like strcmp — lexicographic comparison
// RETURN VALUE  : int — <0, 0, or >0
// HOW TO USE    :
//     if (a.compare(b) == 0) { /* equal */ }

// FUNCTION      : operator==, !=, <, <=, >, >=
// DESCRIPTION   : Lexicographic comparison operators
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (a == b) { /* ... */ }


// ---------------- CAPACITY ----------------

// FUNCTION      : size() / length()
// DESCRIPTION   : Number of characters (BYTES, not code points) —
//                 the two names are exact synonyms, O(1)
// RETURN VALUE  : size_t
// HOW TO USE    :
//     std::cout << s.size();

// FUNCTION      : empty()
// DESCRIPTION   : True if size() == 0
// RETURN VALUE  : bool
// HOW TO USE    :
//     if (s.empty()) { /* ... */ }

// FUNCTION      : capacity()
// DESCRIPTION   : Current allocated buffer size (>= size())
// RETURN VALUE  : size_t
// HOW TO USE    :
//     std::cout << s.capacity();

// FUNCTION      : reserve(n)
// DESCRIPTION   : Pre-allocate for at least n characters, avoiding
//                 reallocations during a known bulk-append
// RETURN VALUE  : void
// HOW TO USE    :
//     s.reserve(1000);

// FUNCTION      : shrink_to_fit()
// DESCRIPTION   : NON-BINDING request to reduce capacity to size()
// RETURN VALUE  : void
// HOW TO USE    :
//     s.shrink_to_fit();

// FUNCTION      : max_size()
// DESCRIPTION   : Theoretical maximum character count
// RETURN VALUE  : size_t
// HOW TO USE    :
//     std::cout << s.max_size();


// ---------------- ITERATORS ----------------

// FUNCTION      : begin()/end(), rbegin()/rend()
// DESCRIPTION   : Forward and reverse iteration (RandomAccessIterator —
//                 stronger than set's Bidirectional, matches vector)
// RETURN VALUE  : iterator / reverse_iterator
// HOW TO USE    :
//     for (char c : s) { /* forward */ }
//     for (auto it = s.rbegin(); it != s.rend(); ++it) { /* reverse */ }


// ---------------- NON-MEMBER / FREE FUNCTIONS ----------------

// FUNCTION      : std::stoi / stol / stoll / stoul / stoull / stof / stod / stold
// DESCRIPTION   : Parse a string into a numeric type. Throws
//                 invalid_argument or out_of_range on failure
// RETURN VALUE  : the numeric type requested
// HOW TO USE    :
//     int x = std::stoi("42");

// FUNCTION      : std::to_string(number)
// DESCRIPTION   : Convert a numeric type to string (fixed 6-decimal
//                 precision for floating point — no format control)
// RETURN VALUE  : std::string
// HOW TO USE    :
//     std::string s = std::to_string(42);

// FUNCTION      : std::from_chars / std::to_chars              [C++17]
// DESCRIPTION   : Fastest, locale-independent, allocation-free
//                 numeric parsing/formatting into a raw buffer
// RETURN VALUE  : struct with {ptr/end, errc} — check ec == errc{}
// HOW TO USE    :
//     auto [ptr, ec] = std::from_chars(buf, buf+n, value);

// FUNCTION      : std::getline(stream, str)
// DESCRIPTION   : Read a line from a stream into str, stripping the
//                 trailing newline automatically
// RETURN VALUE  : the stream reference (for chaining/bool checks)
// HOW TO USE    :
//     std::string line;
//     while (std::getline(file, line)) { /* process line */ }

// FUNCTION      : std::erase_if(str, pred)                     [C++20]
// DESCRIPTION   : Removes every character for which pred returns true
// RETURN VALUE  : size_t — number of characters erased
// HOW TO USE    :
//     std::erase_if(s, [](char c) { return std::isspace(c); });

// ============================================================
// END OF SECTION 24
// ============================================================


// ============================================================
// SECTION 25 : PRACTICE PROBLEM BANK — BY PATTERN,
//              ACROSS ALL 5 PLATFORMS
// (LC = LeetCode, GFG = GeeksforGeeks, STV = Striver A2Z/SDE Sheet,
//  CN = Coding Ninjas Studio, NKR = Naukri Coding 360)
// ============================================================

// --- Pattern 1 — Basic Manipulation / Reversal / Case ---
//  Platform | Problem                                    | Difficulty
//  LC       | #344 Reverse String                         | Easy
//  LC       | #541 Reverse String II                       | Easy
//  LC       | #345 Reverse Vowels of a String                | Easy
//  LC       | #557 Reverse Words in a String III              | Easy
//  LC       | #58 Length of Last Word                          | Easy
//  GFG      | "Reverse a String"                                | Easy
//  STV      | "String Basics" Day                                 | Easy
//  CN       | "String Reversal Problems"                           | Easy
//  NKR      | Basic string manipulation set                         | Easy

// --- Pattern 2 — Anagrams & Frequency Counting ---
//  Platform | Problem                                    | Difficulty
//  LC       | #242 Valid Anagram                          | Easy
//  LC       | #49 Group Anagrams                            | Medium
//  LC       | #438 Find All Anagrams in a String              | Medium
//  LC       | #567 Permutation in String                        | Medium
//  LC       | #383 Ransom Note                                    | Easy
//  GFG      | "Check if two strings are anagrams"                   | Easy
//  STV      | "Anagram Problems" Day                                  | Medium
//  CN       | "Group Anagrams Problem"                                  | Medium

// --- Pattern 3 — Palindromes ---
//  Platform | Problem                                    | Difficulty
//  LC       | #125 Valid Palindrome                        | Easy
//  LC       | #680 Valid Palindrome II                       | Easy
//  LC       | #5 Longest Palindromic Substring                 | Medium
//  LC       | #647 Palindromic Substrings                        | Medium
//  LC       | #131 Palindrome Partitioning                         | Medium
//  LC       | #214 Shortest Palindrome                              | Hard
//  GFG      | "Check for Palindrome"                                  | Easy
//  STV      | "Palindrome Substrings" Day                               | Medium
//  CN       | "Longest Palindromic Substring"                             | Medium

// --- Pattern 4 — Sliding Window / Substring (already covered in depth
//     in the Sliding Window bank earlier in this conversation) ---
//  Platform | Problem                                    | Difficulty
//  LC       | #3 Longest Substring Without Repeating Characters | Medium
//  LC       | #76 Minimum Window Substring                        | Hard
//  LC       | #438 Find All Anagrams in a String                    | Medium
//  LC       | #30 Substring with Concatenation of All Words          | Hard
//  GFG      | "Longest Substring Without Repeating Characters"          | Medium
//  STV      | "Sliding Window on Strings" Day                            | Hard

// --- Pattern 5 — Parsing / Tokenizing / Compression ---
//  Platform | Problem                                    | Difficulty
//  LC       | #443 String Compression                     | Medium
//  LC       | #394 Decode String                            | Medium
//  LC       | #6 Zigzag Conversion                            | Medium
//  LC       | #8 String to Integer (atoi)                       | Medium
//  LC       | #468 Validate IP Address                            | Medium
//  LC       | #71 Simplify Path                                     | Medium
//  GFG      | "Implement atoi"                                        | Medium
//  STV      | "String Parsing Problems" Day                             | Medium
//  CN       | "Decode String / Compression"                               | Medium

// --- Pattern 6 — Pattern Matching / Substring Search Algorithms ---
//  Platform | Problem                                    | Difficulty
//  LC       | #28 Find the Index of the First Occurrence     | Easy
//  LC       | #459 Repeated Substring Pattern                   | Easy
//  LC       | #1392 Longest Happy Prefix (KMP failure function)   | Hard
//  GFG      | "KMP Algorithm for Pattern Searching"                  | Hard
//  GFG      | "Rabin-Karp Algorithm"                                   | Hard
//  GFG      | "Z Algorithm for Pattern Searching"                        | Hard
//  STV      | "String Matching Algorithms" Day                             | Hard
//  CN       | "KMP Pattern Matching"                                         | Hard

// --- Pattern 7 — DP on Strings ---
//  Platform | Problem                                    | Difficulty
//  LC       | #1143 Longest Common Subsequence               | Medium
//  LC       | #72 Edit Distance                                | Hard
//  LC       | #10 Regular Expression Matching                   | Hard
//  LC       | #44 Wildcard Matching                                | Hard
//  LC       | #97 Interleaving String                               | Medium
//  LC       | #516 Longest Palindromic Subsequence                    | Medium
//  GFG      | "Longest Common Subsequence"                              | Medium
//  STV      | "DP on Strings" Day (full section)                          | Hard
//  CN       | "Edit Distance / LCS Problems"                                | Hard

// --- Pattern 8 — Two Pointers / Sorting on Strings ---
//  Platform | Problem                                    | Difficulty
//  LC       | #151 Reverse Words in a String                  | Medium
//  LC       | #392 Is Subsequence                               | Easy
//  LC       | #521 Longest Uncommon Subsequence I                 | Easy
//  LC       | #14 Longest Common Prefix                             | Easy
//  GFG      | "Longest Common Prefix using Sorting"                   | Easy
//  STV      | "Two Pointer on Strings" Day                               | Easy

// --- Pattern 9 — Advanced / Mixed / Trie-Based ---
//  Platform | Problem                                    | Difficulty
//  LC       | #208 Implement Trie (Prefix Tree)               | Medium
//  LC       | #212 Word Search II                               | Hard
//  LC       | #472 Concatenated Words                             | Hard
//  LC       | #316 Remove Duplicate Letters                        | Medium
//  LC       | #227 Basic Calculator II                               | Medium
//  LC       | #1268 Search Suggestions System                         | Medium
//  GFG      | "Trie Data Structure Implementation"                       | Medium
//  STV      | "Trie" Sheet Day                                              | Hard
//  CN       | "Word Search Trie Problems"                                     | Hard
//  NKR       | Advanced string + trie assignment set                          | Hard

// ============================================================
// END OF SECTION 25
// ============================================================


// ============================================================
// SECTION 26 : QUICK-FIRE ORAL DRILL (2-minute recap)
// ============================================================
//
// sizeof(string)            -> 32 bytes on GCC 64-bit (3 words + SSO union)
// SSO threshold              -> 15 chars (GCC/MSVC), 22 chars (Clang libc++)
// SSO detection               -> s.data() == (const char*)&s
// Embedded null trap            -> const char* ctor uses strlen -> stops early;
//                                 ""s literal knows the TRUE compile-time length
// npos danger                    -> NEVER narrow find() result into int; keep as size_t
// substr() vs string_view slice   -> substr ALWAYS allocates; string_view NEVER does
// string_view dangling             -> never return/store a view into a temporary
// data()/c_str() null-term         -> guaranteed since C++11, BOTH functions, same pointer
// Non-const data()                  -> C++17 — write directly, no operator[] needed
// concat cost                        -> operator+ chains = multiple temp allocations;
//                                      reserve()+=  = exactly one
// Move on SSO string                   -> effectively a COPY, no heap buffer to steal
// return std::move(local)                -> ANTI-PATTERN, disables NRVO — never do it
// map<string,int> vs unordered_map        -> BOTH work with zero setup — string has
//                                          operator< AND std::hash built in already
// Traits customization                     -> case-insensitive string via custom
//                                          char_traits — no member function rewritten
// C-string interop bug                       -> strlen/strcpy STOP at first embedded '\0';
//                                          std::string does NOT
// Encoding                                     -> size() is BYTES, not Unicode code points
// Thread safety                                  -> not safe for concurrent writes; pre-C++11
//                                          COW implementations had EXTRA hidden hazards,
//                                          banned by the C++11 standard
// getenv/  /proc pattern                          -> always null-check getenv() before wrapping
//                                          in a string; getline() auto-strips the newline

// ============================================================
// SECTION 27 : FULL WORKING DEMO
// ============================================================

int main()
{
    cout << "===== std::string Complete Demo =====\n\n";

    section2_sso_detection();
    section3_construction();
    section4_element_access();
    section5_modifiers();
    section6_search();
    section7_comparison();
    section8_concatenation_perf();
    section9_iterators();
    section10_capacity();
    section11_conversions();
    section12_literals();
    section13_string_view();
    section14_encoding();
    section15_move_semantics();
    section16_modern_features();
    section17_as_key();
    section19_perf_tip5();
    section21_linux_patterns();
    section22_idioms();

    cout << "\n===== Interview question demos =====\n\n";
    interview_q3_capacity_growth();
    interview_q4_npos_trap();
    interview_q5_cstring_interop();
    interview_q6_dangling_view_demo();
    interview_q7_concat_cost();
    interview_q9_hash_combine_string();
    interview_q10_case_insensitive_string();
    interview_q12_substr_vs_view();

    string demo = "gdb dump example";
    dump_string(demo);

    cout << "\n===== Done =====\n";
    return 0;
}

// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//          -fsanitize=address,undefined String_Explanation.cpp -o string_demo
// ============================================================

// ============================================================
// ADD THIS INCLUDE TO THE TOP OF YOUR EXISTING FILE
// ============================================================
#include <format>   // std::format, std::format_to, std::formatted_size (C++20)
// NOTE: std::print/std::println need <print> and a C++23 stdlib
// (GCC 14+/Clang 17+ roughly) — shown as comments below since your
// toolchain may not have it yet; check before uncommenting.

// ============================================================
// SECTION 28 : KMP ALGORITHM (Knuth-Morris-Pratt) — PATTERN MATCHING
// ============================================================
//
//  PROBLEM: find all occurrences of pattern P (length m) in text T
//  (length n).
//
//  NAIVE APPROACH: try every starting position, compare char by char.
//  Worst case O(n*m) — e.g. text="aaaaaaaaaaaaaaab", pattern="aaaab"
//  re-examines almost the same characters at every shifted start.
//
//  KMP's INSIGHT: when a mismatch occurs after matching k characters,
//  we ALREADY KNOW those k characters — we don't need to re-compare
//  them from scratch at the next shift. The FAILURE FUNCTION (LPS —
//  Longest proper Prefix which is also a Suffix) precomputes, for
//  every prefix of the PATTERN itself, how much of a match we can
//  "carry forward" without ever re-reading the TEXT.
//
//  LPS[i] = length of the longest proper prefix of pattern[0..i]
//           that is ALSO a proper suffix of pattern[0..i]
//  ("proper" means the whole substring itself doesn't count)

vector<int> buildLPS(const string& pattern)
{
    int m = (int)pattern.size();
    vector<int> lps(m, 0);
    int len = 0;   // length of the previous longest prefix-suffix match
    int i = 1;

    while (i < m)
    {
        if (pattern[i] == pattern[len])
        {
            ++len;
            lps[i] = len;
            ++i;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];   // fall back — DON'T advance i
            }
            else
            {
                lps[i] = 0;
                ++i;
            }
        }
    }
    return lps;
}

// TRACED EXAMPLE — pattern = "ABABCABAB" (classic GFG reference case):
//
//  index:   0 1 2 3 4 5 6 7 8
//  pattern: A B A B C A B A B
//  lps:     0 0 1 2 0 1 2 3 4
//
//  Walkthrough of the interesting steps:
//    i=4: pattern[4]='C' vs pattern[len=2]='A' -> mismatch, len!=0
//         -> len = lps[1] = 0 -> pattern[4]='C' vs pattern[0]='A'
//         -> mismatch, len==0 -> lps[4]=0
//    i=8: pattern[8]='B' vs pattern[len=3]='B' -> match -> len=4,
//         lps[8]=4  (the whole "ABAB" prefix reappears as a suffix)

vector<int> kmpSearch(const string& text, const string& pattern)
{
    vector<int> matches;
    int n = (int)text.size();
    int m = (int)pattern.size();

    if (m == 0 || m > n)
    {
        return matches;
    }

    vector<int> lps = buildLPS(pattern);
    int i = 0;   // index into text
    int j = 0;   // index into pattern

    while (i < n)
    {
        if (text[i] == pattern[j])
        {
            ++i;
            ++j;

            if (j == m)
            {
                matches.push_back(i - j);   // full match found, ending at i-1
                j = lps[j - 1];               // look for the NEXT overlapping match
            }
        }
        else if (j != 0)
        {
            j = lps[j - 1];   // fall back using the precomputed table — NEVER moves 'i' backward
        }
        else
        {
            ++i;   // no match at all, no fallback possible — just advance
        }
    }

    return matches;
}

// COMPLEXITY PROOF — O(n + m), same amortized-analysis style as the
// two-pointer / sliding-window proofs earlier in this conversation:
//
//  buildLPS: 'i' only ever increases (bounded by m total increments).
//  'len' decreases (via lps[len-1] < len) ONLY in the mismatch branch,
//  and every decrease of 'len' must have been preceded by a matching
//  increase earlier — so the TOTAL number of 'len' decreases across
//  the whole function is bounded by the total number of increases,
//  which is itself bounded by m. Total work: O(m).
//
//  kmpSearch: 'i' NEVER decreases, ever — it only increases, bounded
//  by n. 'j' decreases (via lps[j-1]) only in the mismatch-with-
//  fallback branch, and by the SAME argument as above, total 'j'
//  decreases are bounded by total 'j' increases, which are themselves
//  bounded by the total increases of 'i' (since j only grows when i
//  also grows, in the match branch). Total work: O(n + m).

void section28_kmp_demo()
{
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";

    vector<int> lps = buildLPS(pattern);
    cout << "LPS array: ";
    for (int x : lps) cout << x << " ";
    cout << "\n";

    vector<int> matches = kmpSearch(text, pattern);
    cout << "Matches found at index: ";
    for (int idx : matches) cout << idx << " ";
    cout << "\n";   // 10
}

// ============================================================
// SECTION 29 : RABIN-KARP ALGORITHM — ROLLING HASH
// ============================================================
//
//  INSIGHT: instead of comparing characters directly, compute a
//  POLYNOMIAL HASH of each window and compare HASHES first — an O(1)
//  integer comparison instead of an O(m) character-by-character one.
//  The hash of the NEXT window is computed incrementally ("rolling")
//  from the PREVIOUS window's hash in O(1), rather than recomputed
//  from scratch.
//
//  hash(s[i..i+m-1]) = s[i]*B^(m-1) + s[i+1]*B^(m-2) + ... + s[i+m-1]*B^0  (mod M)
//
//  ROLLING UPDATE (remove leftmost char, add new rightmost char):
//  hash(s[i+1..i+m]) = (hash(s[i..i+m-1]) - s[i]*B^(m-1)) * B + s[i+m]  (mod M)

vector<int> rabinKarpSearch(const string& text, const string& pattern)
{
    vector<int> matches;
    int n = (int)text.size();
    int m = (int)pattern.size();

    if (m == 0 || m > n)
    {
        return matches;
    }

    const long long BASE = 256;
    const long long MOD = 1000000007LL;

    long long patternHash = 0;
    long long windowHash = 0;
    long long highOrder = 1;   // will become BASE^(m-1) mod M

    for (int k = 0; k < m - 1; ++k)
    {
        highOrder = (highOrder * BASE) % MOD;
    }

    for (int k = 0; k < m; ++k)
    {
        patternHash = (patternHash * BASE + (unsigned char)pattern[k]) % MOD;
        windowHash  = (windowHash  * BASE + (unsigned char)text[k])    % MOD;
    }

    for (int i = 0; i <= n - m; ++i)
    {
        if (patternHash == windowHash)
        {
            // HASH MATCH IS NOT PROOF OF A REAL MATCH — different
            // strings CAN collide to the same hash value ("spurious
            // hit"). ALWAYS verify with a direct comparison before
            // trusting it. Skipping this verification is a genuine,
            // common correctness bug in naive Rabin-Karp implementations.
            if (text.compare(i, m, pattern) == 0)
            {
                matches.push_back(i);
            }
        }

        if (i < n - m)
        {
            windowHash = (windowHash - (unsigned char)text[i] * highOrder % MOD + MOD) % MOD;
            windowHash = (windowHash * BASE + (unsigned char)text[i + m]) % MOD;
        }
    }

    return matches;
}

// WHY "+ MOD" BEFORE THE FINAL "% MOD" ABOVE:
// C++'s % operator can return a NEGATIVE result when the left operand
// is negative (unlike Python's %, which always returns non-negative
// for a positive modulus). Subtracting a large term can legitimately
// produce a negative intermediate value here — adding MOD before the
// final modulo pulls it back into the valid [0, MOD) range. Forgetting
// this is a classic, silent Rabin-Karp bug: the hash comparison starts
// failing to match things it correctly should, with no crash at all.

// COMPLEXITY:
//  Average case: O(n + m) — hash comparison is O(1), verification is
//  O(m) but happens rarely (only on genuine hash collisions or real
//  matches, both rare for a well-chosen BASE/MOD with random-ish input).
//  Worst case: O(n*m) — an ADVERSARIAL input engineered to collide
//  hashes on every window (or a poorly chosen MOD with many small
//  factors) forces verification on every single window, degrading to
//  the naive algorithm's complexity. This worst-case risk is EXACTLY
//  why KMP (guaranteed O(n+m), no adversarial degradation) is generally
//  preferred over Rabin-Karp for single-pattern matching — Rabin-Karp's
//  real strength is MULTI-PATTERN matching (hashing many patterns into
//  a set, checking each window's hash against that set in O(1) average).

void section29_rabin_karp_demo()
{
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";

    vector<int> matches = rabinKarpSearch(text, pattern);
    cout << "Rabin-Karp matches at index: ";
    for (int idx : matches) cout << idx << " ";
    cout << "\n";   // 10 — same result as KMP, different mechanism
}

// ============================================================
// SECTION 30 : Z-ALGORITHM — Z-ARRAY CONSTRUCTION & MATCHING
// ============================================================
//
//  Z[i] = length of the longest substring starting at index i that
//  is ALSO a prefix of the whole string. (Z[0] is conventionally left
//  undefined/0 — comparing the string to itself is trivially useless.)
//
//  Built in O(n) using a maintained window [l, r] — the rightmost
//  segment discovered so far that matches a prefix — to AVOID
//  re-comparing characters already known to match.

vector<int> buildZArray(const string& s)
{
    int n = (int)s.size();
    vector<int> z(n, 0);
    int l = 0, r = 0;   // [l, r) conceptually — rightmost prefix-matching window found so far

    for (int i = 1; i < n; ++i)
    {
        if (i < r)
        {
            // 'i' is INSIDE a previously-found window — reuse already-known info
            z[i] = min(r - i, z[i - l]);
        }

        // Try to EXTEND further, one character at a time, past whatever
        // the reused value already gave us
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
        {
            ++z[i];
        }

        if (i + z[i] > r)
        {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

// TRACED EXAMPLE — s = "aaabaab":
//
//  index: 0 1 2 3 4 5 6
//  s:     a a a b a a b
//  z:     _ 2 1 0 2 1 0
//
//  z[1]=2: "aa" (s[1..2]) matches the prefix "aa" — then s[3]='b' breaks it
//  z[4]=2: "aa" (s[4..5]) matches the prefix "aa" — then s[6]='b' breaks it
//  (z[3]=0, z[6]=0 because 'b' doesn't match the prefix's first char 'a')

vector<int> zSearch(const string& text, const string& pattern)
{
    // Concatenate pattern + SEPARATOR (a char guaranteed absent from
    // both) + text, then any position where Z equals the pattern's
    // length marks a full match.
    string combined = pattern + '\x01' + text;
    vector<int> z = buildZArray(combined);
    vector<int> matches;
    int m = (int)pattern.size();

    for (int i = m + 1; i < (int)combined.size(); ++i)
    {
        if (z[i] == m)
        {
            matches.push_back(i - m - 1);   // translate back to an index into 'text'
        }
    }

    return matches;
}

// COMPLEXITY: O(n + m) — identical window-based amortized argument as
// KMP's proof: [l, r] only ever moves RIGHTWARD across the whole run,
// so the total extension work done inside the while loop, summed
// across ALL iterations of the outer for loop, is bounded by n.
//
// Z-ALGORITHM vs KMP — when to reach for which:
//  Z-array is arguably more INTUITIVE (one array, one clear meaning:
//  "how much of the prefix matches starting here") and generalizes
//  cleanly to problems that need "for every position, how far does a
//  prefix-match extend" as a standalone tool (not just pattern search)
//  — e.g. LC #1392 Longest Happy Prefix can be solved with EITHER the
//  KMP failure function directly (lps.back()) OR a Z-array approach.
//  KMP's failure function is more specialized but has a slight edge
//  in classic single-pattern streaming search (no need to build a
//  concatenated string with a separator).

void section30_z_algorithm_demo()
{
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";

    vector<int> matches = zSearch(text, pattern);
    cout << "Z-algorithm matches at index: ";
    for (int idx : matches) cout << idx << " ";
    cout << "\n";   // 10 — same result as KMP and Rabin-Karp, third mechanism
}

// ============================================================
// SECTION 31 : DP ON STRINGS — LONGEST COMMON SUBSEQUENCE (LC #1143)
// ============================================================
//
//  dp[i][j] = length of the LCS of s1[0..i) and s2[0..j)
//
//  RECURRENCE:
//   if s1[i-1] == s2[j-1]:  dp[i][j] = dp[i-1][j-1] + 1
//   else:                    dp[i][j] = max(dp[i-1][j], dp[i][j-1])
//
//  BASE CASE: dp[0][*] = dp[*][0] = 0 (LCS with an empty string is always 0)

int longestCommonSubsequence(const string& s1, const string& s2)
{
    int n = (int)s1.size();
    int m = (int)s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else
            {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[n][m];
}

// SPACE-OPTIMIZED VERSION — O(min(n,m)) instead of O(n*m):
// dp[i][*] only ever depends on dp[i-1][*] and dp[i][j-1] — a rolling
// pair of 1D arrays is sufficient. Only usable when you need the
// LENGTH, not the actual subsequence (reconstruction needs the full table).

int lcsSpaceOptimized(const string& s1, const string& s2)
{
    int n = (int)s1.size();
    int m = (int)s2.size();
    vector<int> prev(m + 1, 0);
    vector<int> curr(m + 1, 0);

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                curr[j] = prev[j - 1] + 1;
            }
            else
            {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        swap(prev, curr);   // O(1) pointer swap, not an O(m) copy
    }

    return prev[m];
}

// RECONSTRUCTING THE ACTUAL LCS STRING — backtrack through the table
// from dp[n][m] toward dp[0][0], following whichever neighbor produced
// the current cell's value:

string reconstructLCS(const string& s1, const string& s2)
{
    int n = (int)s1.size();
    int m = (int)s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else
            {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    string result;
    int i = n, j = m;

    while (i > 0 && j > 0)
    {
        if (s1[i - 1] == s2[j - 1])
        {
            result += s1[i - 1];
            --i;
            --j;
        }
        else if (dp[i - 1][j] >= dp[i][j - 1])
        {
            --i;
        }
        else
        {
            --j;
        }
    }

    reverse(result.begin(), result.end());   // built back-to-front, must reverse
    return result;
}

void section31_lcs_demo()
{
    string s1 = "ABCBDAB";
    string s2 = "BDCABA";

    cout << "LCS length: " << longestCommonSubsequence(s1, s2) << "\n";       // 4
    cout << "LCS length (space-optimized): " << lcsSpaceOptimized(s1, s2) << "\n";   // 4
    cout << "Reconstructed LCS: " << reconstructLCS(s1, s2) << "\n";           // "BCBA" or "BDAB" (valid LCS, may vary)
}

// ============================================================
// SECTION 32 : DP ON STRINGS — EDIT DISTANCE (LC #72)
// ============================================================
//
//  dp[i][j] = minimum number of operations (insert/delete/replace)
//  to convert s1[0..i) into s2[0..j)
//
//  RECURRENCE:
//   if s1[i-1] == s2[j-1]:  dp[i][j] = dp[i-1][j-1]              (no-op, chars already match)
//   else:                    dp[i][j] = 1 + min(
//                                dp[i-1][j-1],   // REPLACE s1[i-1] with s2[j-1]
//                                dp[i-1][j],      // DELETE s1[i-1]
//                                dp[i][j-1]        // INSERT s2[j-1]
//                            )
//
//  BASE CASE: dp[i][0] = i (delete all i characters of s1)
//             dp[0][j] = j (insert all j characters of s2)

int editDistance(const string& s1, const string& s2)
{
    int n = (int)s1.size();
    int m = (int)s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; ++i)
    {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; ++j)
    {
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else
            {
                dp[i][j] = 1 + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
            }
        }
    }

    return dp[n][m];
}

// THE THREE NEIGHBOR CELLS MAP DIRECTLY TO THE THREE OPERATIONS —
// worth stating explicitly in an interview:
//   dp[i-1][j-1] (diagonal) -> REPLACE  (consume one char from each string)
//   dp[i-1][j]   (above)    -> DELETE   (consume one char from s1 only)
//   dp[i][j-1]   (left)     -> INSERT   (consume one char from s2 only)

void section32_edit_distance_demo()
{
    cout << "editDistance(\"horse\",\"ros\"): "
         << editDistance("horse", "ros") << "\n";   // 3
    cout << "editDistance(\"intention\",\"execution\"): "
         << editDistance("intention", "execution") << "\n";   // 5
}

// ============================================================
// SECTION 33 : DP ON STRINGS — REGULAR EXPRESSION MATCHING (LC #10)
// ============================================================
//
//  Support: '.' matches ANY single character. '*' matches ZERO OR
//  MORE of the PRECEDING element (not the literal '*' character
//  itself — this is what makes it genuinely harder than Wildcard
//  Matching, LC #44, where '*' independently means "any sequence").
//
//  WHY THIS CANNOT BE SOLVED GREEDILY: '*' creates a CHOICE — match
//  zero of the preceding element, or match one more and keep trying.
//  A greedy "consume as many as possible" approach can commit to the
//  wrong choice and get stuck, requiring BACKTRACKING — which is
//  exactly what DP encodes implicitly by trying BOTH options at every
//  cell and keeping whichever succeeds.
//
//  dp[i][j] = does s[0..i) match p[0..j)?
//
//  RECURRENCE (for p[j-1] == '*', treating "p[j-2]*" as ONE unit):
//   ZERO occurrences of p[j-2]:      dp[i][j] |= dp[i][j-2]
//   ONE-OR-MORE occurrences:          if s[i-1] matches p[j-2]:
//                                        dp[i][j] |= dp[i-1][j]
//
//  RECURRENCE (for p[j-1] == normal char or '.'):
//   dp[i][j] = dp[i-1][j-1] && (p[j-1]=='.' || p[j-1]==s[i-1])

bool isMatch(const string& s, const string& p)
{
    int n = (int)s.size();
    int m = (int)p.size();
    vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
    dp[0][0] = true;

    // Handle patterns that can match an EMPTY string, e.g. "a*", "a*b*c*"
    for (int j = 1; j <= m; ++j)
    {
        if (p[j - 1] == '*' && j >= 2)
        {
            dp[0][j] = dp[0][j - 2];
        }
    }

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (p[j - 1] == '*')
            {
                bool zeroOccurrence = (j >= 2) ? dp[i][j - 2] : false;

                bool oneOrMoreOccurrence = false;
                if (j >= 2 && (p[j - 2] == '.' || p[j - 2] == s[i - 1]))
                {
                    oneOrMoreOccurrence = dp[i - 1][j];
                }

                dp[i][j] = zeroOccurrence || oneOrMoreOccurrence;
            }
            else if (p[j - 1] == '.' || p[j - 1] == s[i - 1])
            {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else
            {
                dp[i][j] = false;
            }
        }
    }

    return dp[n][m];
}

void section33_regex_matching_demo()
{
    cout << "isMatch(\"aa\",\"a\"): "       << isMatch("aa", "a")       << "\n";   // 0 (false)
    cout << "isMatch(\"aa\",\"a*\"): "      << isMatch("aa", "a*")      << "\n";   // 1 (true)
    cout << "isMatch(\"ab\",\".*\"): "       << isMatch("ab", ".*")       << "\n";   // 1 (true)
    cout << "isMatch(\"mississippi\",\"mis*is*p*.\"): "
         << isMatch("mississippi", "mis*is*p*.") << "\n";   // 0 (false)
}

// NOTE: LC #44 Wildcard Matching uses the SAME dp[i][j] TABLE SHAPE
// but a SIMPLER recurrence, because its '*' independently matches any
// sequence (including empty) and has no "preceding element" concept:
//   if p[j-1]=='?' or p[j-1]==s[i-1]:  dp[i][j] = dp[i-1][j-1]
//   if p[j-1]=='*':                     dp[i][j] = dp[i-1][j] || dp[i][j-1]
//                                       (dp[i-1][j] = '*' absorbs s[i-1];
//                                        dp[i][j-1] = '*' matches empty here)
// Recognizing these two problems SHARE a table shape but differ in
// exactly one recurrence branch is a strong senior-level signal.

// ============================================================
// SECTION 34 : std::format / std::print (C++20 / C++23) IN DEPTH
// ============================================================
//
//  std::format REPLACES printf/sprintf (unsafe format-string/argument
//  mismatches) AND ostringstream (verbose, multiple allocations, locale
//  overhead) with a TYPE-SAFE, single-allocation, extensible mechanism.
//
//  COMPARISON TABLE (extends Section 8's concatenation-cost discussion):
//   printf/snprintf   -> fast, UNSAFE (format-string/type mismatch is UB),
//                        no string_view support, C-style
//   ostringstream     -> safe, readable, SLOW (multiple internal allocs,
//                        locale-aware overhead on every operator<<)
//   std::to_string    -> simple, but FIXED 6-decimal precision for floats,
//                        allocates every call, decimal-only
//   std::format       -> safe (compile-time-checked for literal format
//                        strings), single allocation, fully extensible
//                        via custom formatter<T> specializations

void section34_format_demo()
{
    // Basic usage — {} placeholders, filled positionally by default
    string s1 = format("Hello, {}! You are {} years old.", "Alice", 30);
    cout << s1 << "\n";

    // Explicit positional arguments — same arg can be reused
    string s2 = format("{0} {1} {0}", "ping", "pong");
    cout << s2 << "\n";   // "ping pong ping"

    // Alignment and width
    cout << format("[{:>10}]", "right") << "\n";   // [     right]
    cout << format("[{:<10}]", "left")  << "\n";   // [left      ]
    cout << format("[{:^10}]", "mid")   << "\n";   // [   mid    ]

    // Numeric formatting — full printf-style control, but type-checked
    cout << format("{:08.3f}", 3.14159) << "\n";   // 0003.142
    cout << format("{:#x}", 255)         << "\n";   // 0xff
    cout << format("{:+}", 42)             << "\n";   // +42

    // format_to — write DIRECTLY into an existing container/iterator,
    // avoiding the extra allocation a standalone format() call makes
    string buf;
    format_to(back_inserter(buf), "pid={} status={}", 1234, "running");
    cout << buf << "\n";

    // formatted_size — know the EXACT size needed before allocating,
    // enabling a single precisely-sized allocation instead of growth
    size_t needed = formatted_size("{}-{}", 2026, 8);
    cout << "formatted_size: " << needed << "\n";
}

// CUSTOM FORMATTER — extend std::format to a user-defined type by
// specializing std::formatter<T>. This is genuinely senior-level
// knowledge — very few candidates know this specialization point exists.

struct Point3D
{
    int x, y;
};

// (place this specialization at NAMESPACE SCOPE, not inside a function)
template<>
struct std::formatter<Point3D>
{
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();   // no custom format-spec syntax supported here
    }

    auto format(const Point3D& p, format_context& ctx) const
    {
        return format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

void section34_custom_formatter_demo()
{
    Point3D p{3, 4};
    cout << format("Location: {}", p) << "\n";   // Location: (3, 4)
}

// std::print / std::println (C++23) — requires <print> and a recent
// enough standard library (GCC 14+/Clang 17+ roughly). These write
// DIRECTLY to a stream (stdout by default) — no intermediate std::string
// is ever constructed at all, unlike format() + cout <<, which builds
// a string THEN copies it to the stream buffer.
//
// #include <print>
// std::print("Hello, {}!\n", "world");     // no auto newline
// std::println("Hello, {}!", "world");      // auto-appends newline
//
// Check `__cpp_lib_print` before relying on this in portable code:
// #ifdef __cpp_lib_print
//     std::println("Available!");
// #endif

// ============================================================
// SECTION 35 : compare() vs operator<=> (C++20 SPACESHIP) DEEP DIVE
// ============================================================
//
//  PRE-C++20: string::compare() — strcmp-style, returns int
//   <0 if *this < other,  0 if equal,  >0 if *this > other
//
//  C++20: operator<=> — the THREE-WAY COMPARISON ("spaceship")
//   operator. std::string implements this, returning a
//   std::strong_ordering — an ENUM-LIKE type with exactly 3 possible
//   values: strong_ordering::less, ::equal, ::greater. "strong"
//   specifically means: if two strings compare equal, they are FULLY
//   substitutable for each other in every observable way — which
//   genuinely holds for string content equality.
//
//  THE BIG PRACTICAL WIN: once operator<=> is defined for a type, the
//  compiler AUTO-GENERATES all 6 relational operators (==, !=, <, <=,
//  >, >=) via "rewritten candidates" — you get a full comparison
//  interface from ONE function instead of writing (or the library
//  shipping) 6 separate operator overloads.

void section35_spaceship_demo()
{
    string a = "apple";
    string b = "banana";

    // Pre-C++20 style
    int c1 = a.compare(b);
    cout << "compare(): " << c1 << " (negative means a < b)\n";

    // C++20 spaceship — returns a strong_ordering, not a plain int
    auto result = a <=> b;

    if (result < 0)
    {
        cout << "a < b (via <=>)\n";
    }
    else if (result == 0)
    {
        cout << "a == b (via <=>)\n";
    }
    else
    {
        cout << "a > b (via <=>)\n";
    }

    // All 6 relational operators work automatically because <=> exists:
    cout << (a < b) << " " << (a >= b) << " " << (a != b) << "\n";
}

// DEFINING YOUR OWN <=> FOR A CUSTOM STRUCT — this directly replaces
// the EmployeeCompare functor pattern from your Set_Explanation.cpp
// (Section 9.7/22): instead of hand-writing a comparator with tie(),
// you write ONE defaulted operator<=> and get lexicographic,
// declaration-order comparison across ALL members for free — usable
// DIRECTLY as a std::set/std::map comparator with NO separate functor
// needed at all, since std::less<T> falls back to T's own operator
// (itself synthesized from operator<=>).

struct EmployeeV2
{
    string department;
    int    id;
    double salary;

    // Compares department, then id, then salary, IN DECLARATION ORDER —
    // exactly matching what tie(a.department,a.id,a.salary) < tie(...)
    // achieved manually in Set_Explanation.cpp, but auto-generated here
    auto operator<=>(const EmployeeV2&) const = default;
    bool operator==(const EmployeeV2&) const = default;   // needed alongside <=> for ==/!=
};

void section35_custom_spaceship_demo()
{
    EmployeeV2 e1{"Engineering", 42, 95000};
    EmployeeV2 e2{"Engineering", 10, 88000};

    cout << "e1 < e2: " << (e1 < e2) << "\n";     // compares department first (tie), then id: 42 < 10 is false
    cout << "e1 == e2: " << (e1 == e2) << "\n";   // false

    set<EmployeeV2> staff;    // works DIRECTLY — no custom comparator functor needed!
    staff.insert(e1);
    staff.insert(e2);
    for (const auto& e : staff)
    {
        cout << e.department << " #" << e.id << "\n";
    }

    // IMPORTANT CAVEAT: 'salary' is a double. Floating-point comparison
    // supports NaN, which is UNORDERED with everything (including
    // itself) — this forces the OVERALL comparison category down to
    // std::partial_ordering, NOT strong_ordering, even though
    // 'department' (string) and 'id' (int) each individually support
    // strong_ordering on their own. The compiler computes this via
    // std::common_comparison_category_t<...> across all members — ANY
    // partial_ordering member degrades the WHOLE aggregate's category.
    // This is a genuinely easy fact to get wrong and a strong signal
    // question: "does adding a double member change what category of
    // ordering your defaulted <=> produces?" — yes, it does.
}

// ============================================================
// SECTION 36 : EXTENDED PRACTICE — PATTERN MATCHING & DP ON STRINGS
// (LC = LeetCode, GFG = GeeksforGeeks, STV = Striver Sheet,
//  CN = Coding Ninjas Studio, NKR = Naukri Coding 360)
// ============================================================

// --- KMP / Failure Function ---
//  Platform | Problem                                      | Difficulty
//  LC       | #28 Find the Index of the First Occurrence     | Easy
//  LC       | #459 Repeated Substring Pattern                    | Easy
//  LC       | #1392 Longest Happy Prefix                            | Hard
//  GFG      | "KMP Algorithm for Pattern Searching"                    | Hard
//  STV      | "String Matching — KMP" Day                                | Hard
//  CN       | "KMP Pattern Matching"                                       | Hard

// --- Rabin-Karp / Rolling Hash ---
//  Platform | Problem                                      | Difficulty
//  LC       | #28 Find the Index (alt. approach)              | Easy
//  LC       | #187 Repeated DNA Sequences                        | Medium
//  LC       | #1044 Longest Duplicate Substring (binary-search + hash) | Hard
//  GFG      | "Rabin-Karp Algorithm for Pattern Searching"          | Hard
//  STV      | "Rolling Hash" Day                                       | Hard

// --- Z-Algorithm ---
//  Platform | Problem                                      | Difficulty
//  LC       | #28 Find the Index (alt. approach)              | Easy
//  LC       | #1392 Longest Happy Prefix (alt. approach)        | Hard
//  GFG      | "Z Algorithm for Pattern Matching"                    | Hard
//  CN       | "Z-Function Problems"                                    | Hard

// --- DP on Strings — LCS Family ---
//  Platform | Problem                                      | Difficulty
//  LC       | #1143 Longest Common Subsequence                | Medium
//  LC       | #583 Delete Operation for Two Strings              | Medium
//  LC       | #712 Minimum ASCII Delete Sum for Two Strings        | Medium
//  LC       | #1092 Shortest Common Supersequence                   | Hard
//  LC       | #516 Longest Palindromic Subsequence                    | Medium
//  GFG      | "Longest Common Subsequence"                              | Medium
//  STV      | "DP on Strings" Day (full section)                          | Hard
//  CN       | "LCS and Variants"                                             | Hard

// --- DP on Strings — Edit Distance Family ---
//  Platform | Problem                                      | Difficulty
//  LC       | #72 Edit Distance                                | Hard
//  LC       | #161 One Edit Distance                             | Medium
//  LC       | #97 Interleaving String                              | Medium
//  GFG      | "Edit Distance (Levenshtein)"                          | Hard
//  STV      | "Edit Distance" Day                                       | Hard
//  CN       | "Edit Distance Problems"                                     | Hard

// --- DP on Strings — Pattern/Regex Matching ---
//  Platform | Problem                                      | Difficulty
//  LC       | #10 Regular Expression Matching                  | Hard
//  LC       | #44 Wildcard Matching                              | Hard
//  LC       | #115 Distinct Subsequences                           | Hard
//  GFG      | "Wildcard Pattern Matching"                             | Hard
//  STV      | "Regex/Wildcard DP" Day                                    | Hard
//  CN       | "Pattern Matching DP"                                         | Hard

// ============================================================
// SECTION 37 : QUICK-FIRE ORAL DRILL — SECTIONS 28-35 ADDENDUM
// ============================================================
//
// KMP core idea         -> LPS array lets you skip re-comparing TEXT
//                          characters already known to match
// KMP complexity proof   -> i/len (or i/j) advances bounded by n+m total,
//                          same amortized argument as two-pointer proofs
// Rabin-Karp core idea    -> compare HASHES first (O(1)), verify only on
//                          hash match to rule out spurious collisions
// Rabin-Karp negative mod  -> C++ % can return negative; always +MOD before
//                          the final %MOD when subtracting in a rolling hash
// Rabin-Karp worst case     -> O(n*m) under adversarial hash collisions —
//                          why KMP is preferred for guaranteed worst-case
// Z-array definition         -> Z[i] = longest prefix-match starting AT i
// Z-algo pattern search        -> pattern + separator + text; Z[i]==m marks a match
// LCS recurrence                -> match: diagonal+1; no match: max(up, left)
// LCS space optimization          -> O(min(n,m)) via rolling prev/curr arrays
// Edit Distance recurrence         -> match: diagonal (no-op); no match:
//                                    1+min(diagonal=replace, up=delete, left=insert)
// Regex matching '*' meaning         -> zero-or-more of the PRECEDING element,
//                                    NOT "any sequence" (that's Wildcard's '*')
// Regex vs Wildcard DP                -> same table shape, ONE recurrence
//                                    branch differs — '*' semantics diverge
// std::format vs printf/ostringstream   -> type-safe + single-allocation +
//                                    extensible via formatter<T> specialization
// format_to() vs format()                 -> writes into existing buffer,
//                                    avoids the extra allocation format() makes
// compare() vs operator<=>                  -> int (strcmp-style) vs
//                                    strong_ordering; <=> auto-generates
//                                    all 6 relational operators from ONE function
// Defaulted <=> on a struct                   -> replaces manual tie()-based
//                                    comparator functors entirely
// float/double member + <=>                    -> degrades WHOLE aggregate's
//                                    category to partial_ordering (NaN is
//                                    unordered with everything, even itself)

// ============================================================
// ADD THESE CALLS INTO YOUR EXISTING main(), before
// the "===== Done =====" line
// ============================================================
//
//    section28_kmp_demo();
//    section29_rabin_karp_demo();
//    section30_z_algorithm_demo();
//    section31_lcs_demo();
//    section32_edit_distance_demo();
//    section33_regex_matching_demo();
//    section34_format_demo();
//    section34_custom_formatter_demo();
//    section35_spaceship_demo();
//    section35_custom_spaceship_demo();
//
// (Sections 36-37 are reference-only — tables and a cheat sheet,
//  no runnable demo, same as several reference-only sections
//  earlier in this file.)

// ============================================================
// END OF NOTES
// Compile: g++ -std=c++20 -Wall -Wextra -Wshadow -Wsign-conversion \
//          -fsanitize=address,undefined String_Explanation.cpp -o string_demo
// ============================================================

/*
2914. Minimum Number of Changes to Make Binary String Beautiful
Medium
Topics
premium lock iconCompanies
Hint

You are given a 0-indexed binary string s having an even length.

A string is beautiful if it's possible to partition it into one or more substrings such that:

    Each substring has an even length.
    Each substring contains only 1's or only 0's.

You can change any character in s to 0 or 1.

Return the minimum number of changes required to make the string s beautiful.

 

Example 1:

Input: s = "1001"
Output: 2
Explanation: We change s[1] to 1 and s[3] to 0 to get string "1100".
It can be seen that the string "1100" is beautiful because we can partition it into "11|00".
It can be proven that 2 is the minimum number of changes needed to make the string beautiful.

Example 2:

Input: s = "10"
Output: 1
Explanation: We change s[1] to 1 to get string "11".
It can be seen that the string "11" is beautiful because we can partition it into "11".
It can be proven that 1 is the minimum number of changes needed to make the string beautiful.

Example 3:

Input: s = "0000"
Output: 0
Explanation: We don't need to make any changes as the string "0000" is beautiful already.

class Solution {
public:
    int minChanges(string s) {
        
    }
};

*/

#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    int minChanges(string s) {
        int changes = 0;
        for (int i = 0; i < (int)s.size(); i += 2) {
            if (s[i] != s[i + 1]) changes++;
        }
        return changes;
    }
};

class Solution1 {
public:
    int minChanges(string s) {
        int changes = 0;
        for (int i = 0; i < (int)s.size(); i += 2) {
            if (s[i] != s[i + 1]) changes++;
        }
        return changes;
    }
};

int main() {
    Solution sol;

    // Test 1
    cout << "Test 1: \"" << "1001" << "\" -> "
         << sol.minChanges("1001") << " (expected 2)" << endl;

    // Test 2
    cout << "Test 2: \"" << "10" << "\" -> "
         << sol.minChanges("10") << " (expected 1)" << endl;

    // Test 3
    cout << "Test 3: \"" << "0000" << "\" -> "
         << sol.minChanges("0000") << " (expected 0)" << endl;

    // Test 4: single pair matching
    cout << "Test 4: \"" << "11" << "\" -> "
         << sol.minChanges("11") << " (expected 0)" << endl;

    // Test 5: alternating
    cout << "Test 5: \"" << "0101" << "\" -> "
         << sol.minChanges("0101") << " (expected 2)" << endl;

    return 0;
}

/*

2914. Minimum Number of Changes to Make Binary String Beautiful
Explanation

We need to make the string "beautiful" — partitionable into even-length substrings where each substring is all 0s or all 1s.
Key Insight — Pair Up Adjacent Characters

Since every partition block must have even length, the string can be split into pairs of adjacent characters: (s[0], s[1]), (s[2], s[3]), ....

A beautiful string is possible if and only if each of these pairs contains two identical characters. Why?

    Any even-length block of identical characters can be viewed as a sequence of identical pairs.

    Conversely, if every adjacent pair (s[2i], s[2i+1]) is identical, then the whole string is a concatenation of "00" or "11" pairs → beautiful.

So the minimum changes = number of pairs where the two characters differ. For each mismatched pair, we change one character (e.g., "01" → "00" or "11") → cost 1.
Why This Is Optimal

    Lower bound: Each mismatched pair (s[2i], s[2i+1]) with different characters cannot be part of any beautiful block without at least 1 change, because a beautiful block of even length must contain equal characters in every adjacent pair. So each mismatched pair requires ≥ 1 change.

    Upper bound: We can achieve exactly 1 change per mismatched pair by flipping one of its characters. This yields a string where every pair is "00" or "11", which is beautiful. So the total is exactly the number of mismatched pairs.

Example 1: s = "1001", n = 4

Pairs: (s[0],s[1]) = (1,0) — mismatch → +1. (s[2],s[3]) = (0,1) — mismatch → +1.

Total = 2 ✅
Example 2: s = "10"

Pair: (1,0) — mismatch → 1 ✅
Example 3: s = "0000"

Pairs: (0,0) ✅, (0,0) ✅ → 0 ✅
Final Code
cpp

#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    int minChanges(string s) {
        int changes = 0;
        for (int i = 0; i < (int)s.size(); i += 2) {
            if (s[i] != s[i + 1]) changes++;
        }
        return changes;
    }
};

int main() {
    Solution sol;

    // Test 1
    cout << "Test 1: \"" << "1001" << "\" -> "
         << sol.minChanges("1001") << " (expected 2)" << endl;

    // Test 2
    cout << "Test 2: \"" << "10" << "\" -> "
         << sol.minChanges("10") << " (expected 1)" << endl;

    // Test 3
    cout << "Test 3: \"" << "0000" << "\" -> "
         << sol.minChanges("0000") << " (expected 0)" << endl;

    // Test 4: single pair matching
    cout << "Test 4: \"" << "11" << "\" -> "
         << sol.minChanges("11") << " (expected 0)" << endl;

    // Test 5: alternating
    cout << "Test 5: \"" << "0101" << "\" -> "
         << sol.minChanges("0101") << " (expected 2)" << endl;

    return 0;
}

Dry Run of Final Code
Dry Run 1: s = "1001", n = 4
i	s[i]	s[i+1]	Mismatch?	changes
0	'1'	'0'	✅	1
2	'0'	'1'	✅	2

Result: 2 ✅
Dry Run 2: s = "10", n = 2
i	s[i]	s[i+1]	Mismatch?	changes
0	'1'	'0'	✅	1

Result: 1 ✅
Dry Run 3: s = "0000", n = 4
i	s[i]	s[i+1]	Mismatch?	changes
0	'0'	'0'	❌	0
2	'0'	'0'	❌	0

Result: 0 ✅
Dry Run 4: s = "11", n = 2
i	s[i]	s[i+1]	Mismatch?	changes
0	'1'	'1'	❌	0

Result: 0 ✅
Dry Run 5: s = "0101", n = 4
i	s[i]	s[i+1]	Mismatch?	changes
0	'0'	'1'	✅	1
2	'0'	'1'	✅	2

Result: 2 ✅

Verification: "0101" → change s[1] to '0' and s[3] to '0' → "0000" (beautiful, 2 changes). ✅
Complexity
Metric	Value	Reasoning
Time	O(n)	Single pass over pairs
Space	O(1)	Only one integer counter
Summary
Aspect	Detail
Problem	Min changes to make string partitionable into even-length homogeneous blocks
Key insight	Beautiful ⟺ every adjacent pair (s[2i], s[2i+1]) is identical
Approach	Count mismatched pairs
Time	O(n)
Space	O(1)
The "Aha" Moment

The "beautiful" property reduces to a local condition: each aligned pair of characters must be equal. Any longer even-length block of identical characters is just a sequence of such equal pairs. So we don't need to think about partitions at all — just count how many pairs (s[2i], s[2i+1]) differ, and that's the answer. Each such pair needs exactly one change (flip one character to match the other), and changes in different pairs are independent.
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int minChanges(string s) {
        int changes = 0;
        for (int i = 0; i < (int)s.size(); i += 2) {
            if (s[i] != s[i + 1]) changes++;
        }
        return changes;
    }
};

🎯 Time: O(n) | Space: O(1)

*/

/*
Min Flips for Alternate
Difficulty: EasyAccuracy: 48.58%Submissions: 59K+Points: 2

Given a binary string s, we can flip any bit (change 0 to 1 or 1 to 0) to make the string alternating. A string is called alternating if no two adjacent characters are the same.

Find the minimum number of bit flips required to make the string alternating.

Examples:

Input: s = "001"
Output: 1
Explanation: Flip the bit at index 0 from 0 to 1 to obtain "101", which is an alternating binary string. Hence, the minimum number of flips required is 1.

Input: s = "0001010111" 
Output: 2
Explanation: Flip the bit at indices 1 and 8 to obtain "0101010101", which is an alternating binary string. Hence, the minimum number of flips required is 2.

class Solution {
  public:
    int minFlips(string &s) {
        // code here
        
    }
};

and also show to interviewer how time and space complexity calculate



Min Flips for Alternate
Explanation

We need to make the binary string alternating (no two adjacent characters are the same) using the minimum number of bit flips.
Key Insight — Only Two Possible Alternating Patterns

An alternating binary string must be one of exactly two patterns:

    Pattern A: 01010101... (starts with 0)

    Pattern B: 10101010... (starts with 1)

We try both patterns and count how many flips are needed to match each. The answer is the minimum of the two.
Example 1: s = "001", length 3
Index	s	Pattern A (010)	Flips A	Pattern B (101)	Flips B
0	0	0	0	1	1
1	0	1	1	0	0
2	1	0	1	1	0
Total			2		1

Minimum = 1 ✅
Example 2: s = "0001010111", length 10

Let's compute both patterns. For index i:

    Pattern A expects '0' if i is even, '1' if i is odd.

    Pattern B expects '1' if i is even, '0' if i is odd.

They are complementary: if Pattern A says '0', Pattern B says '1', and vice versa.

So at each index:

    If s[i] matches Pattern A, it doesn't match Pattern B → contributes 1 to Pattern B's flip count.

    If s[i] matches Pattern B, it contributes 1 to Pattern A's flip count.

    Either way, exactly one of the two patterns requires a flip at each index.

Therefore: flipsA + flipsB = n for every string. So we only need to compute flipsA, and flipsB = n - flipsA. The answer is min(flipsA, n - flipsA).
Algorithm

    Initialize flipsA = 0.

    For each index i from 0 to n-1:

        The expected character for Pattern A at position i is '0' if i is even, '1' if i is odd.

        If s[i] != expected, increment flipsA.

    Return min(flipsA, n - flipsA).

Example 2 Detailed

s = "0001010111", n = 10.

Pattern A expected: 0101010101
i	s[i]	Expected A	Match?	flipsA
0	0	0	✅	0
1	0	1	❌	1
2	0	0	✅	1
3	1	1	✅	1
4	0	0	✅	1
5	1	1	✅	1
6	0	0	✅	1
7	1	1	✅	1
8	1	0	❌	2
9	1	1	✅	2

flipsA = 2, flipsB = 10 - 2 = 8. Minimum = 2 ✅
Final Code


*/


#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Solution {
public:
    int minFlips(string &s) {
        int n = (int)s.size();
        int flipsA = 0;  // flips needed for pattern starting with '0'
        
        for (int i = 0; i < n; i++) {
            // Pattern A expects '0' at even indices, '1' at odd indices
            char expected = (i % 2 == 0) ? '0' : '1';
            if (s[i] != expected) flipsA++;
        }
        
        // Pattern B needs the complementary flips
        int flipsB = n - flipsA;
        
        return min(flipsA, flipsB);
    }
};

int main() {
    Solution sol;

    // Test 1
    string s1 = "001";
    cout << "Test 1: \"" << s1 << "\" -> "
         << sol.minFlips(s1) << " (expected 1)" << endl;

    // Test 2
    string s2 = "0001010111";
    cout << "Test 2: \"" << s2 << "\" -> "
         << sol.minFlips(s2) << " (expected 2)" << endl;

    // Test 3: already alternating
    string s3 = "0101";
    cout << "Test 3: \"" << s3 << "\" -> "
         << sol.minFlips(s3) << " (expected 0)" << endl;

    // Test 4: all same
    string s4 = "0000";
    cout << "Test 4: \"" << s4 << "\" -> "
         << sol.minFlips(s4) << " (expected 2)" << endl;

    // Test 5: single char
    string s5 = "1";
    cout << "Test 5: \"" << s5 << "\" -> "
         << sol.minFlips(s5) << " (expected 0)" << endl;

    return 0;
}

/*

Dry Run of Final Code
Dry Run 1: s = "001", n = 3
i	s[i]	Expected A	Match?	flipsA
0	'0'	'0'	✅	0
1	'0'	'1'	❌	1
2	'1'	'0'	❌	2

flipsA = 2, flipsB = 3 - 2 = 1. min(2, 1) = 1. ✅
Dry Run 2: s = "0001010111", n = 10

(Shown above) flipsA = 2, flipsB = 8, min = 2. ✅
Dry Run 3: s = "0101", n = 4
i	s[i]	Expected A	Match?	flipsA
0	'0'	'0'	✅	0
1	'1'	'1'	✅	0
2	'0'	'0'	✅	0
3	'1'	'1'	✅	0

flipsA = 0, flipsB = 4, min = 0. ✅
Dry Run 4: s = "0000", n = 4
i	s[i]	Expected A	Match?	flipsA
0	'0'	'0'	✅	0
1	'0'	'1'	❌	1
2	'0'	'0'	✅	1
3	'0'	'1'	❌	2

flipsA = 2, flipsB = 2, min = 2. ✅

Verification: "0000" → change to "0101" (2 flips) or "1010" (2 flips). Either way, 2. ✅
Dry Run 5: s = "1", n = 1
i	s[i]	Expected A	Match?	flipsA
0	'1'	'0'	❌	1

flipsA = 1, flipsB = 1 - 1 = 0, min = 0. ✅

Verification: A single character is trivially alternating. ✅
Complexity Analysis
Time Complexity
Step	Cost
Loop over n characters	O(n)
Each iteration: O(1) work (modulo, comparison, increment)	O(1)
Total time	O(n)

We make one pass through the string. No nested loops, no recursion, no sorting.
Space Complexity
Item	Cost
n, flipsA, flipsB, expected, i	O(1) — constant number of integer/char variables
Input string s	O(n) — but this is input, not auxiliary
Total auxiliary space	O(1)

We don't allocate any new arrays, strings, or data structures. Only a few scalar variables.
How to Explain This to an Interviewer

    Problem restatement: "We need to flip the minimum number of bits so the string becomes alternating."

    Key insight: "An alternating binary string has exactly two forms: 010101... or 101010.... These are complements of each other. So at every position, exactly one of the two patterns matches the current character. That means if Pattern A needs k flips, Pattern B needs n - k flips."

    Algorithm: "I count the flips needed for Pattern A in one pass. Then Pattern B needs n - flipsA flips. Return the minimum of the two."

    Time complexity: "One pass over the string, O(1) work per character → O(n). This is optimal because we must read every character to know its value."

    Space complexity: "Only a few integer variables (flip count, index, expected character) → O(1) auxiliary. The input string itself is O(n), but that's not counted as auxiliary space."

    Why not try both patterns separately? "We could, but since they're complementary, computing one gives the other for free. This saves a pass and is cleaner."

    Edge cases: "Empty string → loop doesn't run, flipsA = 0, flipsB = 0, returns 0. Single character → trivially alternating, returns 0. Already alternating → returns 0."

Summary
Aspect	Detail
Problem	Min bit flips to make binary string alternating
Key insight	Only 2 alternating patterns; they're complementary
Approach	Count flips for one pattern; the other is n - flips
Formula	min(flipsA, n - flipsA)
Time	O(n) — single pass
Space	O(1) auxiliary — only scalar variables
Optimality	O(n) is optimal because we must inspect each character
The "Aha" Moment

The two alternating patterns are bitwise complements of each other. At every index, if Pattern A's expected bit matches s[i], then Pattern B's expected bit (which is the opposite) does not match. So every position contributes to exactly one of the two flip counts. This gives the elegant identity flipsA + flipsB = n, meaning we only need to compute one of them. This halves the work and simplifies the code — one pass instead of two.
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int minFlips(string &s) {
        int n = s.size();
        int flipsA = 0;
        for (int i = 0; i < n; i++) {
            char expected = (i % 2 == 0) ? '0' : '1';
            if (s[i] != expected) flipsA++;
        }
        return min(flipsA, n - flipsA);
    }
};

🎯 Time: O(n) | Space: O(1) auxiliary

*/

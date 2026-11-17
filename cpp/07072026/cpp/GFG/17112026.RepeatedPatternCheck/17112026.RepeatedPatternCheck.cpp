/*
Repeated Pattern Check
Difficulty: EasyAccuracy: 26.05%Submissions: 22K+Points: 2

Given a string s made of two characters of 'x' and 'y'.  The task is to verify whether s is made by concatenating substrings that follow the pattern xnyn  (equal number of y's follow equal number of x's). 

Examples:

Input: s = "xxyyxy" 
Output: true
Explanation: The string can be split into "xxyy" + "xy". Both substrings contain an equal number of consecutive 'x' and 'y' characters.

Input: s = "xyx"
Output: false
Explanation: The last 'x' cannot form a valid substring because it is not followed by a matching 'y'. 

class Solution {
  public:
    bool findPattern(string& s) {
        // code here
        
    }
};

*/

#include <iostream>
#include <string>

using namespace std;

class Solution
{
	public:
	bool findPattern(string& s)
	{
		int n = (int)s.size();
		int i = 0;
			
		while(i<n)
		{
			//count consecutive 'x's
			int countX = 0;
			while(i<n && s[i] == 'x')
			{
				countX++;
				i++;
			}
		//If we start with 'y' or have no 'x' in this block -> invalid
		if(countX == 0 ) return  false;
	
		// Count consecutive 'y's
		int countY = 0;
		while (i<n && s[i] == 'y')
		{
			countY++;
			i++;
		}
	
		// Must have equal counts
		if(countX != countY) return false;
		}
		return true;
	}
};
int main()
{
	Solution sol;

	string s1 = "xxyyxy";
	cout << "Test 1: \"" << s1 << "\" -> " << (sol.findPattern(s1) ? "true" : "false") << " (expected true)" << endl;

	    // Test 2
    string s2 = "xyx";
    cout << "Test 2: \"" << s2 << "\" -> "
         << (sol.findPattern(s2) ? "true" : "false")
         << " (expected false)" << endl;

    // Test 3: single block
    string s3 = "xxyy";
    cout << "Test 3: \"" << s3 << "\" -> "
         << (sol.findPattern(s3) ? "true" : "false")
         << " (expected true)" << endl;

    // Test 4: just "xy"
    string s4 = "xy";
    cout << "Test 4: \"" << s4 << "\" -> "
         << (sol.findPattern(s4) ? "true" : "false")
         << " (expected true)" << endl;

    // Test 5: only x's
    string s5 = "xxx";
    cout << "Test 5: \"" << s5 << "\" -> "
         << (sol.findPattern(s5) ? "true" : "false")
         << " (expected false)" << endl;

    // Test 6: only y's
    string s6 = "yyy";
    cout << "Test 6: \"" << s6 << "\" -> "
         << (sol.findPattern(s6) ? "true" : "false")
         << " (expected false)" << endl;

    // Test 7: extra y's
    string s7 = "xxyyy";
    cout << "Test 7: \"" << s7 << "\" -> "
         << (sol.findPattern(s7) ? "true" : "false")
         << " (expected false)" << endl;

    // Test 8: single char
    string s8 = "x";
    cout << "Test 8: \"" << s8 << "\" -> "
         << (sol.findPattern(s8) ? "true" : "false")
         << " (expected false)" << endl;

    // Test 9: multiple blocks
    string s9 = "xyxxyyxy";
    cout << "Test 9: \"" << s9 << "\" -> "
         << (sol.findPattern(s9) ? "true" : "false")
         << " (expected true)" << endl;

    return 0;
}
/*

Dry Run of Final Code
Dry Run 1: s = "xxyyxy"

Initial: i = 0, n = 6.
Iteration	countX	countY	i after	Valid?
1	2 (xx)	2 (yy)	4	✅
2	1 (x)	1 (y)	6	✅

i = 6 = n → loop ends → return true ✅
Dry Run 2: s = "xyx"

Initial: i = 0, n = 3.
Iteration	countX	countY	i after	Valid?
1	1 (x)	1 (y)	2	✅
2	1 (x)	0 (end)	3	❌ countX ≠ countY

Return false ✅
Dry Run 3: s = "xxyy"

Initial: i = 0, n = 4.
Iteration	countX	countY	i after	Valid?
1	2 (xx)	2 (yy)	4	✅

i = 4 = n → return true ✅
Dry Run 4: s = "xy"
Iteration	countX	countY	i after	Valid?
1	1 (x)	1 (y)	2	✅

Return true ✅
Dry Run 5: s = "xxx"
Iteration	countX	countY	i after	Valid?
1	3 (xxx)	0 (end)	3	❌

Return false ✅
Dry Run 6: s = "yyy"
Iteration	countX	countY	i after	Valid?
1	0	—	0	❌ countX == 0

Return false ✅
Dry Run 7: s = "xxyyy"
Iteration	countX	countY	i after	Valid?
1	2 (xx)	3 (yyy)	5	❌ countX ≠ countY

Return false ✅
Dry Run 8: s = "x"
Iteration	countX	countY	i after	Valid?
1	1 (x)	0 (end)	1	❌

Return false ✅
Dry Run 9: s = "xyxxyyxy"

Initial: i = 0, n = 8.
Iteration	countX	countY	i after	Valid?
1	1 (x)	1 (y)	2	✅
2	2 (xx)	2 (yy)	6	✅
3	1 (x)	1 (y)	8	✅

i = 8 = n → return true ✅

Verification: Blocks are "xy", "xxyy", "xy" — all valid. ✅
Complexity
Metric	Value	Reasoning
Time	O(n)	Each character is visited exactly once
Space	O(1)	Only a few integer variables
Summary
Aspect	Detail
Problem	Check if s is a concatenation of x^n y^n blocks
Key insight	Greedy parsing: each block is forced to be a maximal run of xs followed by the immediately following run of ys
Check	countX == countY for each block, and no block starts with y
Time	O(n)
Space	O(1)
Edge cases	Empty string (trivially true? Loop doesn't run → returns true), string of only xs or only ys (false), single char (false)
The "Aha" Moment

The parsing is unambiguous: a block must be the maximal run of consecutive xs followed by the maximal run of consecutive ys. There's no choice about where to split. So a single greedy pass suffices — count xs, count ys, compare, repeat. If any block fails the equality check, the whole string is invalid.
Note on Empty String

For s = "", the while loop doesn't run, and the function returns true. Depending on interpretation, an empty string could be considered trivially valid (concatenation of zero blocks). If the problem expects false for empty input, add an early check:
cpp

if (s.empty()) return false;

*/

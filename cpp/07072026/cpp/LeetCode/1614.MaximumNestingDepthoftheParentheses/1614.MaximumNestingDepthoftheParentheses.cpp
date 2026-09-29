#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

class Solution
{
public:
  int maxDepth(string s)
  {
	int depth = 0;
	int maxdepth = 0;

	for(auto c: s)
	{
		if (c == '(')
		{
			depth++;
			maxdepth = std::max(maxdepth,depth);
		}
		else if (c == ')')
		{
			depth--;
		}
	}

	return maxdepth;

  }
};

int main()
{
  Solution sol;

  // Test 1
  string s1 = "(1+(2*3)+((8)/4))+1";
  cout << "Test 1: " << sol.maxDepth(s1) << "  (expected 3)\n";

  // Test 2
  string s2 = "(1)+((2))+(((3)))";
  cout << "Test 2: " << sol.maxDepth(s2) << "  (expected 3)\n";

  // Test 3
  string s3 = "()(())((()()))";
  cout << "Test 3: " << sol.maxDepth(s3) << "  (expected 3)\n";

  // Edge cases
  cout << "Test 4: " << sol.maxDepth("1") << "  (expected 0)\n";
  cout << "Test 5: " << sol.maxDepth("") << "  (expected 0)\n";
  cout << "Test 6: " << sol.maxDepth("()()()") << "  (expected 1)\n";
  cout << "Test 7: " << sol.maxDepth("((((()))))") << "  (expected 5)\n";

  return 0;
}

/*
1614. Maximum Nesting Depth of the Parentheses
Easy
Topics
premium lock iconCompanies
Hint

Given a valid parentheses string s, return the nesting depth of s. The nesting depth is the maximum number of nested parentheses.

 

Example 1:

Input: s = "(1+(2*3)+((8)/4))+1"

Output: 3

Explanation:

Digit 8 is inside of 3 nested parentheses in the string.

Example 2:

Input: s = "(1)+((2))+(((3)))"

Output: 3

Explanation:

Digit 3 is inside of 3 nested parentheses in the string.

Example 3:

Input: s = "()(())((()()))"

Output: 3

class Solution {
public:
    int maxDepth(string s) {
        
    }
};

give this with int main() and header files
*/

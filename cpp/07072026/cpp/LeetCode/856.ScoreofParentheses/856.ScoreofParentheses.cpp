#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cmath> //std::pow();

class Solution
{
public:
  // Approach 1: stack of scores.
  int scoreOfParentheses1(std::string s)
  {
    std::stack<int> st;
    st.push(0); // running score of the current level

    for (char c : s)
    {
      if (c == '(')
      {
        st.push(0); // start a new level
      }
      else
      {
        int inner = st.top();
        st.pop();
        int score = (inner == 0) ? 1 : 2 * inner;
        st.top() += score; // add to parent level
      }
    }
    return st.top();
  }

	
  int scoreOfParentheses2(std::string s)
  {
	int ans = 0, depth = 0;
	bool was_open = false;

	for(auto c : s)	
	{
		if(c == '(')
		{
			depth++;
			was_open = true;
		}
		else if (c == ')')
		{
			depth--;
			//if (was_open) ans += pow(2,depth);  required #include <cmath> header

			//if (was_open) ans += 1 << depth; // mathematical equal to ans = ans + 2 ^ depth //  give right answer
// bitwise shift operator (<<) has a higher priority than the addition-assignment operator (+=) . complier see as ans = ans + (1 << depth)  


			//if (was_open) ans = ans + 1 << depth;  // standard addition operator (+) has a higher priority than the bitwise shift operator (<<). Because of this, the compiler groups the calculation incorrectly.
/*
    Order of Execution: The compiler actually executes ans + 1 first, and then takes that combined sum and shifts all of its bits to the left by depth.
    How the compiler sees it: ans = (ans + 1) << depth;
    Mathematical equivalent:  ans = (ans + 1) * 2 ^ depth   // wrong
*/
			if (was_open) ans = ans + (1 << depth);
			was_open = false;
		}
	}
	return ans;
  }

int scoreOfParentheses(std::string s)
{
	int depth = 0, ans = 0;

	//for(int i =0 ; i<(int)s.size(); ++i)
	for(int i =0 ; i<(int)s.length(); ++i)
	{
		if( s[i] == '(') depth++;

		else if (s[i] == ')') 
		{
			depth --; // if current = ')' //end
			// if(s[i-1] == '(') 		{ans += pow(2,depth); } // so check previous is '('  start
			 if(s[i-1] == '(') 		{ans += (1 << depth); } // so check previous is '('  start
		}
	
	}

	return ans;

}
};

int main()
{
  Solution sol;

  struct Case
  {
    std::string s;
    int expected;
  };
  std::vector<Case> tests = 
  {
      {"()", 1},     {"(())", 2},   {"()()", 2},
      {"(()())", 3}, // (  () () ) = 2*(1+1) = 4? no: (1+1)=2 -> *2 = 4? see
                     // below
      {"((()))", 4}, {"()(())", 3}, {"(()(()))", 6},
      {"", 0},       {"()()()", 3}, {"((()()))", 8}, {"(()(()))", 6}
  };

  // Correct expectations (recomputed by hand) for reference:
  // "(()())"   = ( 1 + 1 ) = 2 * 2 = 4
  // "((()))"   = 2 * (2 * (1)) = 4
  // "()(())"   = 1 + 2 = 3
  // "(()(()))" = 2 * (1 + 2) = 6
  // "((()()))" = 2 * (2 * (1 + 1)) = 8
  tests[3] = {"(()())", 4};

  bool all_pass = true;
  for (const auto &tc : tests)
  {
    int got = sol.scoreOfParentheses(tc.s);
    bool ok = (got == tc.expected);
    all_pass = all_pass && ok;
    std::cout << "s = \"" << tc.s << "\""
              << "  got = " << got << "  expected = " << tc.expected
              << (ok ? "  [PASS]" : "  [FAIL]") << std::endl;
  }

  std::cout << (all_pass ? "\nAll tests passed.\n" : "\nSome tests FAILED.\n");
  return 0;
}

/*

856. Score of Parentheses
Medium
Topics
premium lock iconCompanies

Given a balanced parentheses string s, return the score of the string.

The score of a balanced parentheses string is based on the following rule:

    "()" has score 1.
    AB has score A + B, where A and B are balanced parentheses strings.
    (A) has score 2 * A, where A is a balanced parentheses string.

 

Example 1:

Input: s = "()"
Output: 1

Example 2:

Input: s = "(())"
Output: 2

Example 3:

Input: s = "()()"
Output: 2

class Solution {
public:
    int scoreOfParentheses(string s) {
        
    }
};

give this with int main()

*/

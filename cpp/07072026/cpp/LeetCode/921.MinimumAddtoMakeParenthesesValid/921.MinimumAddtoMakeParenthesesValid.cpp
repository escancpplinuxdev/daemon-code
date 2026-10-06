/*
 The idea: scan left to right, keep a count of unmatched (.
 When you see (, increment.
 When you see ), if there's an unmatched ( to pair with, decrement; 
 otherwise this ) must be fixed by inserting a ( before it — so count it as one insertion. 
 At the end, any remaining unmatched ( each needs a ) appended — that's the second count.

*/

/*

stack explanation Explanation 

We use a stack to keep track of unmatched opening parentheses '('.

    Traverse each character in the string:

        If it's '(', push it onto the stack.

        If it's ')':

            If the stack is not empty, it means this ')' matches a previous '('. Pop the stack (that pair is valid).

            If the stack is empty, it means this ')' has no matching '(', so we must insert an opening parenthesis → increment count.

    After processing all characters, any '(' still left in the stack have no matching ')', so we must insert a closing parenthesis for each → add stack.size() to count.

The final count is the minimum number of moves needed.

Example: s = "())"

    '(' → push → stack = ['(']

    ')' → pop → stack = []

    ')' → empty → count = 1

    Result = 1 ✅

Example: s = "((("

    push 3 times → stack = ['(','(','(']

    count = 0 + 3 = 3 ✅

*/



#include <iostream>
#include <string>
#include <vector>
#include <stack>

class Solution
{
public:
  	
  int minAddToMakeValid(std::string s)
  {
	std::stack<char> st;
	int count = 0;

	for(auto c: s)
	{
		if(c == '(')
		{
			st.push(c);
		}
		else if(c == ')')
		{
			if(!st.empty()) // stack not empty  means stack has previous '(' then stack.pop()
			{
				st.pop();
			}
			else		// if stack empty then count++;
			{
				++count;
			}
		}
	}
//	count += st.size();

	return count + st.size();
  }
  int minAddToMakeValid2(std::string s)
  {
  	int open =0; 
	int close = 0;

	for(char c : s)
	{
		if( c == '(')
		{
			++open;
		}
		else if( c == ')')
		{
			if(open>0)
			{
			--open;
			}
			else
			{
			++close;	
			}
		}
	}
	return open + close;
  
  }
  int minAddToMakeValid1(std::string s)
  {
    int open = 0;       // unmatched '(' seen so far
    int insertions = 0; // total insertions needed

    for (char c : s)
    {
      if (c == '(')
      {
        ++open;
      }
      else if (c == ')')
      { // c == ')'
        if (open > 0)
        {
          --open; // pair this ')' with an earlier '('
        }
        else
        {
          ++insertions; // no '(' to pair, must insert one
        }
      }
    }

    // Any remaining unmatched '(' each need a ')' appended.
    return insertions + open;
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
  std::vector<Case> tests = {
      {"())", 1},    {"(((", 3},  {"", 0},    {"()", 0},
      {"((())", 1},  {"()))", 1}, // wait — see recompute below
      {")))(((", 6},              // 3 ')' unmatched + 3 '(' unmatched
      {"())())", 2},              // recompute below
      {"()()", 0},   {"(()", 1},  {")))", 3}, {"())(()", 2}, // recompute below
  };

  // Recompute some expectations by hand:
  // "()))"    -> '(' matched with first ')', then 2 unmatched ')' → 2
  // "())())"  -> '(' matches ')' then '(' matches ')' then one ')' unmatched →
  // 1
  //              actually: scan:
  //              (  open=1
  //              )  open=0
  //              )  open=0, insertions=1
  //              (  open=1
  //              )  open=0
  //              )  open=0, insertions=2
  //              → 2
  // "())(()"  -> scan:
  //              (  open=1
  //              )  open=0
  //              )  open=0, ins=1
  //              (  open=1
  //              (  open=2
  //              )  open=1
  //              → ins=1 + open=1 = 2
  tests[5] = {"()))", 2};
  tests[7] = {"())())", 2};
  tests[11] = {"())(()", 2};

  bool all_pass = true;
  for (const auto &tc : tests)
  {
    int got = sol.minAddToMakeValid(tc.s);
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
 *
 * 921. Minimum Add to Make Parentheses Valid
Medium
Topics
premium lock iconCompanies

A parentheses string is valid if and only if:

    It is the empty string,
    It can be written as AB (A concatenated with B), where A and B are valid strings, or
    It can be written as (A), where A is a valid string.

You are given a parentheses string s. In one move, you can insert a parenthesis at any position of the string.

    For example, if s = "()))", you can insert an opening parenthesis to be "(()))" or a closing parenthesis to be "())))".

Return the minimum number of moves required to make s valid.



Example 1:

Input: s = "())"
Output: 1

Example 2:

Input: s = "((("
Output: 3

class Solution {
public:
    int minAddToMakeValid(string s) {

    }
};

give this with int main() and header files

*/

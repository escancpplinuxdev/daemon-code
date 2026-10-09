#include <iostream>
#include <stack>
#include <string>
using namespace std;

class Solution
{
public:
  int minInsertions(string s)
  {
    stack<char> st;
    int insertions = 0;
    int n = s.size();

    for (int i = 0; i < n; i++)
    {
      if (s[i] == '(')
      {
        st.push('(');
      }
      else
      {
        // Check if this is part of a '))' pair
        if (i + 1 < n && s[i + 1] == ')')
        {
          i++; // consume the second ')'
          if (!st.empty())
          {
            st.pop();
          }
          else
          {
            insertions++; // insert '('
          }
        }
        else
        {
          // Lone ')'
          insertions++; // insert another ')' to complete '))'
          if (!st.empty())
          {
            st.pop();
          }
          else
          {
            insertions++; // insert '('
          }
        }
      }
    }
    insertions += 2 * st.size();

    return insertions;
  }

  int minInsertions11(string s)
  {
    int insertions = 0;
    int open = 0; // unmatched '(' still needing '))'
    int i = 0;
    int n = s.size();

    while (i < n)
    {
      if (s[i] == '(')
      {
        open++;
        i++;
      }
      else
      {
        // s[i] == ')'
        // Check if the next character is also ')'
        if (i + 1 < n && s[i + 1] == ')')
        {
          // We have a complete '))' pair
          if (open > 0)
          {
            open--;
          }
          else
          {
            // No '(' to match → insert one
            insertions++;
          }
          i += 2; // consume both ')'
        }
        else
        {
          // Lone ')' → need to insert one more ')' to make '))'
          insertions++; // insert ')'
          if (open > 0)
          {
            open--;
          }
          else
          {
            // No '(' to match → insert one too
            insertions++; // insert '('
          }
          i += 1; // consume the single ')'
        }
      }
    }

    // Each remaining '(' needs two ')' → 2 insertions each
    insertions += 2 * open;

    return insertions;
  }
};

int main()
{
  Solution sol;

  // Test 1
  string s1 = "(()))";
  cout << "Input: \"" << s1 << "\" -> Output: " << sol.minInsertions(s1)
       << " (expected 1)" << endl;

  // Test 2
  string s2 = "())";
  cout << "Input: \"" << s2 << "\" -> Output: " << sol.minInsertions(s2)
       << " (expected 0)" << endl;

  // Test 3
  string s3 = "))())(";
  cout << "Input: \"" << s3 << "\" -> Output: " << sol.minInsertions(s3)
       << " (expected 3)" << endl;

  // Test 4
  string s4 = "(";
  cout << "Input: \"" << s4 << "\" -> Output: " << sol.minInsertions(s4)
       << " (expected 2)" << endl;

  // Test 5
  string s5 = ")";
  cout << "Input: \"" << s5 << "\" -> Output: " << sol.minInsertions(s5)
       << " (expected 2)" << endl;

  // Test 6
  string s6 = "())(())))";
  cout << "Input: \"" << s6 << "\" -> Output: " << sol.minInsertions(s6)
       << " (expected 0)" << endl;

  // Test 7
  string s7 = "))";
  cout << "Input: \"" << s7 << "\" -> Output: " << sol.minInsertions(s7)
       << " (expected 1)" << endl;

  return 0;
}

#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

class Solution
{
public:
  int longestValidParentheses(std::string s)
  {
    // Stack stores indices. Bottom of stack is the index of the last
    // unmatched ')', which acts as a "base" for the current valid run.
    std::stack<int> st;
    st.push(-1); // sentinel: index before the string starts

    int best = 0;
    for (int i = 0; i < (int)s.size(); ++i)
    {
      if (s[i] == '(')
      {
        st.push(i);
      }
      else
      { // s[i] == ')'
        st.pop();
        if (st.empty())
        {
          // This ')' has no matching '('. It becomes the new base.
          st.push(i);
        }
        else
        {
          // Valid substring runs from st.top()+1 to i inclusive.
          best = std::max(best, i - st.top());
        }
      }
    }
    return best;
  }
};

int longestValidParentheses(std::string s)
{
  int best = 0;
  auto pass = [&](char open, char close)
  {
    int l = 0, r = 0;
    for (char c : s)
    {
      if (c == open)
        l++;
      else if (c == close)
        r++;
      if (l == r)
        best = std::max(best, 2 * r);
      else if (r > l)
        l = r = 0;
    }
  };
  pass('(', ')'); // handles "(()" etc.
  std::reverse(s.begin(), s.end());
  pass(')', '('); // handles "())" etc.
  return best;
}

int main()
{
  Solution sol;

  struct Case
  {
    std::string s;
    int expected;
  };
  std::vector<Case> tests = {
      {"(()", 2},      {")()())", 4},       {"", 0},       {"()", 2},
      {"()()", 4},     {"(())", 4},         {"(()())", 6}, {"(", 0},
      {")", 0},        {")))))", 0},        {"(((((", 0},  {"()(()", 2},
      {"(()", 2},      {")()())()()()", 4}, // longest is "()()" = 4
      {"()(()())", 6}, {"(()))())", 4},     // "()()" at 3..6
  };

  bool all_pass = true;
  for (const auto &tc : tests)
  {
    int got = sol.longestValidParentheses(tc.s);
    bool ok = (got == tc.expected);
    all_pass = all_pass && ok;
    std::cout << "s = \"" << tc.s << "\""
              << "  got = " << got << "  expected = " << tc.expected
              << (ok ? "  [PASS]" : "  [FAIL]") << std::endl;
  }

  std::cout << (all_pass ? "\nAll tests passed.\n" : "\nSome tests FAILED.\n");
  return 0;
}

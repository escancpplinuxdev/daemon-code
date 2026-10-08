#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

class Solution
{
public:
  // Check if a string has valid parentheses (ignoring letters)
  bool isValid(const string &s)
  {
    int balance = 0;
    for (char c : s)
    {
      if (c == '(')
        balance++;
      else if (c == ')')
      {
        balance--;
        if (balance < 0)
          return false; // more ')' than '('
      }
    }
    return balance == 0;
  }

  vector<string> removeInvalidParentheses(string s)
  {
    vector<string> result;
    if (s.empty())
      return {""};

    unordered_set<string> visited; // avoid duplicate strings
    queue<string> q;

    q.push(s);
    visited.insert(s);

    bool found = false; // becomes true once we find a valid level

    while (!q.empty())
    {
      int levelSize = q.size();

      // Process all strings at the current BFS level
      for (int i = 0; i < levelSize; i++)
      {
        string curr = q.front();
        q.pop();

        if (isValid(curr))
        {
          result.push_back(curr);
          found = true;
        }

        // If we already found valid strings at this level,
        // don't generate the next level (would be more removals)
        if (found)
          continue;

        // Generate all strings by removing one parenthesis
        for (int j = 0; j < (int)curr.size(); j++)
        {
          if (curr[j] != '(' && curr[j] != ')')
            continue;

          string next = curr.substr(0, j) + curr.substr(j + 1);

          if (visited.find(next) == visited.end())
          {
            visited.insert(next);
            q.push(next);
          }
        }
      }

      // Once we found valid strings, stop going deeper
      if (found)
        break;
    }

    return result;
  }
};

int main()
{
  Solution sol;

  vector<string> tests = {"()())()", "(a)())()", ")(", "()", "((((", "n"};

  for (const string &s : tests)
  {
    vector<string> ans = sol.removeInvalidParentheses(s);
    cout << "Input: \"" << s << "\" -> Output: [";
    for (int i = 0; i < (int)ans.size(); i++)
    {
      cout << "\"" << ans[i] << "\"";
      if (i + 1 < (int)ans.size())
        cout << ", ";
    }
    cout << "]" << endl;
  }

  return 0;
}

/*

301. Remove Invalid Parentheses
Hard
Topics
premium lock iconCompanies
Hint

Given a string s that contains parentheses and letters, remove the minimum
number of invalid parentheses to make the input string valid.

Return a list of unique strings that are valid with the minimum number of
removals. You may return the answer in any order.



Example 1:

Input: s = "()())()"
Output: ["(())()","()()()"]

Example 2:

Input: s = "(a)())()"
Output: ["(a())()","(a)()()"]

Example 3:

Input: s = ")("
Output: [""]

class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {

    }
};

give this with int main() and headers files

*/

#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

class Solution
{
public:
  bool hasValidPath(vector<vector<char>> &grid)
  {
    int m = grid.size();
    int n = grid[0].size();

    // ----- Early pruning -----
    if ((m + n - 1) % 2 != 0)
      return false; // total length must be even
    if (grid[0][0] == ')')
      return false; // must start with '('
    if (grid[m - 1][n - 1] == '(')
      return false; // must end with ')'

    // dp[i][j] : set of reachable balances at cell (i, j)
    vector<vector<unordered_set<int>>> dp(m, vector<unordered_set<int>>(n));

    dp[0][0].insert(1); // first cell is '('

    for (int i = 0; i < m; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        if (i == 0 && j == 0)
          continue;

        int val = (grid[i][j] == '(') ? 1 : -1;
        unordered_set<int> balances;

        // From above
        if (i > 0)
        {
          for (int b : dp[i - 1][j])
          {
            if (b + val >= 0)
              balances.insert(b + val);
          }
        }
        // From left
        if (j > 0)
        {
          for (int b : dp[i][j - 1])
          {
            if (b + val >= 0)
              balances.insert(b + val);
          }
        }
        dp[i][j] = std::move(balances);
      }
    }
    return dp[m - 1][n - 1].count(0) > 0;
  }
};

int main()
{
  Solution sol;

  // Test 1
  vector<vector<char>> g1 = {
      {'(', '(', '('}, {')', '(', ')'}, {'(', '(', ')'}, {'(', '(', ')'}};
  cout << "Test 1: " << (sol.hasValidPath(g1) ? "true" : "false")
       << "  (expected true)\n";

  // Test 2
  vector<vector<char>> g2 = {{')', ')'}, {'(', '('}};
  cout << "Test 2: " << (sol.hasValidPath(g2) ? "true" : "false")
       << "  (expected false)\n";

  // Test 3 — single cell (can never be valid since non-empty valid string needs
  // at least 2 cells)
  vector<vector<char>> g3 = {{'('}};
  cout << "Test 3: " << (sol.hasValidPath(g3) ? "true" : "false")
       << "  (expected false)\n";

  // Test 4 — minimal valid
  vector<vector<char>> g4 = {{'(', ')'}};
  cout << "Test 4: " << (sol.hasValidPath(g4) ? "true" : "false")
       << "  (expected true)\n";

  // Test 5 — negative early
  vector<vector<char>> g5 = {{'(', ')'}, {')', '('}};
  cout << "Test 5: " << (sol.hasValidPath(g5) ? "true" : "false")
       << "  (expected false)\n";

  return 0;
}

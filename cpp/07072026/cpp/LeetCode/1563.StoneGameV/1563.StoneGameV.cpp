
#include <iostream>


#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution
{
public:
    int stoneGameV(vector<int>& stoneValue)
    {
        int n = stoneValue.size();
        if (n == 0) return 0;

        // prefix sums for O(1) range sum queries
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; ++i)
         {
 		prefix[i + 1] = prefix[i] + stoneValue[i];
	 }

        // memoization table: dp[l][r] = max score from subarray l..r
        vector<vector<int>> dp(n, vector<int>(n, -1));

        // recursive function
        function<int(int, int)> solve = [&](int l, int r) -> int
        {
            if (l == r) return 0;   // only one stone, no more score
            if (dp[l][r] != -1) return dp[l][r];

            int best = 0;
            // try all splits between k and k+1
            for (int k = l; k < r; ++k)
            {
                int leftSum = prefix[k + 1] - prefix[l];
                int rightSum = prefix[r + 1] - prefix[k + 1];

                if (leftSum < rightSum)
                {
                    // Bob throws away right row, Alice keeps left row
                    best = max(best, leftSum + solve(l, k));
                }
                else if (leftSum > rightSum)
                {
                    // Bob throws away left row, Alice keeps right row
                    best = max(best, rightSum + solve(k + 1, r));
                }
                else // equal sums
                {
                    // Alice can decide which row to keep
                    best = max(best, leftSum + solve(l, k));
                    best = max(best, rightSum + solve(k + 1, r));
                }
            }
            dp[l][r] = best;
            return best;
        };

        return solve(0, n - 1);
    }
};

int main()
{
    Solution sol;

    // Example 1
    vector<int> stoneValue1 = {6, 2, 3, 4, 5, 5};
    cout << "Example 1: " << sol.stoneGameV(stoneValue1) << " (Expected 18)\n";

    // Example 2
    vector<int> stoneValue2 = {7, 7, 7, 7, 7, 7, 7};
    cout << "Example 2: " << sol.stoneGameV(stoneValue2) << " (Expected 28)\n";

    // Example 3
    vector<int> stoneValue3 = {4};
    cout << "Example 3: " << sol.stoneGameV(stoneValue3) << " (Expected 0)\n";

    return 0;
}


/*
1563. Stone Game V
Hard
Topics
premium lock iconCompanies
Hint

There are several stones arranged in a row, and each stone has an associated value which is an integer given in the array stoneValue.

In each round of the game, Alice divides the row into two non-empty rows (i.e. left row and right row), then Bob calculates the value of each row which is the sum of the values of all the stones in this row. Bob throws away the row which has the maximum value, and Alice's score increases by the value of the remaining row. If the value of the two rows are equal, Bob lets Alice decide which row will be thrown away. The next round starts with the remaining row.

The game ends when there is only one stone remaining. Alice's score is initially zero.

Return the maximum score that Alice can obtain.

 

Example 1:

Input: stoneValue = [6,2,3,4,5,5]
Output: 18
Explanation: In the first round, Alice divides the row to [6,2,3], [4,5,5]. The left row has the value 11 and the right row has value 14. Bob throws away the right row and Alice's score is now 11.
In the second round Alice divides the row to [6], [2,3]. This time Bob throws away the left row and Alice's score becomes 16 (11 + 5).
The last round Alice has only one choice to divide the row which is [2], [3]. Bob throws away the right row and Alice's score is now 18 (16 + 2). The game ends because only one stone is remaining in the row.

Example 2:

Input: stoneValue = [7,7,7,7,7,7,7]
Output: 28

Example 3:

Input: stoneValue = [4]
Output: 0

class Solution {
public:
    int stoneGameV(vector<int>& stoneValue) {
        
    }
};


*/

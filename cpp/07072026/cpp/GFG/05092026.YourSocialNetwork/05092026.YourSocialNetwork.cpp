#include <iostream>
#include <vector>
#include <algorithm> // sort()

class Solution {
public:
    std::vector<std::vector<int>> socialNetwork(std::vector<int>& arr) {
        int n = (int)arr.size() + 1;   // arr has size n-1

        // parent[i] = friend of user i (i is 1-indexed for clarity)
        std::vector<int> parent(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            parent[i] = arr[i - 2];    // friend of user i
        }

        std::vector<std::vector<int>> result;

        // For each user i (2..n), walk up the parent chain.
        // j must be in 1..i-1 and reachable.
        for (int i = 2; i <= n; ++i) {
            int cur = parent[i];
            int dist = 1;

            while (cur != 0) {
                // cur is always < i because parent is always smaller,
                // but keep the check for safety / clarity.
                if (cur < i && cur >= 1) {
                    result.push_back({i, cur, dist});
                }
                cur = parent[cur];
                ++dist;
            }
        }

        // The problem says: for each i, list j from 1..i-1 in increasing
        // order. The parent-chain walk gives j in DECREASING order (largest
        // ancestor first). So sort each i's block by j ascending.
        //
        // Simpler: rebuild result with j sorted per i.
        std::vector<std::vector<int>> sorted_result;
        sorted_result.reserve(result.size());

        int idx = 0;
        for (int i = 2; i <= n; ++i) {
            std::vector<std::vector<int>> block;
            while (idx < (int)result.size() && result[idx][0] == i) {
                block.push_back(result[idx]);
                ++idx;
            }
            // sort block by j ascending (j is block[k][1])
            std::sort(block.begin(), block.end(),
                      [](const std::vector<int>& a, const std::vector<int>& b) {
                          return a[1] < b[1];
                      });
            for (auto& row : block) sorted_result.push_back(row);
        }

        return sorted_result;
    }
};

// ---------- main with tests ----------
int main() {
    Solution sol;

    // Test 1
    {
        std::vector<int> arr = {1, 2};
        auto res = sol.socialNetwork(arr);
        std::cout << "Test 1 (arr = [1,2]) output:\n";
        for (auto& v : res) {
            std::cout << "  [" << v[0] << ", " << v[1] << ", " << v[2] << "]\n";
        }
        // Expected: [[2,1,1],[3,1,2],[3,2,1]]
    }

    // Test 2
    {
        std::vector<int> arr = {1, 1};
        auto res = sol.socialNetwork(arr);
        std::cout << "Test 2 (arr = [1,1]) output:\n";
        for (auto& v : res) {
            std::cout << "  [" << v[0] << ", " << v[1] << ", " << v[2] << "]\n";
        }
        // Expected: [[2,1,1],[3,1,1]]
    }

    // Test 3 — chain 4 -> 3 -> 2 -> 1
    {
        std::vector<int> arr = {1, 2, 3};
        auto res = sol.socialNetwork(arr);
        std::cout << "Test 3 (arr = [1,2,3]) output:\n";
        for (auto& v : res) {
            std::cout << "  [" << v[0] << ", " << v[1] << ", " << v[2] << "]\n";
        }
        // Expected: 2->1 (1)
        //           3->1 (2), 3->2 (1)
        //           4->1 (3), 4->2 (2), 4->3 (1)
    }

    // Test 4 — n = 2, single user with friend 1
    {
        std::vector<int> arr = {1};
        auto res = sol.socialNetwork(arr);
        std::cout << "Test 4 (arr = [1]) output:\n";
        for (auto& v : res) {
            std::cout << "  [" << v[0] << ", " << v[1] << ", " << v[2] << "]\n";
        }
        // Expected: [[2,1,1]]
    }

    return 0;
}

/*

Your Social Network
Difficulty: MediumAccuracy: 63.45%Submissions: 4K+Points: 4Average Time: 20m

Geek is creating a social networking site called Geeksbook with n users numbered from 1 to n. Each user i (2 ≤ i ≤ n) has exactly one friend, and that friend must have a smaller user number than i. User 1 has no friend. The friends of users 2 to n are given in an array arr[] of size n - 1, where:

    arr[0] is the friend of user 2.
    arr[1] is the friend of user 3.
    ...
    arr[i - 2] is the friend of user i.

The relationship is one-way. A user can reach another user by repeatedly following their friend's link. For every user i from 2 to n, find all users j (1 ≤ j < i) that can be reached from i. For every reachable pair (i, j), create an array [i, j, k] where:

    i is the starting user.
    j is the reachable user.
    k is the number of links that must be followed to reach j from i.

The result should contain these arrays in the following order:

    Process users i from 2 to n.
    For each user i, consider users j from 1 to i - 1 in increasing order.
    Include [i, j, k] only if j is reachable from i.

Return a 2D array containing information about all reachable pairs.

Examples:

Input: arr[] = [1, 2]
Output: [[2, 1, 1], [3, 1, 2], [3, 2, 1]]
Explanation: The links are 2 → 1 and 3 → 2. User 2 can reach user 1 in 1 link. User 3 can reach user 1 in 2 links. User 3 can reach user 2 in 1 link.

Input: arr[] = [1, 1]
Output: [[2, 1, 1], [3, 1, 1]]
Explanation: The links are 2 → 1 and 3 → 1. User 2 can reach user 1 in 1 link. User 3 can reach user 1 in 1 link.
class Solution {
  public:
    vector<vector<int>> socialNetwork(vector<int>& arr) {
        // code here
        
    }
};

give this with int main() and header files
*/

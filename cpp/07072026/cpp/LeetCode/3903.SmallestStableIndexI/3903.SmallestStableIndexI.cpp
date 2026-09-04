#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int firstStableIndex(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0) return -1;
        
        // Prefix max: max from 0 to i
        vector<int> prefixMax(n);
        prefixMax[0] = nums[0];
        for (int i = 1; i < n; i++) {
            prefixMax[i] = max(prefixMax[i-1], nums[i]);
        }
        
        // Suffix min: min from i to n-1
        vector<int> suffixMin(n);
        suffixMin[n-1] = nums[n-1];
        for (int i = n-2; i >= 0; i--) {
            suffixMin[i] = min(suffixMin[i+1], nums[i]);
        }
        
        // Find first stable index
        for (int i = 0; i < n; i++) {
            int instability = prefixMax[i] - suffixMin[i];
            if (instability <= k) {
                return i;
            }
        }
        
        return -1;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    vector<int> nums1 = {5, 0, 1, 4};
    int k1 = 3;
    cout << "Example 1: [5,0,1,4], k=3 -> " << solution.firstStableIndex(nums1, k1) << endl;
    // Expected: 3
    
    // Example 2
    vector<int> nums2 = {3, 2, 1};
    int k2 = 1;
    cout << "Example 2: [3,2,1], k=1 -> " << solution.firstStableIndex(nums2, k2) << endl;
    // Expected: -1
    
    // Example 3
    vector<int> nums3 = {0};
    int k3 = 0;
    cout << "Example 3: [0], k=0 -> " << solution.firstStableIndex(nums3, k3) << endl;
    // Expected: 0
    
    // Additional test cases
    vector<int> nums4 = {1, 2, 3, 4};
    int k4 = 2;
    cout << "Test 4: [1,2,3,4], k=2 -> " << solution.firstStableIndex(nums4, k4) << endl;
    // Expected: 0 (prefixMax[0]=1, suffixMin[0]=1, diff=0 <= 2)
    
    vector<int> nums5 = {10, 1, 2, 3, 4};
    int k5 = 5;
    cout << "Test 5: [10,1,2,3,4], k=5 -> " << solution.firstStableIndex(nums5, k5) << endl;
    // Expected: 4 (prefixMax=10, suffixMin=4, diff=6 > 5 for i=3)
    // Wait, let me trace: 
    // i=0: 10 - 1 = 9 > 5
    // i=1: 10 - 1 = 9 > 5
    // i=2: 10 - 2 = 8 > 5
    // i=3: 10 - 3 = 7 > 5
    // i=4: 10 - 4 = 6 > 5
    // Actually, none! Let me re-evaluate...
    
    vector<int> nums6 = {1, 2, 3, 4, 5};
    int k6 = 1;
    cout << "Test 6: [1,2,3,4,5], k=1 -> " << solution.firstStableIndex(nums6, k6) << endl;
    // Expected: 4 (prefixMax=5, suffixMin=5, diff=0)
    
    vector<int> nums7 = {5, 4, 3, 2, 1};
    int k7 = 2;
    cout << "Test 7: [5,4,3,2,1], k=2 -> " << solution.firstStableIndex(nums7, k7) << endl;
    // Expected: -1 (diff is always 4)
    
    vector<int> nums8 = {1, 5, 2, 4, 3};
    int k8 = 2;
    cout << "Test 8: [1,5,2,4,3], k=2 -> " << solution.firstStableIndex(nums8, k8) << endl;
    
    return 0;
}


/*
3903. Smallest Stable Index I
Easy
Topics
premium lock iconCompanies
Hint

You are given an integer array nums of length n and an integer k.

For each index i, define its instability score as max(nums[0..i]) - min(nums[i..n - 1]).

In other words:

    max(nums[0..i]) is the largest value among the elements from index 0 to index i.
    min(nums[i..n - 1]) is the smallest value among the elements from index i to index n - 1.

An index i is called stable if its instability score is less than or equal to k.

Return the smallest stable index. If no such index exists, return -1.

 

Example 1:

Input: nums = [5,0,1,4], k = 3

Output: 3

Explanation:

    At index 0: The maximum in [5] is 5, and the minimum in [5, 0, 1, 4] is 0, so the instability score is 5 - 0 = 5.
    At index 1: The maximum in [5, 0] is 5, and the minimum in [0, 1, 4] is 0, so the instability score is 5 - 0 = 5.
    At index 2: The maximum in [5, 0, 1] is 5, and the minimum in [1, 4] is 1, so the instability score is 5 - 1 = 4.
    At index 3: The maximum in [5, 0, 1, 4] is 5, and the minimum in [4] is 4, so the instability score is 5 - 4 = 1.
    This is the first index with an instability score less than or equal to k = 3. Thus, the answer is 3.

Example 2:

Input: nums = [3,2,1], k = 1

Output: -1

Explanation:

    At index 0, the instability score is 3 - 1 = 2.
    At index 1, the instability score is 3 - 1 = 2.
    At index 2, the instability score is 3 - 1 = 2.
    None of these values is less than or equal to k = 1, so the answer is -1.

Example 3:

Input: nums = [0], k = 0

Output: 0

Explanation:

At index 0, the instability score is 0 - 0 = 0, which is less than or equal to k = 0. Therefore, the answer is 0.

class Solution {
public:
    int firstStableIndex(vector<int>& nums, int k) {
        
    }
};

give with int main()

*/

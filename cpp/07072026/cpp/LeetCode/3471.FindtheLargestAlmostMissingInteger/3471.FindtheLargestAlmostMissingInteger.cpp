#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution
{
public:
    int largestInteger(vector<int>& nums, int k)
    {
        int n = nums.size();

        // If k == n, every element appears in exactly one subarray (the whole array)
        if (k == n)
        {
            // Find the largest element in the array
            return *max_element(nums.begin(), nums.end());
        }

        unordered_map<int, int> subarrayCount; // count of subarrays of size k that contain each integer

        // For each starting index of a subarray of size k
        for (int i = 0; i <= n - k; ++i)
        {
            // Use a set to track which elements are present in this subarray (to avoid double counting)
            unordered_map<int, bool> presentInSubarray;

            // Mark all elements in the current subarray as present
            for (int j = i; j < i + k; ++j)
            {
                presentInSubarray[nums[j]] = true;
            }

            // For each element present in this subarray, increment its count
            for (auto& p : presentInSubarray)
            {
                subarrayCount[p.first]++;
            }
        }

        // Find the largest integer that appears in exactly one subarray
        int result = -1;
        for (auto& p : subarrayCount)
        {
            if (p.second == 1)
            {
                result = max(result, p.first);
            }
        }

        return result;
    }
};

int main()
{
    Solution sol;

    // Example 1
    vector<int> nums1 = {3, 9, 2, 1, 7};
    int k1 = 3;
    cout << "Example 1: " << sol.largestInteger(nums1, k1) << " (Expected 7)\n";

    // Example 2
    vector<int> nums2 = {3, 9, 7, 2, 1, 7};
    int k2 = 4;
    cout << "Example 2: " << sol.largestInteger(nums2, k2) << " (Expected 3)\n";

    // Example 3
    vector<int> nums3 = {0, 0};
    int k3 = 1;
    cout << "Example 3: " << sol.largestInteger(nums3, k3) << " (Expected -1)\n";

    // Test case: [0,0], k = 2
    vector<int> nums4 = {0, 0};
    int k4 = 2;
    cout << "Test case [0,0], k=2: " << sol.largestInteger(nums4, k4) << " (Expected 0)\n";

    // Test case: [1,2,3,4], k = 4
    vector<int> nums5 = {1, 2, 3, 4};
    int k5 = 4;
    cout << "Test case [1,2,3,4], k=4: " << sol.largestInteger(nums5, k5) << " (Expected 4)\n";

    return 0;
}




/*


3471. Find the Largest Almost Missing Integer
Easy
Topics
premium lock iconCompanies
Hint

You are given an integer array nums and an integer k.

An integer x is almost missing from nums if x appears in exactly one subarray of size k within nums.

Return the largest almost missing integer from nums. If no such integer exists, return -1.
A subarray is a contiguous sequence of elements within an array.

 

Example 1:

Input: nums = [3,9,2,1,7], k = 3

Output: 7

Explanation:

    1 appears in 2 subarrays of size 3: [9, 2, 1] and [2, 1, 7].
    2 appears in 3 subarrays of size 3: [3, 9, 2], [9, 2, 1], [2, 1, 7].
    3 appears in 1 subarray of size 3: [3, 9, 2].
    7 appears in 1 subarray of size 3: [2, 1, 7].
    9 appears in 2 subarrays of size 3: [3, 9, 2], and [9, 2, 1].

We return 7 since it is the largest integer that appears in exactly one subarray of size k.

Example 2:

Input: nums = [3,9,7,2,1,7], k = 4

Output: 3

Explanation:

    1 appears in 2 subarrays of size 4: [9, 7, 2, 1], [7, 2, 1, 7].
    2 appears in 3 subarrays of size 4: [3, 9, 7, 2], [9, 7, 2, 1], [7, 2, 1, 7].
    3 appears in 1 subarray of size 4: [3, 9, 7, 2].
    7 appears in 3 subarrays of size 4: [3, 9, 7, 2], [9, 7, 2, 1], [7, 2, 1, 7].
    9 appears in 2 subarrays of size 4: [3, 9, 7, 2], [9, 7, 2, 1].

We return 3 since it is the largest and only integer that appears in exactly one subarray of size k.

Example 3:

Input: nums = [0,0], k = 1

Output: -1

Explanation:

There is no integer that appears in only one subarray of size 1.

class Solution {
public:
    int largestInteger(vector<int>& nums, int k) {
        
    }
};

give this with int main()

*/

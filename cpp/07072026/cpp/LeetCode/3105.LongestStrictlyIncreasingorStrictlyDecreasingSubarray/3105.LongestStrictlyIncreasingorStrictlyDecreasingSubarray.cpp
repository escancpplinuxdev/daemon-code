/*

Dry Run of Final Code
Dry Run 1: nums = [1, 4, 3, 3, 2]

Initial: inc = 1, dec = 1, maxLen = 1.
i	nums[i]	vs prev	inc	dec	maxLen
1	4	4 > 1	2	1	max(1, 2, 1) = 2
2	3	3 < 4	1	2	max(2, 1, 2) = 2
3	3	3 == 3	1	1	max(2, 1, 1) = 2
4	2	2 < 3	1	2	max(2, 1, 2) = 2

Result: 2 ✅
Dry Run 2: nums = [3, 3, 3, 3]

Initial: inc = 1, dec = 1, maxLen = 1.
i	nums[i]	vs prev	inc	dec	maxLen
1	3	==	1	1	1
2	3	==	1	1	1
3	3	==	1	1	1

Result: 1 ✅
Dry Run 3: nums = [3, 2, 1]

Initial: inc = 1, dec = 1, maxLen = 1.
i	nums[i]	vs prev	inc	dec	maxLen
1	2	2 < 3	1	2	2
2	1	1 < 2	1	3	3

Result: 3 ✅
Dry Run 4: nums = [5]

    n == 1 → return 1 ✅

Dry Run 5: nums = [1, 2, 3, 4, 5]
i	nums[i]	vs prev	inc	dec	maxLen
1	2	>	2	1	2
2	3	>	3	1	3
3	4	>	4	1	4
4	5	>	5	1	5

Result: 5 ✅
Dry Run 6: nums = [1, 2, 3, 2, 1]
i	nums[i]	vs prev	inc	dec	maxLen
1	2	>	2	1	2
2	3	>	3	1	3
3	2	<	1	2	3
4	1	<	1	3	3

Result: 3 ✅
Complexity
Metric	Value	Reasoning
Time	O(n)	Single pass through the array
Space	O(1)	Only a few integer variables
Summary
Aspect	Detail
Problem	Longest strictly increasing or strictly decreasing subarray
Key insight	Maintain two running counters inc and dec; reset the other when direction changes
Time	O(n)
Space	O(1)
The "Aha" Moment

At each index, a strictly increasing subarray can only extend if nums[i] > nums[i-1]. If nums[i] <= nums[i-1], the increasing streak must restart at length 1. The same applies symmetrically for decreasing. By tracking both streaks simultaneously and taking the maximum, we find the answer in a single pass.
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        int inc = 1, dec = 1, maxLen = 1;
        for (int i = 1; i < n; i++) {
            if (nums[i] > nums[i-1]) {
                inc++;
                dec = 1;
            } else if (nums[i] < nums[i-1]) {
                dec++;
                inc = 1;
            } else {
                inc = 1;
                dec = 1;
            }
            maxLen = max(maxLen, max(inc, dec));
        }
        return maxLen;
    }
};

🎯 Time: O(n) | Space: O(1)

*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        int n = (int)nums.size();
        if (n == 0) return 0;
        if (n == 1) return 1;

        int inc = 1, dec = 1;
        int maxLen = 1;

        for (int i = 1; i < n; i++) {
            if (nums[i] > nums[i-1]) {
                inc++;
                dec = 1;
            } else if (nums[i] < nums[i-1]) {
                dec++;
                inc = 1;
            } else {
                inc = 1;
                dec = 1;
            }
            maxLen = max(maxLen, max(inc, dec));
        }

        return maxLen;
    }
};

int main() {
    Solution sol;

    // Test 1
    {
        vector<int> nums = {1, 4, 3, 3, 2};
        cout << "Test 1: " << sol.longestMonotonicSubarray(nums)
             << " (expected 2)" << endl;
    }

    // Test 2
    {
        vector<int> nums = {3, 3, 3, 3};
        cout << "Test 2: " << sol.longestMonotonicSubarray(nums)
             << " (expected 1)" << endl;
    }

    // Test 3
    {
        vector<int> nums = {3, 2, 1};
        cout << "Test 3: " << sol.longestMonotonicSubarray(nums)
             << " (expected 3)" << endl;
    }

    // Test 4: single element
    {
        vector<int> nums = {5};
        cout << "Test 4: " << sol.longestMonotonicSubarray(nums)
             << " (expected 1)" << endl;
    }

    // Test 5: strictly increasing
    {
        vector<int> nums = {1, 2, 3, 4, 5};
        cout << "Test 5: " << sol.longestMonotonicSubarray(nums)
             << " (expected 5)" << endl;
    }

    // Test 6: mixed
    {
        vector<int> nums = {1, 2, 3, 2, 1};
        cout << "Test 6: " << sol.longestMonotonicSubarray(nums)
             << " (expected 3)" << endl;
        // Increasing: [1,2,3] length 3
        // Decreasing: [3,2,1] length 3
    }

    return 0;
}

/*

3105. Longest Strictly Increasing or Strictly Decreasing Subarray
Easy
Topics
premium lock iconCompanies

You are given an array of integers nums. Return the length of the longest subarray of nums which is either strictly increasing or strictly decreasing.

 

Example 1:

Input: nums = [1,4,3,3,2]

Output: 2

Explanation:

The strictly increasing subarrays of nums are [1], [2], [3], [3], [4], and [1,4].

The strictly decreasing subarrays of nums are [1], [2], [3], [3], [4], [3,2], and [4,3].

Hence, we return 2.

Example 2:

Input: nums = [3,3,3,3]

Output: 1

Explanation:

The strictly increasing subarrays of nums are [3], [3], [3], and [3].

The strictly decreasing subarrays of nums are [3], [3], [3], and [3].

Hence, we return 1.

Example 3:

Input: nums = [3,2,1]

Output: 3

Explanation:

The strictly increasing subarrays of nums are [3], [2], and [1].

The strictly decreasing subarrays of nums are [3], [2], [1], [3,2], [2,1], and [3,2,1].

Hence, we return 3.

class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        
    }
};

*/

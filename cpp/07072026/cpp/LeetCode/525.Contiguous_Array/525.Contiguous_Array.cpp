/*
525. Contiguous Array
Medium
Topics
premium lock iconCompanies

Given a binary array nums, return the maximum length of a contiguous subarray with an equal number of 0 and 1.

 

Example 1:

Input: nums = [0,1]
Output: 2
Explanation: [0, 1] is the longest contiguous subarray with an equal number of 0 and 1.

Example 2:

Input: nums = [0,1,0]
Output: 2
Explanation: [0, 1] (or [1, 0]) is a longest contiguous subarray with equal number of 0 and 1.

Example 3:

Input: nums = [0,1,1,1,1,1,0,0,0]
Output: 6
Explanation: [1,1,1,0,0,0] is the longest contiguous subarray with equal number of 0 and 1.

*/

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

class Solution 
{
public:
    int findMaxLength(std::vector<int>& nums) 
    {
        int n = nums.size();
        std::unordered_map<int, int> firstSeen;
        int prefixSum = 0;
        int maxLen = 0;

        // Base case: sum 0 seen before the array starts (index -1)
        firstSeen[0] = -1;

        for (int i = 0; i < n; ++i) 
	{
            // Replace 0 with -1, keep 1 as +1
            prefixSum += (nums[i] == 1) ? 1 : -1;

            auto it = firstSeen.find(prefixSum);
            if (it != firstSeen.end()) 
	    {
                // Same prefix sum appeared before → subarray sum = 0
                int length = i - it->second;
                maxLen = std::max(maxLen, length);
            }
	    else
	    {
                // First time seeing this prefix sum
                firstSeen[prefixSum] = i;
            }
        }

        return maxLen;
    }
};

int main() 
{
    Solution sol;

    // Example 3
    std::vector<int> nums3 = {0, 1, 1, 1, 1, 1, 0, 0, 0};
    std::cout << "Input: [0,1,1,1,1,1,0,0,0] -> Output: " << sol.findMaxLength(nums3) << std::endl; // 6

    
}

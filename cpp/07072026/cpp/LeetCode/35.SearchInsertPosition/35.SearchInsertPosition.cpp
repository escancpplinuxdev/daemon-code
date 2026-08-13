
#include <iostream>
#include <vector>

class Solution 
{
public:
    int searchInsert(std::vector<int>& nums, int target) 
    {
        int lo = 0, hi = nums.size() - 1;

        while(lo<=hi)    
        {
            int mid = lo + (hi - lo) / 2;

            if(nums[mid] == target)
            {
                return mid;
            }
            else if (target > nums[mid])
            {
                lo = mid + 1;
            }
            else
            {
                hi = mid - 1;
            }
        }
        return lo;
    }
};

int main()
{
	Solution sol;
//Input: nums = [1,3,5,6], target = 5 -> Output: 2
	std::vector<int> nums1 = {1,3,5,6};
	std::cout<<"o/p = "<<sol.searchInsert(nums1,5)<<"\n";

	std::vector<int> nums2 = {1,3,5,6};
	std::cout<<"o/p = "<<sol.searchInsert(nums2,2)<<"\n";

	std::vector<int> nums3 = {1,3,5,6};
	std::cout<<"o/p = "<<sol.searchInsert(nums3,7)<<"\n";

	return 0;
}

/*
35. Search Insert Position
Easy
Topics
premium lock iconCompanies

Given a sorted array of distinct integers and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.

You must write an algorithm with O(log n) runtime complexity.

 

Example 1:

Input: nums = [1,3,5,6], target = 5
Output: 2

Example 2:

Input: nums = [1,3,5,6], target = 2
Output: 1

Example 3:

Input: nums = [1,3,5,6], target = 7
Output: 4


*/

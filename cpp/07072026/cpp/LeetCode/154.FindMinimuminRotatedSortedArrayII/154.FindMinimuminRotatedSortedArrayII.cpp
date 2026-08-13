
#include <iostream>
#include <vector>
/*
class Solution 
{
	public : 
	int  findMin (std::vector<int> & nums)
	{
		int lo = 0, hi = nums.size() - 1;
		
		while(lo <= hi)
		{
			int mid = lo + (hi - lo) /2;
			
			if(nums[mid] > nums[hi])
			{
				lo = mid + 1;
			}
			else if (nums[mid] < nums[hi])
			{
				hi = mid;
			}
			else
			{
				--hi;
			}
		}
		return nums[lo];
	}
};
*/

class Solution {
public:
    int findMin(std::vector<int>& nums) 
    {
        int lo = 0, hi = nums.size() - 1;    
        while(lo<=hi)
        {
            int mid = lo + (hi - lo ) / 2;

            if(nums[mid]> nums[hi])
            {
                lo = mid + 1;
            }
            else if (nums[mid] < nums[hi])
            {
                hi = mid;
            }
            else
            {
                hi--;
            }
        }
        return nums[lo];
    }
};
int main()
{
	Solution sol;
	std::vector<int> nums1 = {1,3,5};
	std::cout<<"o/p = "<<sol.findMin(nums1)<<"\n";

	std::vector<int> nums2 = {2,2,2,0,1};
	std::cout<<"o/p = "<<sol.findMin(nums2)<<"\n";

	return 0;
}

/*

154. Find Minimum in Rotated Sorted Array II
Hard
Topics
premium lock iconCompanies

Suppose an array of length n sorted in ascending order is rotated between 1 and n times. For example, the array nums = [0,1,4,4,5,6,7] might become:

    [4,5,6,7,0,1,4] if it was rotated 4 times.
    [0,1,4,4,5,6,7] if it was rotated 7 times.

Notice that rotating an array [a[0], a[1], a[2], ..., a[n-1]] 1 time results in the array [a[n-1], a[0], a[1], a[2], ..., a[n-2]].

Given the sorted rotated array nums that may contain duplicates, return the minimum element of this array.

You must decrease the overall operation steps as much as possible.

 

Example 1:

Input: nums = [1,3,5]
Output: 1

Example 2:

Input: nums = [2,2,2,0,1]
Output: 0


*/


#include <iostream>
#include <vector>
/*
   class Solution
   {
   public :
   int findMin(std::vector<int> & nums)
   {
   int lo = 0, hi = nums.size() - 1;

   while(lo<hi)
   {
   int mid = lo + (hi - lo )/2;

   if(nums[mid]>nums[hi])
   {
   lo = mid + 1;
   }
   else
   {
   hi = mid;
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

			while (lo < hi )
			{  
				int mid = lo + (hi - lo) /2;
				std::cout<<"mid = "<<mid<<"\n";
				std::cout<<"hi = "<<hi<<"\n";
				std::cout<<nums[mid]<<" > "<<nums[hi]<<"\t";
				if (nums[mid]> nums[hi])
				{
					std::cout<<"  -> true \n";
					lo = mid + 1;
				}
				else
				{
					std::cout<<"  -> false \n";
					hi = mid;
				}
				std::cout<<"lo = "<<lo<<"\n";
				std::cout<<"hi = "<<hi<<"\n";
			}
			return nums[lo];
		}
};

int main()
{
	Solution sol;
	std::vector<int> nums1 = {3,4,5,1,2};
	std::cout<<"o/p = "<<sol.findMin(nums1)<<"\n";
/*
	std::vector<int> nums2 = {4,5,6,7,0,1,2};
	std::cout<<"o/p = "<<sol.findMin(nums2)<<"\n";

	std::vector<int> nums3 = {11,13,15,17};
	std::cout<<"o/p = "<<sol.findMin(nums3)<<"\n";
*/
	return 0;
}

/*
   153. Find Minimum in Rotated Sorted Array
   Solved
   Medium
   Topics
   premium lock iconCompanies
   Hint

   Suppose an array of length n sorted in ascending order is rotated between 1 and n times. For example, the array nums = [0,1,2,4,5,6,7] might become:

   [4,5,6,7,0,1,2] if it was rotated 4 times.
   [0,1,2,4,5,6,7] if it was rotated 7 times.

   Notice that rotating an array [a[0], a[1], a[2], ..., a[n-1]] 1 time results in the array [a[n-1], a[0], a[1], a[2], ..., a[n-2]].

   Given the sorted rotated array nums of unique elements, return the minimum element of this array.

   You must write an algorithm that runs in O(log n) time.



   Example 1:

Input: nums = [3,4,5,1,2]
Output: 1
Explanation: The original array was [1,2,3,4,5] rotated 3 times.

Example 2:

Input: nums = [4,5,6,7,0,1,2]
Output: 0
Explanation: The original array was [0,1,2,4,5,6,7] and it was rotated 4 times.

Example 3:

Input: nums = [11,13,15,17]
Output: 11
Explanation: The original array was [11,13,15,17] and it was rotated 4 times. 


 */


#include <iostream>
#include <vector>
using namespace std;
/*
   class Solution
   {
   public:
   int search(std::vector<int>&nums, int target)
   {
   int n = nums.size();
   int lo = 0, hi = n- 1;

   while(lo<=hi)
   {
   int mid = lo + (hi - lo ) / 2;
   if(nums[mid] == target ) 
   {
   return mid;
   }

   if(nums[lo]<= nums[mid]) // left half is sorted [lo ... mid]
   {
   if(nums[lo] <= target && target < nums[mid])	
   {
   hi = mid -1; // target is in left sorted half
   }
   else
   {
   lo = mid + 1;	// target is in right sorted half
   }
   }
   else // right half is sorted [mid ... hi]
   {
   if(nums[mid]< target && target <= nums[hi])
   {
   lo = mid + 1;
   }
   else
   {
   hi = mid - 1;
   }
   }

   }
   return -1;
   }
   };
 */


class Solution 
{
	public:
		int search(vector<int>& nums, int target)
		{
			int n = nums.size();
			int lo = 0 , hi = n- 1;

			while(lo<=hi)    
			{
				int mid = lo + (hi - lo )/2;
				if( nums[mid] == target)
				{
					return mid;
				}
				if(nums[lo]<=nums[mid])  // left is sorted  [lo ... mid]
				{    if(nums[lo]<= target && target < nums[mid])  // target is in left side  
					{
						hi = mid -1;
					}
					else // target is in right side
					{
						lo = mid + 1;
					}
				}
				else  // right is sorted  [mid ... hi]
				{
					if (nums[mid]< target && target <= nums[hi])
					{
						lo = mid + 1 ;
					}
					else
					{
						hi = mid -1;
					}
				}
			}
			return -1;
		}
};


int main()
{
	Solution sol;
	std::vector<int > v = {4,5,6,7,0,1,2};
	std::cout<<"o/p = "<<sol.search(v,0)<<"\n";
	return 0;
}

/*


 */

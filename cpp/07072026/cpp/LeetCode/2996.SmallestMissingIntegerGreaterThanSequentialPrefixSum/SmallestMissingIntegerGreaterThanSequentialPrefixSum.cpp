
#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <unordered_set>

class Solution 
{
	public:
		int missingInteger(std::vector<int> &nums )
		{
			int n = nums.size();
			int sum = nums[0];
			int i = 1;
			while (i<n && nums[i] == nums[i-1]+1) // if i = 0 then it start form nums[0-1] + 1 -> error
			{
				sum = sum + nums[i];
				++i;
			}	
			
			while( std::find(nums.begin(), nums.end(), sum) != nums.end())	
			{
				sum++;
			}
			return sum;
		}
	/*	
		int missingInteger1(std::vector<int>& nums) 
		{
			int n = nums.size();
			// 1. Compute sum of the longest sequential prefix
			int sum = nums[0];
			int i = 1;
			while (i < n && nums[i] == nums[i - 1] + 1) 
			{

				sum += nums[i];
				++i;
			}
			//std::cout<<"sum = "<<sum<<"\n";

			// 2. Store all numbers in a hash set for O(1) lookup
			std::unordered_set<int> seen(nums.begin(), nums.end());

			// 3. Find the smallest missing integer >= sum
			int x = sum;
			while (seen.find(x) != seen.end()) 
			{
				x++; 
			//	std::cout<<"++x = "<<x<<"\n";
			}
			return x;
		}
	*/
};

int main()
{
	Solution sol;
	std::vector v = {3,4,5,1,12,14,13};
	std::cout<<"missing Integer = "<<sol.missingInteger(v)<<"\n";

	return 0;
}

/*


 */

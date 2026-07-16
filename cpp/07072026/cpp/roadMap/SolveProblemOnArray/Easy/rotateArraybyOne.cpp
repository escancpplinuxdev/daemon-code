
#include <iostream>
#include <vector>

class Solution 
{
	public:
		void rotateArraybyOne(std::vector<int> &nums)
		{
			int temp = nums[0];
			for(int i = 1 ; i< nums.size(); ++i)
			{
				nums[i-1] = nums[i];
			}
			nums[nums.size() - 1] = temp;
		}
};
int main()
{
	std::vector<int> v = {1,2,3,4,5};
	Solution sol;

	sol.rotateArraybyOne(v);

	for(int i = 0 ; i< v.size(); ++i)
	{
		std::cout<<v[i]<<" ";
	}
	std::cout<<"\n";
	return 0;
}

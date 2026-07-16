
#include <iostream>
#include <vector>

class solution
{
	public:
		int removeDuplicate(std::vector<int>&nums)
		{
			int i = 0;
			for(int j = 1 ; j < nums.size() ; ++j)	
			{
				if(nums[j] != nums[i])
				{
					++i;
					nums[i] = nums[j];
				}
			}
			return i + 1;
		}

};

int main()
{
	solution sol;
	std::vector<int> v = {1,1,1,2,2,3,3,3,4,5,6,7,7,8,8,9,9,9};
	int k = sol.removeDuplicate(v);

	std::cout<<"size of array = "<<k<<"\n";
	for(int i = 0 ; i < k ; ++i)
	{
		std::cout<<v[i]<<" ";
	}
	std::cout<<"\n";
	return 0;
}

/*
time complexity : 	O(N), we traverse the entire original array at only once.
space complexity : 	O(1), constant additional space is used to check unique elements.
*/


#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

class Solution 
{
	public : 
		std::vector<int>twoSum(const std::vector<int>&nums, int target)
		{
			/*				size_t n = nums.size();
							for(size_t i = 0; i< n; ++i)
							{
							for(size_t j = i+1; j<n; ++j)
							{
							if( (nums[i]+nums[j]) == target )
							{
							return{(int)i,(int)j};
							}
							}
							}
							return {};
							}
			 */
		/*
		   std::unordered_map<int, int> map;
		   for (int i = 0; i < (int)nums.size(); ++i) 
		   {
		   int complement = target - nums[i];
		   auto it = map.find(complement);
		   if (it != map.end()) 
		   {
		   return {it->second, i};
		   }
		   map[nums[i]] = i;
		   }
		   return {};


		// 1. Create vector of (value, original_index)
		std::vector<std::pair<int, int>> indexed;
		indexed.reserve(nums.size());
		for (int i = 0; i < (int)nums.size(); ++i)
		indexed.emplace_back(nums[i], i);

		// 2. Sort by value (lexicographic by first, then second)
		std::sort(indexed.begin(), indexed.end());

		// 3. Two‑pointer search
		int left = 0, right = (int)indexed.size() - 1;
		while (left < right) 
		{
		long long sum = (long long)indexed[left].first + indexed[right].first;
		if (sum == target) 
		{
		return {indexed[left].second, indexed[right].second};
		}
		else if (sum < target) 
		{
		++left;
		} 
		else
		{
		--right;
		}
		}
		return {};


		 */
		/*

		   std::unordered_map<int, int> map;
		//		map.reserve(nums.size() * 2);   // ensure capacity, reduce rehash
		for (int i = 0; i < static_cast<int>(nums.size()); ++i) 
		{
		int complement = target - nums[i];
		auto it = map.find(complement);
		if (it != map.end()) 
		{
		//				return {it->second, i};
		std::cout<<it->second<<" , "<<i<<"\n";
		}
		map.emplace(nums[i], i);
		}

		for (auto it = map.begin(); it != map.end(); ++it) 
		{

		std::cout<<it->first<<" | "<<it->second<<"\n";
		}
		return {};
*/
//		std::unordered_map<int,int>map;
		std::map<int,int>map;
		for(size_t i =0; i<nums.size();++i)
		{
			int rem = target - nums[i];
			auto it = map.find(rem);
			if(it != map.end())
			{
				return {it->second, i};
			}
			map.emplace(nums[i],i);
		}
		return {};


}

};

void printV(const std::vector<int>&p)
{
	for(size_t i =0;i<p.size();++i)
	{
		std::cout<<p[i]<<" ";
	}
	std::cout<<"\n";
}

int main()
{
	Solution sol;
	std::vector<int>v= {2,5,7,8};
	printV(v);
	int target = 10;
	std::cout<<"Target = "<<target<<"\n";
	std::vector<int>r = sol.twoSum(v,target);
	std::cout<<"\nsolution = ";
	printV(r);
	return 0;
}

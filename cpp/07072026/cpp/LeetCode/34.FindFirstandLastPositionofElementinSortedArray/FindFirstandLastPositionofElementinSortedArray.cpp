
#include <iostream>
#include <vector>

class Solution
{
	public :
//		std::pair<int,int>searchRange (std::vector<int> &a, int target)
		std::vector<int>searchRange(std::vector<int> &a, int target)
		{
			int lo = 0 , hi = a.size() - 1, first = -1;
			while(lo <= hi)
			{
				int mid = lo + (hi - lo) /2;
				if(a[mid]== target)
				{
					first = mid;
					hi = mid - 1; // keep search left in array for early occurance of target.
				}

				else if(target>a[mid]) // here need else if() {} // if above if() {} failed it has to go else{} 
				{			// but there is no else -> so wrong o/p .
					lo = mid + 1;
				}
				else
				{
					hi = mid - 1;
				}	
			}

			if(first == -1)
			{
				return {-1, -1};
			}

			lo = 0 ; hi = a.size() - 1; 
			int last = -1;

			while(lo<=hi)
			{
				int mid = lo + (hi - lo ) /2;

				if(a[mid] == target)
				{
					last = mid;
					lo = mid + 1;  // keep searching right of array for late occurance.
				}

				else if(target>a[mid])
				{
					lo = mid + 1;
				}
				else
				{
					hi = mid - 1;
				}
			}

			return {first, last};

		}
};
int main()
{	
	Solution sol;

	//std::vector<int> v1 = {5,7,7,8,8,10};
	std::vector<int> v1 = {2,2};
	std::vector res = sol.searchRange(v1,2);
	std::cout<<"element = {"<<res[0]<<" , "<<res[1]<<"}\n";

/*	std::vector<int> v1 = {5,7,7,8,8,10};
	auto it1  = sol.searchRange(v1,8);
	std::cout<<"element = {"<<it1.first<<" , "<<it1.second<<"}\n";

	std::vector<int> v2 = {5,7,7,8,8,10};
	auto it2  = sol.searchRange(v2,6);
	std::cout<<"element = {"<<it2.first<<" , "<<it2.second<<"}\n";

	std::vector<int> v3 = {};
	auto it3  = sol.searchRange(v3,0);
	std::cout<<"element = {"<<it3.first<<" , "<<it3.second<<"}\n";
*/
	return 0;
}

/*


*/

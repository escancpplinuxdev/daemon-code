#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;


class Solution {
	public:
		vector<int> twoSum(vector<int>& nums, int target) {
			unordered_map<int, int> mp; // value -> index

			for (int i = 0; i < nums.size(); i++) {
				int complement = target - nums[i];

				// Check if complement exists
				if (mp.find(complement) != mp.end()) {
					return {mp[complement], i};
				}

				// Store current number with index
				mp[nums[i]] = i;
			}

			return {}; // fallback (not needed as per problem)
		}
};





int main()
{


	int n, target;

	//Input size
	cout<<"Enter number of elements: ";
	cin>>n;

	vector<int> nums(n);

	// Input array;
	cout<<"Enter elements: ";
	for (int i=0; i<n;i++)
	{
		cin>>nums[i];
	}

	//Input target
	cout<<"Enter  target :";
	cin>>target;

	Solution obj;
	vector<int> result = obj.twoSum(nums, target);

	//output result

	cout<< "Indices: ";
	for(int i: result)
	{
		cout<< i<<" ";
	}
	cout<<endl;




	return 0;
}

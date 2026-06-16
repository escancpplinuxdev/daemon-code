
#include <iostream> // for  cout , cin ,endl , string s , to_string
#include <vector> // for vector
using namespace std;

class Solution
{
	public:
		vector<int>twoindex(vector<int>&nums,int target)
		{
			int n =nums.size();
			for(int i =0;i<n;i++)  // start i form Zero to n
			{
				for(int j = i+1;j<n;j++) // for exact one solution, j start form i + 1 not i  to n
				{
					if((nums[i]+nums[j])==target)
					{
						return {i,j}; //addition match to target return { indices }
					}
				}
			}
		return {};
		}
};

void printV(vector<int>&nums)
{
	string s="";
	cout<<"[";
	for(int i=0;i<nums.size();i++)
	{
		cout<<nums[i];
		if(i!=nums.size()-1) // do not add " " to last
		{
			cout<<" ";
		}
	}
	cout<<"] = ";
}

int main()
{
	string s="";
	Solution sol;
	vector <int>nums={2,3,4,6};
	int target = 6;

	printV(nums);
	vector<int> result=sol.twoindex(nums,target);
	for(int index : result)
	{
		s+=to_string(index)+",";
	}
	if(!s.empty()) s.pop_back();   // remove trailing "," form string
	cout<<s;
	cout<<endl;


	return 0;
}

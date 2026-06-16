
#include <iostream>
#include <vector>
using namespace std;

class Solution 
{
	public:
		vector<int> twoSum(vector<int>& nums, int target) // for exact one solution , return {indices of two numbers} they add upto target 
		{
			int n = nums.size();
			for(int i =0; i< n;i++)
			{
				for(int j = i+1;j<n;j++) // 1 st index i i , j= i+1
				{
					if((nums[i]+nums[j]) == target)
					{
						return{i,j};  // if match return {two indices}
					}
				}
			}
			return {};
		}
};

void printV(vector<int>& nums)
{
	/*
	// logic 1
	string s="";
	cout<<"[";
	for(int index : nums)
	{
	//		cout<<index<<" ";
	s += to_string(index)+" ";
	}
	if(!s.empty())
	s.pop_back();
	cout<<s;
	cout<<"] ";    // put all 2 7 11 15 int in string where last " " is remove / trailing space remove

*/	
	//logic 2

	cout<<"[";
	for(int i = 0; i<nums.size();i++)
	{
		cout<<nums[i];
		if(i != nums.size() - 1) // here i start form 0 so last char index is n-1 , so at n-1 remove trailing space 
			cout<<" ";
	}
	cout<<"] ";
}

int main()
{
	Solution sol;
	// test case 1
	vector <int> nums1 = {2,7,11,15};
	int target1=9;
	printV(nums1);
	vector<int>result1=sol.twoSum(nums1,target1); //this is result <int> vector which has two indices number.

	cout<<"Indices for target "<<target1<<" = ";
	for(int idx: result1) cout<<idx<<",";
	cout<<endl;

	//test case 2

	vector <int> nums2 = {3,2,4};
	int target2=6;
	printV(nums2);
	vector<int>result2=sol.twoSum(nums2,target2);

	cout<<"Indices for target "<<target2<<" = ";
	for(int indx: result2)	cout<<indx<<",";
	cout<<endl;

	//test case 3

	vector <int> nums3 = {3,3,4};
	int target3=6;
	printV(nums3);
	vector<int>result3=sol.twoSum(nums3,target3);

	cout<<"Indices for target "<<target3<<" = ";
	for(int indx: result3)	cout<<indx<<",";
	cout<<endl;

	//test case 4

	vector <int> nums4 = {3,5,4};
	int target4=6;
	printV(nums4);
	vector<int>result4=sol.twoSum(nums4,target4);

	cout<<"Indices for target "<<target4<<" = ";
	for(int indx: result4)	cout<<indx<<",";
	cout<<endl;

	//test case 5

	vector <int> nums5 = {3,3,4,2};
	int target5=6;
	printV(nums5);
	vector<int>result5=sol.twoSum(nums5,target5);

	cout<<"Indices for target "<<target5<<" = ";
	for(int indx: result5)	cout<<indx<<",";
	cout<<endl;

	return 0;
}

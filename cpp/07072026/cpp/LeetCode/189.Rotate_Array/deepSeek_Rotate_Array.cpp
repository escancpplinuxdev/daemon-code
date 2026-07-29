/*
   189. Rotate Array
   Medium
   Topics
   premium lock iconCompanies
   Hint

   Given an integer array nums, rotate the array to the right by k steps, where k is non-negative.



   Example 1:

Input: nums = [1,2,3,4,5,6,7], k = 3
Output: [5,6,7,1,2,3,4]
Explanation:
rotate 1 steps to the right: [7,1,2,3,4,5,6]
rotate 2 steps to the right: [6,7,1,2,3,4,5]
rotate 3 steps to the right: [5,6,7,1,2,3,4]

Example 2:

Input: nums = [-1,-100,3,99], k = 2
Output: [3,99,-1,-100]
Explanation: 
rotate 1 steps to the right: [99,-1,-100,3]
rotate 2 steps to the right: [3,99,-1,-100]

*/


#include <iostream>
#include <vector>
#include <algorithm>

void printVector(const std::vector<int> &v, int k)
{
	(void)k;
	for(auto x : v)
	{
		std::cout<<x<<" ";
	}
	std::cout<<"\n";
	std::cout<<"element right rotate to "<<k<<" position\n";

}

class Solution 
{
	public:
		void rotate1(std::vector<int>& nums, int k) 
		{
			int n = nums.size();
			if(n == 0)
			{return ;}
			std::cout<<"nums.size() = '"<<n<<"'\n";
			std::cout<<"k value = '"<<k<<"'\n";
			k = k % n;  // handle cases where k > n
			std::cout<<"k % n = '"<<k<<"'\n";

			// 1. Reverse the whole array
			std::cout<<"rotate entire "<<n<<" th elements\n";
			reverse(nums.begin(), nums.end());
			printVector(nums,k);

			// 2. Reverse the first k elements
			std::cout<<"rotate First "<<k<<" th elements\n";
			reverse(nums.begin(), nums.begin() + k);
			printVector(nums,k);

			// 3. Reverse the remaining n-k elements
			std::cout<<"rotate next  "<<n-k<<" th elements\n";
			reverse(nums.begin() + k, nums.end());
			printVector(nums,k);
		}

		void rotate(std::vector<int>& nums, int k)
		{

			int n = nums.size();
			if(n == 0)
			{ return; }
			k = k % n ;

			std::vector<int> copy = nums;
			for(int i = 0; i<n; ++i)
			{
				nums[(i+k) %n] = copy[i];

			}

		}
};


int main() 
{
	Solution sol;

	// Example 1
	std::vector<int> nums1 = {1, 2, 3, 4, 5, 6, 7};
	int k1 = 3;
	printVector(nums1,k1);
	sol.rotate(nums1, k1);
	std::cout << "Output: [";
	for (int x : nums1) std::cout << x << " ";
	std::cout << "]\n\n";  // [5, 6, 7, 1, 2, 3, 4]

	// Example 2
	std::vector<int> nums2 = {-1, -100, 3, 99};
	int k2 = 2;
	printVector(nums2,k2);
	sol.rotate(nums2, k2);
	std::cout << "Output: [";
	for (int x : nums2) std::cout << x << " ";
	std::cout << "]\n\n";  // [3, 99, -1, -100]

	// Example 3
	std::vector<int> nums3 = {-1,3,5};
	int k3 = 13;
	printVector(nums3,k3);
	sol.rotate(nums3, k3);
	std::cout << "Output: [";
	for (int x : nums3) std::cout << x << " ";
	std::cout << "]\n\n";  // [3, 99, -1, -100]

	// Example 4
	std::vector<int> nums4 = {1,2,2,3,4,5,6,7,5,8,9,6,3,5,8,7};
	int k4 = 13;
	printVector(nums4,k4);
	sol.rotate(nums4, k4);
	std::cout << "Output: [";
	for (int x : nums4) std::cout << x << " ";
	std::cout << "]\n\n"; 

	// Example 5
	std::vector<int> nums5 = {};
	int k5 = 16;
	printVector(nums5,k5);
	sol.rotate(nums5, k5);
	std::cout << "Output: [";
	for (int x : nums5) std::cout << x << " ";
	std::cout << "]\n\n"; 

	return 0;
}

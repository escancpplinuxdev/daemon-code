
#include <iostream>
#include <vector>
class Solution 
{
	public:
		bool isPossible(std::vector<int>& arr, int s, int x) 
		{
			// code here
			if (x == 0) return true;               // empty subset
			if (s > x) return false;               // initial number itself is too large

			std::vector<long long> nums;                // numbers written on paper
			long long sum = s;                     // sum of all numbers generated so far
			nums.push_back(s);

			for (int a : arr) 
			{
				long long next = sum + a;
				if (next > x) break;               // all further numbers will be even larger
				nums.push_back(next);
				sum += next;
			}

			// Greedy from largest to smallest
			long long rem = x;
			for (int i = (int)nums.size() - 1; i >= 0; --i) 
			{
				if (nums[i] <= rem) 
				{
					rem -= nums[i];
				}
			}
			return rem == 0;
		}
};
int main()
{
	Solution sol;

	std::vector<int> arr1 = {1, 2, 4, 2};
	int s1 = 1, x1 = 7;
	std::cout<<"o/p = "<<sol.isPossible(arr1,s1,x1)<<"\n";

	std::vector<int> arr2 = {51, 88};
	int s2 = 100, x2 = 500;
	std::cout<<"o/p = "<<sol.isPossible(arr2,s2,x2)<<"\n";

	return 0;
}

/*
   Subset Sum on Generated Sequence
   Solved
Difficulty: MediumAccuracy: 37.44%Submissions: 18K+Points: 4Average Time: 15m

There are n children standing in a queue, each assigned a number arr[i]. The teacher writes s on a paper and gives it to the first child.

Each child writes the sum of all numbers already on the paper and arr[i], then passes it to the next child.

Return true if x can be formed by adding some of the numbers written on the paper: else return false.

Input: arr[] = [1, 2, 4, 2], s = 1, x = 7
Output: true
Explanation: The first student gets 1 on paper and writes 2. The second student gets [1, 2] and writes 5. The third student gets [1, 2, 5] and writes 12. The final sequence of numbers on the paper is 1, 2, 5, 12, 22. Using 2 and 5 we can form 7. 

Input: arr[] = [51, 88], s = 100, x = 500
Output: false
Explanation: The final sequence of numbers on the paper is 100, 151, 339. Using these numbers we cannot form 500. 

 */


//#include <bits/stdc++.h>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

class Solution
{
public: 
	int missingMultiple(std::vector<int>& nums, int k)
	{
		std::unordered_set <int> seen(nums.begin(),nums.end()); // fill unordered_set 

		int multiple = k; // start form smallest missing multiple.

		while( seen.find(multiple) != seen.end() )  // != seen.end() means element present
		{
			multiple = multiple + k; // increament multiple
		}

		return multiple; // if element not present loop break and got smallest missing multiple.
	}
};



/*
class Solution 
{
public:
    int missingMultiple(vector<int>& nums, int k) 
   {
        // Store all numbers in a set for O(1) lookup
        unordered_set<int> seen(nums.begin(), nums.end());
        
        // Start from the smallest positive multiple of k
        int multiple = k;
        
        // Keep checking multiples until we find one not in the set
        while (seen.find(multiple) != seen.end()) 
	{
            multiple += k;
        }
        
        return multiple;
    }
};
*/
int main() 
{
    Solution sol;
    
    // Example 1
    vector<int> nums1 = {8, 2, 3, 4, 6};
    cout << "Example 1: " << sol.missingMultiple(nums1, 2) << endl;  // Expected: 10
    
    // Example 2
    vector<int> nums2 = {1, 4, 7, 10, 15};
    cout << "Example 2: " << sol.missingMultiple(nums2, 5) << endl;  // Expected: 5
    
    // Additional test
    vector<int> nums3 = {2, 4, 6, 8, 10};
    cout << "Test 3: " << sol.missingMultiple(nums3, 2) << endl;  // Expected: 12
    
    return 0;
}

/*

3718. Smallest Missing Multiple of K
Easy
Topics
premium lock iconCompanies
Hint

Given an integer array nums and an integer k, return the smallest positive multiple of k that is missing from nums.

A multiple of k is any positive integer divisible by k.

 

Example 1:

Input: nums = [8,2,3,4,6], k = 2

Output: 10

Explanation:

The multiples of k = 2 are 2, 4, 6, 8, 10, 12... and the smallest multiple missing from nums is 10.

Example 2:

Input: nums = [1,4,7,10,15], k = 5

Output: 5

Explanation:

The multiples of k = 5 are 5, 10, 15, 20... and the smallest multiple missing from nums is 5.

class Solution {
public:
    int missingMultiple(vector<int>& nums, int k) {
        
    }
};

give this with int main()

*/

#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    int minMoves(vector<int>& arr) 
    {
        int n = arr.size();
	
	         std::cout<<"arr = [";
        for (auto x: arr)
        {
                std::cout<<x<<",";
        }
                std::cout<<"]\t";

        
        // We need to find the longest subsequence of consecutive numbers
        // that are already in increasing order and appear consecutively in value.
        // For permutation 1..n, the longest contiguous increasing subsequence
        // (by value, not by index) that is already in correct relative order.
        
        // pos[value] = index in arr
        vector<int> pos(n + 1);
        for (int i = 0; i < n; i++) 
	{
            pos[arr[i]] = i;
        }

		std::cout<<"pos = [";
	for (auto x: pos)
	{
		std::cout<<x<<",";
	}
		std::cout<<"]\n";
        
        // Find the longest consecutive sequence of numbers that appear in order
        int longest = 1;
        int cur = 1;
        for (int i = 2; i <= n; i++) 
	{
            if (pos[i] > pos[i - 1]) 	
	    {
                cur++; // to find min Move get maximum asscending longest substring
                longest = max(longest, cur);
            }
	    else 
	    {
                cur = 1;
            }
        }
        
        // We need to move all elements except the longest consecutive sequence
        return n - longest;
    }
};

class Solution
{
public: int minMoves(vector<int> &arr)
{

}
	
};

int main() 
{
    Solution sol;
    
    // Example 1
    vector<int> arr1 = {2, 1, 3};
    cout << "Example 1: " << sol.minMoves(arr1) << endl;  // Expected: 1
    
    // Example 2
    vector<int> arr2 = {4, 3, 1, 2};
    cout << "Example 2: " << sol.minMoves(arr2) << endl;  // Expected: 2
    
    // Additional test: already sorted
    vector<int> arr3 = {1, 2, 3, 4, 5};
    cout << "Test 3: " << sol.minMoves(arr3) << endl;  // Expected: 0
    
    // Additional test: reverse sorted
    vector<int> arr4 = {5, 4, 3, 2, 1};
    cout << "Test 4: " << sol.minMoves(arr4) << endl;  // Expected: 4
    
    return 0;
}


/*

Minimum Moves to Sort Permutation
Difficulty: EasyAccuracy: 57.25%Submissions: 6K+Points: 2

Given an array arr[] containing integers from 1 to n exactly once, sort the array in ascending order.

In one operation, you can pick any element and move it either to the beginning or to the end of the array.

Return the minimum number of operations required to sort the array.

Examples:

Input: arr[] = [2, 1, 3]
Output: 1
Explanation: Move 1 to the beginning.

Input: arr[] = [4, 3, 1, 2]
Output: 2
Explanation: Move 3 to the end to get [4, 1, 2, 3]. Then move 4 to the end to get [1, 2, 3, 4].

class Solution {
  public:
    int minMoves(vector<int>& arr) {
        // code here
        
    }
};

give this with int main()

*/

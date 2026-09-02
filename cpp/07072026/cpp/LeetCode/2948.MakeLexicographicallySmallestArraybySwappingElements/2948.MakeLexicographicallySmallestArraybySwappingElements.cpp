#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    vector<int> lexicographicallySmallestArray(vector<int>& nums, int limit) 
    {
        int n = nums.size();
        
        // Store (value, original_index) pairs
        vector<pair<int, int>> pairs;
        for (int i = 0; i < n; i++) 
        {
            pairs.push_back({nums[i], i});
        }
        
        // Sort by value to find connected components
        sort(pairs.begin(), pairs.end());
        
        // Group elements into components where adjacent values differ <= limit
        vector<vector<pair<int, int>>> groups;
        vector<pair<int, int>> currentGroup;
        currentGroup.push_back(pairs[0]);
        
        for (int i = 1; i < n; i++) 
        {
            if (pairs[i].first - pairs[i - 1].first <= limit) 
            {
                currentGroup.push_back(pairs[i]);
            } 
            else 
            {
                groups.push_back(currentGroup);
                currentGroup.clear();
                currentGroup.push_back(pairs[i]);
            }
        }
        if (!currentGroup.empty()) 
        {
            groups.push_back(currentGroup);
        }
        
        // For each group, sort the values and assign them to the positions
        // of the original indices in ascending order
        vector<int> result(n);
        for (auto& group : groups) 
        {
            // Get original indices and their sorted values
            vector<int> indices;
            vector<int> values;
            for (auto& p : group) 
            {
                indices.push_back(p.second);
                values.push_back(p.first);
            }
            
            // Sort indices (they are already sorted by index, but sort to be safe)
            sort(indices.begin(), indices.end());
            
            // Assign the smallest values to the smallest indices
            for (int i = 0; i < (int)indices.size(); i++) 
            {
                result[indices[i]] = values[i];
            }
        }
        
        return result;
    }
};

int main() 
{
    Solution sol;
    
    // Example 1
    vector<int> nums1 = {1, 5, 3, 9, 8};
    vector<int> result1 = sol.lexicographicallySmallestArray(nums1, 2);
    cout << "Example 1: ";
    for (int x : result1) cout << x << " ";
    cout << endl;  // Expected: [1,3,5,8,9]
    
    // Example 2
    vector<int> nums2 = {1, 7, 6, 18, 2, 1};
    vector<int> result2 = sol.lexicographicallySmallestArray(nums2, 3);
    cout << "Example 2: ";
    for (int x : result2) cout << x << " ";
    cout << endl;  // Expected: [1,6,7,18,1,2]
    
    // Example 3
    vector<int> nums3 = {1, 7, 28, 19, 10};
    vector<int> result3 = sol.lexicographicallySmallestArray(nums3, 3);
    cout << "Example 3: ";
    for (int x : result3) cout << x << " ";
    cout << endl;  // Expected: [1,7,28,19,10]
    
    return 0;
}

/*

2948. Make Lexicographically Smallest Array by Swapping Elements
Medium
Topics
premium lock iconCompanies
Hint

You are given a 0-indexed array of positive integers nums and a positive integer limit.

In one operation, you can choose any two indices i and j and swap nums[i] and nums[j] if |nums[i] - nums[j]| <= limit.

Return the lexicographically smallest array that can be obtained by performing the operation any number of times.

An array a is lexicographically smaller than an array b if in the first position where a and b differ, array a has an element that is less than the corresponding element in b. For example, the array [2,10,3] is lexicographically smaller than the array [10,2,3] because they differ at index 0 and 2 < 10.

 

Example 1:

Input: nums = [1,5,3,9,8], limit = 2
Output: [1,3,5,8,9]
Explanation: Apply the operation 2 times:
- Swap nums[1] with nums[2]. The array becomes [1,3,5,9,8]
- Swap nums[3] with nums[4]. The array becomes [1,3,5,8,9]
We cannot obtain a lexicographically smaller array by applying any more operations.
Note that it may be possible to get the same result by doing different operations.

Example 2:

Input: nums = [1,7,6,18,2,1], limit = 3
Output: [1,6,7,18,1,2]
Explanation: Apply the operation 3 times:
- Swap nums[1] with nums[2]. The array becomes [1,6,7,18,2,1]
- Swap nums[0] with nums[4]. The array becomes [2,6,7,18,1,1]
- Swap nums[0] with nums[5]. The array becomes [1,6,7,18,1,2]
We cannot obtain a lexicographically smaller array by applying any more operations.

Example 3:

Input: nums = [1,7,28,19,10], limit = 3
Output: [1,7,28,19,10]
Explanation: [1,7,28,19,10] is the lexicographically smallest array we can obtain because we cannot apply the operation on any two indices.


class Solution {
public:
    vector<int> lexicographicallySmallestArray(vector<int>& nums, int limit) {
        
    }
};

give this with int main()

*/

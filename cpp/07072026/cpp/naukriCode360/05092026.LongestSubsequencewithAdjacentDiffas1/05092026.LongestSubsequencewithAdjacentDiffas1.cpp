#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestSubseq(vector<int>& arr) {
        int n = arr.size();
        if (n == 0) return 0;
        if (n == 1) return 1;
        
        // dp[value] = length of longest subsequence ending with 'value'
        unordered_map<int, int> dp;
        int maxLen = 1;
        
        for (int num : arr) {
            // Current subsequence length ending at 'num'
            int currLen = 1;
            
            // Check if we can extend from num-1
            if (dp.find(num - 1) != dp.end()) {
                currLen = max(currLen, dp[num - 1] + 1);
            }
            
            // Check if we can extend from num+1
            if (dp.find(num + 1) != dp.end()) {
                currLen = max(currLen, dp[num + 1] + 1);
            }
            
            // Update dp for current value
            dp[num] = max(dp[num], currLen);
            
            // Update global maximum
            maxLen = max(maxLen, currLen);
        }
        
        return maxLen;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    vector<int> arr1 = {10, 9, 4, 5, 4, 8, 6};
    cout << "Example 1: [10,9,4,5,4,8,6] -> " << solution.longestSubseq(arr1) << endl;
    // Expected: 3
    
    // Example 2
    vector<int> arr2 = {1, 2, 3, 2, 3, 7, 2, 1};
    cout << "Example 2: [1,2,3,2,3,7,2,1] -> " << solution.longestSubseq(arr2) << endl;
    // Expected: 7
    
    // Additional test cases
    vector<int> arr3 = {1, 2, 3, 4, 5};
    cout << "Test 3: [1,2,3,4,5] -> " << solution.longestSubseq(arr3) << endl;
    // Expected: 5
    
    vector<int> arr4 = {5, 4, 3, 2, 1};
    cout << "Test 4: [5,4,3,2,1] -> " << solution.longestSubseq(arr4) << endl;
    // Expected: 5
    
    vector<int> arr5 = {1, 3, 5, 7, 9};
    cout << "Test 5: [1,3,5,7,9] -> " << solution.longestSubseq(arr5) << endl;
    // Expected: 1 (no adjacent diff = 1)
    
    vector<int> arr6 = {1, 2, 1, 2, 1};
    cout << "Test 6: [1,2,1,2,1] -> " << solution.longestSubseq(arr6) << endl;
    // Expected: 5
    
    vector<int> arr7 = {10, 9, 8, 7, 6, 5};
    cout << "Test 7: [10,9,8,7,6,5] -> " << solution.longestSubseq(arr7) << endl;
    // Expected: 6
    
    vector<int> arr8 = {1, 2, 4, 3, 5, 4, 6};
    cout << "Test 8: [1,2,4,3,5,4,6] -> " << solution.longestSubseq(arr8) << endl;
    // Expected: 4 ([1,2,3,4] or [2,3,4,5] or [3,4,5,6])
    
    vector<int> arr9 = {1, 2, 3, 2, 1};
    cout << "Test 9: [1,2,3,2,1] -> " << solution.longestSubseq(arr9) << endl;
    // Expected: 5
    
    return 0;
}

/*

Longest Subsequence with Adjacent Diff as 1
Difficulty: MediumAccuracy: 29.43%Submissions: 59K+Points: 4

Given an array arr[] with n elements. find the longest subsequence such that the absolute difference between adjacent elements is one.

Examples:

Input : arr[] = [10, 9, 4, 5, 4, 8, 6]
Output : 3
Explanation: Longest subsequences with difference 1 are [10, 9, 8], [4, 5, 4] and [4, 5, 6]. 

Input : arr[] = [1, 2, 3, 2, 3, 7, 2, 1]
Output : 7
Explanation:  Longest subsequences with difference 1 is [1, 2, 3, 2, 3, 2, 1]. 

class Solution {
  public:
    int longestSubseq(vector<int>& arr) {
        // code here
        
    }
};

give this with int main()

*/

/*

Max Adjacent Diffs Sum with 1 Replacements
Difficulty: MediumAccuracy: 53.42%Submissions: 5K+Points: 4

Given an integer array arr[], you are allowed to replace any elements with 1.  Find the maximum sum of absolute differences between consecutive elements after any number of modifications.

Examples:

Input: arr[] = [3, 2, 1, 4, 5]
Output: 8
Explanation: Modify the array as arr[] = [3, 1, 1, 4, 1]. 
Sum = |1-3| + |1-1| + |4-1| + |1-4| = 8, the maximum possible.

Input: arr[] = [1, 5]
Output: 4
Explanation: No modification needed. Sum = |5-1| = 4

class Solution {
  public:
    int maxDiffSum(vector<int>& arr) {
        // code here
        
    }
};

give this with int main()

*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxDiffSum(vector<int>& arr) {
        int n = arr.size();
        if (n <= 1) return 0;
        
        // dp[i][0] = max sum up to index i, where arr[i] is NOT replaced with 1
        // dp[i][1] = max sum up to index i, where arr[i] IS replaced with 1
        
        vector<vector<int>> dp(n, vector<int>(2, 0));
        
        // Base case: first element
        dp[0][0] = 0;  // Keep original value (no difference yet)
        dp[0][1] = 0;  // Replace with 1 (no difference yet)
        
        for (int i = 1; i < n; i++) {
            // Case 1: arr[i] is NOT replaced (keep original)
            // Previous can be either original or replaced
            int prevOriginal = dp[i-1][0] + abs(arr[i] - arr[i-1]);
            int prevReplaced = dp[i-1][1] + abs(arr[i] - 1);
            dp[i][0] = max(prevOriginal, prevReplaced);
            
            // Case 2: arr[i] IS replaced with 1
            // Previous can be either original or replaced
            int prevOriginal2 = dp[i-1][0] + abs(1 - arr[i-1]);
            int prevReplaced2 = dp[i-1][1] + abs(1 - 1); // 1 - 1 = 0
            dp[i][1] = max(prevOriginal2, prevReplaced2);
        }
        
        return max(dp[n-1][0], dp[n-1][1]);
    }
};

int main() {
    Solution solution;
    
    // Example 1
    vector<int> arr1 = {3, 2, 1, 4, 5};
    cout << "Example 1: [3,2,1,4,5] -> " << solution.maxDiffSum(arr1) << endl;
    // Expected: 8
    
    // Example 2
    vector<int> arr2 = {1, 5};
    cout << "Example 2: [1,5] -> " << solution.maxDiffSum(arr2) << endl;
    // Expected: 4
    
    // Additional test cases
    vector<int> arr3 = {1, 2, 3};
    cout << "Test 3: [1,2,3] -> " << solution.maxDiffSum(arr3) << endl;
    // Expected: 4 (keep [1,2,3] -> |2-1| + |3-2| = 1+1=2, or [1,1,3] -> 0+2=2, or [1,2,1] -> 1+1=2, or [1,1,1] -> 0)
    // Wait, let me re-evaluate...
    // [1,2,3]: |2-1| + |3-2| = 1+1=2
    // [1,1,3]: |1-1| + |3-1| = 0+2=2
    // [1,2,1]: |2-1| + |1-2| = 1+1=2
    // [1,1,1]: 0
    // So max is 2? But can we do better?
    // [3,1,3]? No, we can only replace with 1, not 3.
    // Let me check: [3,2,1,4,5] -> [3,1,1,4,1] -> 8
    // For [1,2,3], max is 2? Let me try [1,1,3] -> 0+2=2, [1,2,1] -> 1+1=2.
    // Wait, what about [1,1,2]? -> 0+1=1
    // So max is 2.
    
    vector<int> arr4 = {5, 4, 3, 2, 1};
    cout << "Test 4: [5,4,3,2,1] -> " << solution.maxDiffSum(arr4) << endl;
    // Expected: 8 (replace middle with 1: [5,1,1,2,1] -> 4+0+1+1=6, or [5,1,1,1,1] -> 4+0+0+0=4, or [5,4,1,2,1] -> 1+3+1+1=6)
    // Actually, [5,1,1,2,1] -> |1-5| + |1-1| + |2-1| + |1-2| = 4+0+1+1=6
    // [5,1,1,1,1] -> 4+0+0+0=4
    // [5,4,1,2,1] -> 1+3+1+1=6
    // [5,1,1,2,1] gave 6, but maybe [5,1,1,1,2] -> 4+0+0+1=5
    // What about [5,1,3,1,5]? Can't replace with 3 or 5.
    // Let me think: We want to maximize adjacent differences.
    // For decreasing array, we can replace middle elements with 1 to create spikes.
    // [5,1,1,1,1] -> 4
    // [5,1,1,2,1] -> 4+0+1+1=6
    // [5,4,1,2,1] -> 1+3+1+1=6
    // [5,1,1,1,2] -> 4+0+0+1=5
    // What about [5,1,4,1,5]? Can't replace with 4 or 5.
    // So max might be 6? But wait, we can do [5,1,1,1,1] -> 4, [5,1,1,2,1] -> 6.
    // Actually, what about [5,1,1,1,2] -> 4+0+0+1=5
    // [5,1,2,1,1] -> 4+1+1+0=6
    // So max is 6.
    // But wait, can we do [5,1,1,1,5]? No, can't replace with 5.
    // Let me run the DP and see.
    
    vector<int> arr5 = {10, 1, 10, 1, 10};
    cout << "Test 5: [10,1,10,1,10] -> " << solution.maxDiffSum(arr5) << endl;
    // Expected: 36 (|1-10| + |10-1| + |1-10| + |10-1| = 9+9+9+9=36)
    
    vector<int> arr6 = {1, 1, 1, 1};
    cout << "Test 6: [1,1,1,1] -> " << solution.maxDiffSum(arr6) << endl;
    // Expected: 0 (all differences are 0)
    
    vector<int> arr7 = {5, 5, 5};
    cout << "Test 7: [5,5,5] -> " << solution.maxDiffSum(arr7) << endl;
    // Expected: 8 ([5,1,5] -> 4+4=8)
    
    vector<int> arr8 = {2, 4, 6, 8};
    cout << "Test 8: [2,4,6,8] -> " << solution.maxDiffSum(arr8) << endl;
    // Expected: 12 ([2,1,1,8] -> 1+0+7=8, [2,1,6,1] -> 1+5+5=11, [2,4,1,8] -> 2+3+7=12)
    // Actually [2,4,1,8] -> |4-2| + |1-4| + |8-1| = 2+3+7=12
    
    vector<int> arr9 = {8, 6, 4, 2};
    cout << "Test 9: [8,6,4,2] -> " << solution.maxDiffSum(arr9) << endl;
    // Expected: 12 ([8,1,4,1] -> 7+3+3=13, [8,6,1,2] -> 2+5+1=8, [8,1,1,2] -> 7+0+1=8)
    // [8,1,4,1] -> 7+3+3=13
    // [8,1,1,2] -> 7+0+1=8
    // [8,1,1,1] -> 7
    // So max is 13? Let me try [8,1,1,2] -> 8, [8,1,4,1] -> 13.
    // Wait, [8,1,4,1] sum = |1-8| + |4-1| + |1-4| = 7+3+3=13.
    // But can we do better? [8,1,1,2] -> 7+0+1=8
    // [8,6,1,2] -> 2+5+1=8
    // [8,1,1,1] -> 7
    // So 13 seems max.
    
    return 0;
}

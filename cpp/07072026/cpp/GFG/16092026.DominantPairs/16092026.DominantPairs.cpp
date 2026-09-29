#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int dominantPairs(vector<int>& arr) {
        int n = arr.size();
        int half = n / 2;
        
        // Split into two halves
        vector<int> firstHalf(arr.begin(), arr.begin() + half);
        vector<int> secondHalf(arr.begin() + half, arr.end());
        
        // Sort both halves
        sort(firstHalf.begin(), firstHalf.end());
        sort(secondHalf.begin(), secondHalf.end());
        
        int count = 0;
        
        // For each element in second half, find how many in first half satisfy arr[i] >= 5 * arr[j]
        for (int j = 0; j < secondHalf.size(); j++) {
            int target = 5 * secondHalf[j];
            
            // Find first element in firstHalf >= target
            // Since firstHalf is sorted, all elements from this index onwards satisfy the condition
            int idx = lower_bound(firstHalf.begin(), firstHalf.end(), target) - firstHalf.begin();
            count += (firstHalf.size() - idx);
        }
        
        return count;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    vector<int> arr1 = {10, 2, 2, 1};
    cout << "Example 1: [10,2,2,1] -> " << solution.dominantPairs(arr1) << endl;
    // Expected: 2
    
    // Example 2
    vector<int> arr2 = {10, 8, 2, 1, 1, 2};
    cout << "Example 2: [10,8,2,1,1,2] -> " << solution.dominantPairs(arr2) << endl;
    // Expected: 5
    
    // Additional test cases
    vector<int> arr3 = {5, 5, 1, 1};
    cout << "Test 3: [5,5,1,1] -> " << solution.dominantPairs(arr3) << endl;
    // First: [5,5], Second: [1,1]
    // (5,1): 5 >= 5*1=5 ✅, (5,1): 5 >= 5 ✅
    // Total: 4
    
    vector<int> arr4 = {1, 2, 3, 4};
    cout << "Test 4: [1,2,3,4] -> " << solution.dominantPairs(arr4) << endl;
    // First: [1,2], Second: [3,4]
    // (1,3): 1 >= 15 ❌, (1,4): 1 >= 20 ❌
    // (2,3): 2 >= 15 ❌, (2,4): 2 >= 20 ❌
    // Total: 0
    
    vector<int> arr5 = {10, 20, 1, 2};
    cout << "Test 5: [10,20,1,2] -> " << solution.dominantPairs(arr5) << endl;
    // First: [10,20], Second: [1,2]
    // (10,1): 10 >= 5 ✅, (10,2): 10 >= 10 ✅
    // (20,1): 20 >= 5 ✅, (20,2): 20 >= 10 ✅
    // Total: 4
    
    vector<int> arr6 = {100, 50, 10, 5};
    cout << "Test 6: [100,50,10,5] -> " << solution.dominantPairs(arr6) << endl;
    // First: [50,100], Second: [5,10]
    // (50,5): 50 >= 25 ✅, (50,10): 50 >= 50 ✅
    // (100,5): 100 >= 25 ✅, (100,10): 100 >= 50 ✅
    // Total: 4
    
    vector<int> arr7 = {1, 1, 1, 1};
    cout << "Test 7: [1,1,1,1] -> " << solution.dominantPairs(arr7) << endl;
    // (1,1): 1 >= 5 ❌
    // Total: 0
    
    vector<int> arr8 = {5, 10, 1, 1, 1, 1};
    cout << "Test 8: [5,10,1,1,1,1] -> " << solution.dominantPairs(arr8) << endl;
    // First: [5,10], Second: [1,1,1,1]
    // (5,1): 5 >= 5 ✅ for all 4 ones
    // (10,1): 10 >= 5 ✅ for all 4 ones
    // Total: 8
    
    vector<int> arr9 = {25, 30, 5, 6};
    cout << "Test 9: [25,30,5,6] -> " << solution.dominantPairs(arr9) << endl;
    // First: [25,30], Second: [5,6]
    // (25,5): 25 >= 25 ✅, (25,6): 25 >= 30 ❌
    // (30,5): 30 >= 25 ✅, (30,6): 30 >= 30 ✅
    // Total: 3
    
    return 0;
}


/*

Dominant Pairs
Difficulty: EasyAccuracy: 50.57%Submissions: 39K+Points: 2

Given an even-sized integer array arr[], count the number of dominant pairs. A pair of indices (i, j) is called dominant if all of the following conditions hold:

    0 ≤ i < arr.size() / 2
    arr.size() / 2 ≤ j < arr.size() 
    arr[i] ≥ 5 × arr[j] 

Return the total number of dominant pairs.

Note: 0-based indexing is used.

Examples:

Input: arr[] = [10, 2, 2, 1]
Output: 2
Explanation: First half: [10, 2], Second half: [2, 1]. So valid two pairs are: 
{0, 2}: 10 >= 5 × 2 
{0, 3}: 10 >= 5 × 1 

Input: arr[] = [10, 8, 2, 1, 1, 2]
Output: 5
Explanation: First half: [10, 8, 2], Second half: [1, 1, 2]. So valid five pairs are: 
{0, 3}: 10 >= 5 × 1
{0, 4}: 10 >= 5 × 1 
{0, 5}: 10 >= 5 × 2
{1, 3}: 8 >= 5 × 1 
{1, 4}: 8 >= 5 × 1 

class Solution {
  public:
    int dominantPairs(vector<int> &arr) {
        // Code here
        
    }
};

give this with int main() with header files

*/

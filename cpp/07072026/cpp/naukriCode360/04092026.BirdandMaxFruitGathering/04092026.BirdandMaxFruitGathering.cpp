#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxFruits(vector<int>& arr, int m) {
        int n = arr.size();
        
        // If m >= n, we can collect all fruits
        if (m >= n) {
            int total = 0;
            for (int val : arr) total += val;
            return total;
        }
        
        // Duplicate array to handle circular nature
        vector<int> doubled(n * 2);
        for (int i = 0; i < n; i++) {
            doubled[i] = arr[i];
            doubled[i + n] = arr[i];
        }
        
        // Sliding window to find max sum of length exactly m
        int windowSum = 0;
        for (int i = 0; i < m; i++) {
            windowSum += doubled[i];
        }
        
        int maxSum = windowSum;
        
        for (int i = m; i < n + m; i++) {
            windowSum += doubled[i] - doubled[i - m];
            maxSum = max(maxSum, windowSum);
        }
        
        return maxSum;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    vector<int> arr1 = {2, 1, 3, 5, 0, 1, 4};
    int m1 = 3;
    cout << "Example 1: [2,1,3,5,0,1,4], m=3 -> " << solution.maxFruits(arr1, m1) << endl;
    // Expected: 9
    
    // Example 2
    vector<int> arr2 = {1, 6, 2, 5, 3, 4};
    int m2 = 2;
    cout << "Example 2: [1,6,2,5,3,4], m=2 -> " << solution.maxFruits(arr2, m2) << endl;
    // Expected: 8
    
    // Example 3
    vector<int> arr3 = {7, 2, 1, 3, 4};
    int m3 = 2;
    cout << "Example 3: [7,2,1,3,4], m=2 -> " << solution.maxFruits(arr3, m3) << endl;
    // Expected: 11
    
    // Additional test cases
    vector<int> arr4 = {1, 2, 3, 4, 5};
    int m4 = 3;
    cout << "Test 4: [1,2,3,4,5], m=3 -> " << solution.maxFruits(arr4, m4) << endl;
    // Expected: 12 (3+4+5 or 4+5+1)
    
    vector<int> arr5 = {10, 20, 30, 40, 50};
    int m5 = 4;
    cout << "Test 5: [10,20,30,40,50], m=4 -> " << solution.maxFruits(arr5, m5) << endl;
    // Expected: 140 (20+30+40+50 or 40+50+10+20)
    
    vector<int> arr6 = {5, -2, 3, 4, -1};
    int m6 = 3;
    cout << "Test 6: [5,-2,3,4,-1], m=3 -> " << solution.maxFruits(arr6, m6) << endl;
    // Expected: 10 (3+4+5)
    
    vector<int> arr7 = {1, 2, 3};
    int m7 = 2;
    cout << "Test 7: [1,2,3], m=2 -> " << solution.maxFruits(arr7, m7) << endl;
    // Expected: 5 (2+3 or 3+1)
    
    vector<int> arr8 = {1, 2, 3, 4};
    int m8 = 5;
    cout << "Test 8: [1,2,3,4], m=5 -> " << solution.maxFruits(arr8, m8) << endl;
    // Expected: 10 (all fruits)
    
    return 0;
}


/*

Bird and Max Fruit Gathering
Difficulty: EasyAccuracy: 56.1%Submissions: 4K+Points: 2

Given an array arr[] representing the fruit values of trees arranged in a circle and an integer m, find the maximum total fruits the bird can collect by visiting at most m trees.

    Bird can start from any tree and move to a neighboring tree.
    The first and last trees are also considered neighbors.
    The bird collects the fruit value of every tree it visits.

Examples:

Input: arr[] = [2, 1, 3, 5, 0, 1, 4], m = 3
Output: 9
Explanation: The bird can start from the second tree and visit the second, third, and fourth trees.
The total fruit value collected is 1 + 3 + 5 = 9.

Input: arr[] = [1, 6, 2, 5, 3, 4], m = 2
Output: 8
Explanation: The bird can start from the second tree and visit the second and third trees, collecting 6 + 2 = 8. It can also start from the fourth tree and visit the fourth and fifth trees, collecting 5 + 3 = 8. The maximum total fruit value is 8.

Input: arr[] = [7, 2, 1, 3, 4], m = 2
Output: 11
Explanation: The bird can start from the fifth tree and visit the fifth and first trees, collecting 4 + 7 = 11. These trees are neighbors because the trees are arranged in a circle. The maximum total fruit value is 11.

class Solution {
  public:
    int maxFruits(vector<int>& arr, int m) {
        // code here
        
    }
};

giove this with int main()
*/

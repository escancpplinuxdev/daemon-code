
#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    int countTriplets(vector<int> &arr, int l, int r) 
    {
        int n = arr.size();
        sort(arr.begin(), arr.end());
        
        // Function to count triplets with sum <= target
        auto countLessEqual = [&](int target) -> long long 
        {
            long long count = 0;
            for (int i = 0; i < n - 2; i++) 
            {
                int j = i + 1;
                int k = n - 1;
                while (j < k) 
                {
                    int sum = arr[i] + arr[j] + arr[k];
                    if (sum <= target) 
                    {
                        count += (k - j);
                        j++;
                    } 
                    else 
                    {
                        k--;
                    }
                }
            }
            return count;
        };
        
        return countLessEqual(r) - countLessEqual(l - 1);
    }
};

int main() 
{
    Solution sol;
    
    // Example 1
    vector<int> arr1 = {8, 3, 5, 2};
    cout << "Example 1: " << sol.countTriplets(arr1, 7, 11) << endl;  // Expected: 1
    
    // Example 2
    vector<int> arr2 = {5, 1, 4, 3, 2};
    cout << "Example 2: " << sol.countTriplets(arr2, 2, 7) << endl;   // Expected: 2
    
    // Additional test
    vector<int> arr3 = {1, 2, 3, 4, 5};
    cout << "Example 3: " << sol.countTriplets(arr3, 5, 10) << endl;  // Expected: 3 (1,2,4=7; 1,2,5=8; 1,3,4=8; 1,3,5=9; 1,4,5=10; 2,3,4=9; 2,3,5=10) -> 7? Let's compute: actually (1,2,4), (1,2,5), (1,3,4), (1,3,5), (1,4,5), (2,3,4), (2,3,5) -> 7 triplets with sum between 5 and 10)
    
    return 0;
}


/*
Triplets with Sum in Range
Difficulty: MediumAccuracy: 30.77%Submissions: 21K+Points: 4

Given an array arr[]  and a range from l to r, the task is to count the number of triplets having a sum in the range [l, r].

Examples :

Input: arr = [8, 3, 5, 2], l = 7, r = 11
Output: 1
Explanation: There is only one triplet [2, 3, 5] having sum 10 in range [7, 11].

Input: arr = [5, 1, 4, 3, 2], l = 2, r = 7
Output: 2
Explanation: There are two triplets having sum in range, [1,4,2] and [1,3,2].

class Solution {
  public:
  int countTriplets(vector<int> &arr, int l, int r) {
        // code here
        
    }
};

give this with int main()

*/

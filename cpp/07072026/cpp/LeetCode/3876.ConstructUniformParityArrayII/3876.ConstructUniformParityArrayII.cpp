/*
3876. Construct Uniform Parity Array II
Medium
Topics
premium lock iconCompanies
Hint

You are given an array nums1 of n distinct integers.

You want to construct another array nums2 of length n such that the elements in nums2 are either all odd or all even.

For each index i, you must choose exactly one of the following (in any order):

    nums2[i] = nums1[i]​​​​​​​
    nums2[i] = nums1[i] - nums1[j], for an index j != i, such that nums1[i] - nums1[j] >= 1

Return true if it is possible to construct such an array, otherwise return false.

 

Example 1:

Input: nums1 = [1,4,7]

Output: true

Explanation:​​​​​​​​​​​​​​

    Set nums2[0] = nums1[0] = 1.
    Set nums2[1] = nums1[1] - nums1[0] = 4 - 1 = 3.
    Set nums2[2] = nums1[2] = 7.
    nums2 = [1, 3, 7], and all elements are odd. Thus, the answer is true.

Example 2:

Input: nums1 = [2,3]

Output: false

Explanation:

It is not possible to construct nums2 such that all elements have the same parity. Thus, the answer is false.

Example 3:

Input: nums1 = [4,6]

Output: true

Explanation:

    Set nums2[0] = nums1[0] = 4.
    Set nums2[1] = nums1[1] = 6.
    nums2 = [4, 6], and all elements are even. Thus, the answer is true.

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        
    }
};
give this with int main()

*/

#include <iostream>
#include <climits>
#include <vector>
using namespace std;

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        int n = nums1.size();
        int evenCount = 0, oddCount = 0;
        int minOdd = INT_MAX;
        
        for (int num : nums1) {
            if (num % 2 == 0) {
                evenCount++;
            } else {
                oddCount++;
                minOdd = min(minOdd, num);
            }
        }
        
        // Already uniform
        if (evenCount == n || oddCount == n) return true;
        
        // Check if we can make all odd
        // We need to convert all even numbers to odd
        // even - odd = odd, if even > odd
        if (oddCount >= 1) {
            bool canMakeAllOdd = true;
            for (int num : nums1) {
                if (num % 2 == 0 && num - minOdd < 1) {
                    canMakeAllOdd = false;
                    break;
                }
            }
            if (canMakeAllOdd) return true;
        }
        
        // We cannot make all even if there are any odd numbers
        // Because the smallest odd number cannot become even
        // (needs a smaller odd to subtract from it)
        // The only way to make all even is if all numbers are already even
        // which is already covered by the "Already uniform" check above
        
        return false;
    }
};


int main() 
{
    Solution solution;
    
    // Example 1
    vector<int> nums1 = {1, 4, 7};
    cout << "[1,4,7] -> " << (solution.uniformArray(nums1) ? "true" : "false") << endl;
    // Expected: true
    
    // Example 2
    vector<int> nums2 = {2, 3};
    cout << "[2,3] -> " << (solution.uniformArray(nums2) ? "true" : "false") << endl;
    // Expected: false
    
    // Example 3
    vector<int> nums3 = {4, 6};
    cout << "[4,6] -> " << (solution.uniformArray(nums3) ? "true" : "false") << endl;
    // Expected: true
    
    // Failing test case
    vector<int> nums4 = {2, 7, 11};
    cout << "[2,7,11] -> " << (solution.uniformArray(nums4) ? "true" : "false") << endl;
    // Expected: false
    
    // Additional tests
    vector<int> nums5 = {2, 4, 6, 1, 3};
    cout << "[2,4,6,1,3] -> " << (solution.uniformArray(nums5) ? "true" : "false") << endl;
    // Expected: true (1 is odd, 2-1=1, 4-1=3, 6-1=5 -> all odd)
    
    vector<int> nums6 = {3, 5, 7};
    cout << "[3,5,7] -> " << (solution.uniformArray(nums6) ? "true" : "false") << endl;
    // Expected: true (all odd already)
    
    vector<int> nums7 = {2, 4, 6};
    cout << "[2,4,6] -> " << (solution.uniformArray(nums7) ? "true" : "false") << endl;
    // Expected: true (all even already)
    
    vector<int> nums8 = {1, 2, 3, 4};
    cout << "[1,2,3,4] -> " << (solution.uniformArray(nums8) ? "true" : "false") << endl;
    // Expected: true (2-1=1, 4-1=3 -> all odd with 1,3 remaining odd)
    
    return 0;
}

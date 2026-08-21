
#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:
        int trap (std::vector<int> & height)
        {
                int n = height.size() ; // start form 0.
                int ans = 0;
                int l = 0, r = n-1;
                int lmax = 0, rmax = 0;

                while(l<r)
                {
                        lmax = std::max(lmax,height[l]);
                        rmax = std::max(rmax,height[r]);

                        if(lmax < rmax)
                        {
                                ans += lmax - height[l];
                                l++;
                        }
                        else
                        {
                                ans += rmax - height[r];
                                r--;
                        }
                }
                return ans;
        }
};


int main()
{


         Solution sol;
/*
    // Example 1
    vector<int> height1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Example 1: " << sol.trap(height1) << " (Expected: 6)" << endl;

    // Example 2
    vector<int> height2 = {4, 2, 0, 3, 2, 5};
    cout << "Example 2: " << sol.trap(height2) << " (Expected: 9)" << endl;

    // Additional test cases
    vector<int> height3 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Example 3: " << sol.trap(height3) << " (Expected: 6)" << endl;
*/
    vector<int> height4 = {3, 0, 0, 2, 0, 4};
    cout << "Example 4: " << sol.trap(height4) << " (Expected: 10)" << endl;

/*    vector<int> height5 = {2, 0, 1};
    cout << "Example 5: " << sol.trap(height5) << " (Expected: 1)" << endl;
*/

        return 0;
}










/*

42. Trapping Rain Water
Hard
Topics
premium lock iconCompanies

Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it can trap after raining.

 

Example 1:

Input: height = [0,1,0,2,1,0,1,3,2,1,2,1]
Output: 6
Explanation: The above elevation map (black section) is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped.

Example 2:

Input: height = [4,2,0,3,2,5]
Output: 9

class Solution {
public:
    int trap(vector<int>& height) {
        
    }
};

give with int main()

*/

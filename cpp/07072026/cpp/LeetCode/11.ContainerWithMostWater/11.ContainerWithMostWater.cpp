#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

class Solution
{
public:
    int maxArea(vector<int>& height)
    {
        int left = 0;
        int right = height.size() - 1;
        int maxWater = 0;
	std::unordered_map<std::string , int> bucket;
	

        while (left < right)
        {
            // Calculate the area between the two lines
            int width = right - left;
            int minHeight = min(height[left], height[right]);
            int area = width * minHeight;

	    bucket[to_string(height[left])+" , "+ to_string(height[right])] = area;

            // Update maximum water if current area is larger
            maxWater = max(maxWater, area);

            // Move the pointer pointing to the shorter line
            // Moving the taller line won't increase area because width decreases
            if (height[left] < height[right]) // if any height is smaller then change that height. 
            {
                left++;
            }
            else
            {
                right--;
            }
        }

	for(auto x : bucket)
	{
	
	std::cout<<"["<<x.first<<" ] = "<<x.second<<" \n";
		
	}

        return maxWater;
    }
};

int main()
{
    Solution sol;

    vector<int> height = {1, 8, 6, 2, 5, 4, 6, 8, 3, 7};
    cout << "Example 1: " << sol.maxArea(height) << " (Expected: 56)" << endl;

    // Example 1
    vector<int> height1 = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "Example 1: " << sol.maxArea(height1) << " (Expected: 49)" << endl;

    // Example 2
    vector<int> height2 = {1, 1};
    cout << "Example 2: " << sol.maxArea(height2) << " (Expected: 1)" << endl;

    // Additional test case
    vector<int> height3 = {4, 3, 2, 1, 4};
    cout << "Example 3: " << sol.maxArea(height3) << " (Expected: 16)" << endl;

    vector<int> height4 = {1, 2, 1};
    cout << "Example 4: " << sol.maxArea(height4) << " (Expected: 2)" << endl;

    return 0;
}

/*

11. Container With Most Water
Medium
Topics
premium lock iconCompanies
Hint

You are given an integer array height of length n. There are n vertical lines drawn such that the two endpoints of the ith line are (i, 0) and (i, height[i]).

Find two lines that together with the x-axis form a container, such that the container contains the most water.

Return the maximum amount of water a container can store.

Notice that you may not slant the container.

 

Example 1:

Input: height = [1,8,6,2,5,4,8,3,7]
Output: 49
Explanation: The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.

Example 2:

Input: height = [1,1]
Output: 1

class Solution 
{
public:
    int maxArea(vector<int>& height) 
    {
        
    }
};

give with int main()

*/


#include <iostream>
#include <vector> // std::vector
#include <algorithm> // std::max
using namespace std;
class Solution
{
public:
	int trap(std::vector<int >&height)
	{
		int n = height.size();
		int l = 0, r = n-1; // left = 0 , right = size() -1;
		int water = 0;
		int lmax = 0, rmax = 0;

		while(l<r)
		{
			lmax = std::max(lmax, height[l]);
			rmax = std::max(rmax, height[r]);

			if(lmax<rmax)
			{
				water = water + lmax - height[l];
				l++;
			}
			else
			{
				water = water + rmax -height[r];
				r--;
			}
		}
		return water;
	}

};

int main()
{
	
         Solution sol;
    
// Example 1
    vector<int> height1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Example 1: " << sol.trap(height1) << " (Expected: 6)" << endl;

    // Example 2
    vector<int> height2 = {4, 2, 0, 3, 2, 5};
    cout << "Example 2: " << sol.trap(height2) << " (Expected: 9)" << endl;

    // Additional test cases
    vector<int> height3 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Example 3: " << sol.trap(height3) << " (Expected: 6)" << endl;

    vector<int> height4 = {3, 0, 0, 2, 0, 4};
    cout << "Example 4: " << sol.trap(height4) << " (Expected: 10)" << endl;

    vector<int> height5 = {2, 0, 1};
    cout << "Example 5: " << sol.trap(height5) << " (Expected: 1)" << endl;


	return 0;
}

/*


*/

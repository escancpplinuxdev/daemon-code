#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution
{
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
    {
        // Ensure nums1 is the smaller array for optimal binary search
        if (nums1.size() > nums2.size())
        {
            swap(nums1, nums2);
        }

        int m = nums1.size();
        int n = nums2.size();
        int totalLeft = (m + n + 1) / 2;   // number of elements on the left side

        int low = 0, high = m;  // binary search on nums1 partition index

        while (low <= high)
        {
            int i = (low + high) / 2;          // partition point in nums1
            int j = totalLeft - i;             // partition point in nums2

            // If i == 0, no elements from nums1 on the left; else use nums1[i-1]
            int leftMax1 = (i == 0) ? INT_MIN : nums1[i - 1];
            int rightMin1 = (i == m) ? INT_MAX : nums1[i];
            int leftMax2 = (j == 0) ? INT_MIN : nums2[j - 1];
            int rightMin2 = (j == n) ? INT_MAX : nums2[j];

            if (leftMax1 <= rightMin2 && leftMax2 <= rightMin1)
            {
                // Found correct partition
                if ((m + n) % 2 == 1)
                {
                    // Odd total length: median is the maximum of the left side
                    return max(leftMax1, leftMax2);
                }
                else
                {
                    // Even total length: median is average of max left and min right
                    return (max(leftMax1, leftMax2) + min(rightMin1, rightMin2)) / 2.0;
                }
            }
            else if (leftMax1 > rightMin2)
            {
                // Too many elements from nums1, move partition left
                high = i - 1;
            }
            else
            {
                // leftMax2 > rightMin1, move partition right
                low = i + 1;
            }
        }

        // Should never reach here (input is guaranteed valid)
        return 0.0;
    }
};

int main()
{
    Solution sol;

    // Example 1: [1,3] and [2] → median = 2.0
    vector<int> nums1_1 = {1, 3};
    vector<int> nums2_1 = {2};
    cout << "Example 1: " << sol.findMedianSortedArrays(nums1_1, nums2_1) << endl;

    // Example 2: [1,2] and [3,4] → median = 2.5
    vector<int> nums1_2 = {1, 2};
    vector<int> nums2_2 = {3, 4};
    cout << "Example 2: " << sol.findMedianSortedArrays(nums1_2, nums2_2) << endl;

    // Additional test: [0,0] and [0,0] → median = 0
    vector<int> nums1_3 = {0, 0};
    vector<int> nums2_3 = {0, 0};
    cout << "Example 3: " << sol.findMedianSortedArrays(nums1_3, nums2_3) << endl;

    // Additional test: [] and [1] → median = 1 (though the problem assumes non‑empty, but works)
    vector<int> nums1_4 = {};
    vector<int> nums2_4 = {1};
    cout << "Example 4: " << sol.findMedianSortedArrays(nums1_4, nums2_4) << endl;

    return 0;
}

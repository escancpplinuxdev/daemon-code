#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // Ensure nums1 is the smaller array (to reduce binary search range)
        if (nums1.size() > nums2.size()) {
            swap(nums1, nums2);
        }
        
        int m = nums1.size();
        int n = nums2.size();
        int totalLeft = (m + n + 1) / 2;   // number of elements on the left side of the merged array
        
        int low = 0, high = m;   // binary search on nums1 partition position
        
        while (low <= high) {
            int i = (low + high) / 2;          // partition point in nums1 (elements taken from left)
            int j = totalLeft - i;             // partition point in nums2
            
            // handle edge cases: if i == 0, no left element from nums1; if i == m, no right element from nums1
            int nums1LeftMax = (i == 0) ? INT_MIN : nums1[i - 1];
            int nums1RightMin = (i == m) ? INT_MAX : nums1[i];
            int nums2LeftMax = (j == 0) ? INT_MIN : nums2[j - 1];
            int nums2RightMin = (j == n) ? INT_MAX : nums2[j];
            
            if (nums1LeftMax <= nums2RightMin && nums2LeftMax <= nums1RightMin) {
                // found correct partition
                if ((m + n) % 2 == 1) {
                    // odd total length → median is the max of left half
                    return max(nums1LeftMax, nums2LeftMax);
                } else {
                    // even total length → average of (max left + min right)
                    return (max(nums1LeftMax, nums2LeftMax) + min(nums1RightMin, nums2RightMin)) / 2.0;
                }
            }
            else if (nums1LeftMax > nums2RightMin) {
                // i is too large → move partition left
                high = i - 1;
            }
            else {
                // nums2LeftMax > nums1RightMin → i is too small → move partition right
                low = i + 1;
            }
        }
        
        return 0.0; // should never reach here
    }
};

int main() {
    Solution sol;
    
    // Example 1
    vector<int> nums1_1 = {1, 3};
    vector<int> nums2_1 = {2};
    cout << "Example 1: " << sol.findMedianSortedArrays(nums1_1, nums2_1) << endl;  // expected 2.0
    
    // Example 2
    vector<int> nums1_2 = {1, 2};
    vector<int> nums2_2 = {3, 4};
    cout << "Example 2: " << sol.findMedianSortedArrays(nums1_2, nums2_2) << endl;  // expected 2.5
    
    // Additional test: one empty array
    vector<int> nums1_3 = {};
    vector<int> nums2_3 = {1};
    cout << "Empty + [1]: " << sol.findMedianSortedArrays(nums1_3, nums2_3) << endl; // expected 1.0
    
    return 0;
}

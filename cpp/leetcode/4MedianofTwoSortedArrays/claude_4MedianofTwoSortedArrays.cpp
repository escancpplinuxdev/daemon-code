#include <iostream>
#include <vector>
#include <climits>
#include <stdexcept>

using namespace std;

class Solution
{
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
    {
        // Always binary-search on the smaller array
        // If nums1 is larger, swap so nums1 is always the shorter one
        if (nums1.size() > nums2.size())
            return findMedianSortedArrays(nums2, nums1);

        int m = nums1.size();
        int n = nums2.size();
	
        // half = number of elements that belong on the LEFT side of the median split
        // The +1 handles both even and odd total lengths uniformly:
        //   odd  total (5): half = 3 → left side has 3 elements, right has 2
        //   even total (6): half = 3 → left side has 3 elements, right has 3
        int half = (m + n + 1) / 2;
	cout<<"half : ("<<m<<" + "<<n<<" + 1 )/2 = '"<<half<<"'\n";
        // Binary search range: i can be 0 (no elements from nums1 on left)
        //                      to m (all elements from nums1 on left)
        int lo = 0, hi = m;

        while (lo <= hi)
        {
		cout<<"lo : '"<<lo<<"' <= hi : '"<<hi<<"'\n";
            // i = number of elements taken from nums1 for the left partition
            // j = number of elements taken from nums2 for the left partition
            // Invariant: i + j == half   always
            int i = lo + (hi - lo) / 2;
            int j = half - i;
		cout<<" i = '"<<i<<"'\n";
		cout<<" j = '"<<j<<"'\n";
            // Edge values:
            // If i == 0, nothing is taken from nums1 on the left  → left1  = -INF (sentinel)
            // If i == m, all of nums1 is on the left              → right1 = +INF (sentinel)
            // Same logic applies for j and nums2
            //
            // Visual for nums1 = [1, 3, 5], i = 1:
            //   nums1:  [ 1 | 3, 5 ]
            //             left1  right1
            int left1  = (i == 0) ? INT_MIN : nums1[i - 1];
            int right1 = (i == m) ? INT_MAX : nums1[i];
		cout<<"left1 = '"<<left1<<"' right1 = '"<<right1<<"'\n";
            int left2  = (j == 0) ? INT_MIN : nums2[j - 1];
            int right2 = (j == n) ? INT_MAX : nums2[j];
		cout<<"left2 = '"<<left2<<"' right2 = '"<<right2<<"'\n";

            // --------------- Check if partition is correct ---------------
            // Valid partition requires:
            //   max(left side) <= min(right side)
            //   i.e.  left1 <= right2   AND   left2 <= right1
            //
            // If left1 > right2: i is too big   → move left  (hi = i - 1)
            // If left2 > right1: i is too small → move right (lo = i + 1)
            // Otherwise: perfect split found

            if (left1 > right2)
            {
                hi = i - 1;   // too many elements from nums1 on left side
            }
            else if (left2 > right1)
            {
                lo = i + 1;   // too few elements from nums1 on left side
            }
            else
            {
                // -------- Perfect partition found --------
                //
                // Merged array visualized:
                //   [ ...left1... | ...left2... || ...right1... | ...right2... ]
                //                               ^^ median boundary here
                //
                // For odd total:  median = max(left1, left2)
                //   (left side has one extra element — that's the median)
                //
                // For even total: median = ( max(left1, left2) + min(right1, right2) ) / 2

                if ((m + n) % 2 == 1)
                {
                    return static_cast<double>(max(left1, left2));
                }
                else
                {
                    return (max(left1, left2) + min(right1, right2)) / 2.0;
                }
            }
        }

        // Should never reach here for valid sorted input
        throw invalid_argument("Input arrays are not sorted");
    }
};

// ─────────────────────────── helpers ───────────────────────────

void print_vec(const vector<int>& v)
{
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++)
    {
        cout << v[i];
        if (i + 1 < (int)v.size())
            cout << ",";
    }
    cout << "]";
}

void run_test(vector<int> nums1, vector<int> nums2, double expected)
{
    Solution sol;
    double result = sol.findMedianSortedArrays(nums1, nums2);

    cout << "nums1=";   print_vec(nums1);
    cout << "  nums2="; print_vec(nums2);
    cout << "  → "      << result;
    cout << "  (expected " << expected << ")";
    cout << (abs(result - expected) < 1e-5 ? "  ✓" : "  ✗ FAIL") << "\n";
}

// ─────────────────────────── main ───────────────────────────

int main()
{
    cout << "===== Median of Two Sorted Arrays =====\n\n";

    // Basic examples from the problem
    run_test({1, 3},    {2},       2.0);      // odd  total=3

    run_test({1, 2},    {3, 4},    2.5);      // even total=4

    // Edge: one empty array
    run_test({},        {1},       1.0);      // only nums2
    run_test({2},       {},        2.0);      // only nums1
    run_test({},        {1, 2},    1.5);      // even, only nums2

    // Edge: all elements of one array are smaller
    run_test({1, 2, 3}, {4, 5, 6}, 3.5);     // even total=6
    run_test({1, 2},    {3, 4, 5}, 3.0);     // odd  total=5

    // Duplicate values
    run_test({1, 1},    {1, 1},    1.0);
    run_test({1, 2},    {1, 2},    1.5);

    // Single elements
    run_test({1},       {2},       1.5);
    run_test({2},       {1},       1.5);      // nums1 > nums2 — triggers swap

    // Large value range
    run_test({1, 1000000}, {500000}, 500000.0);

    // Negative numbers
    run_test({-5, -3, -1}, {-4, -2}, -3.0);
    run_test({-2, -1},     {3},      -1.0);

    // Both size 1
    run_test({1},       {3},       2.0);
    run_test({},       {},      0);

    return 0;
}

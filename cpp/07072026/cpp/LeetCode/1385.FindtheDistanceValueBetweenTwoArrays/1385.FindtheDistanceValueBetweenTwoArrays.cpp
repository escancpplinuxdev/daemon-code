#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

class Solution 
{
	public:
	int findTheDistanceValue(vector<int >&arr1, vector<int>& arr2, int d)
	{
		sort(arr2.begin(), arr2.end());
		int count = 0;
		for (int x: arr1)
		{
			auto it = lower_bound(arr2.begin(), arr2.end(), x);
			bool ok = true;
			if (it != arr2.end() && abs(x - *it) <= d)
			{
				ok = false;
			}

			if(it != arr2.begin() && abs(x - *(it-1)) <= d)
			{
				ok = false;
			}

			if(ok)
			{
				count++;
			}
		}
		return count;
	}
};

int main()
{
 Solution sol;

    // Test 1
    vector<int> arr1_1 = {4,5,8};
    vector<int> arr2_1 = {10,9,1,8};
    cout << "Test 1: " << sol.findTheDistanceValue(arr1_1, arr2_1, 2)
         << " (expected 2)" << endl;

    // Test 2
    vector<int> arr1_2 = {1,4,2,3};
    vector<int> arr2_2 = {-4,-3,6,10,20,30};
    cout << "Test 2: " << sol.findTheDistanceValue(arr1_2, arr2_2, 3)
         << " (expected 2)" << endl;

    // Test 3
    vector<int> arr1_3 = {2,1,100,3};
    vector<int> arr2_3 = {-5,-2,10,-3,7};
    cout << "Test 3: " << sol.findTheDistanceValue(arr1_3, arr2_3, 6)
         << " (expected 1)" << endl;

    return 0;
}

/*

1385. Find the Distance Value Between Two Arrays
Easy
Topics
premium lock iconCompanies
Hint

Given two integer arrays arr1 and arr2, and the integer d, return the distance value between the two arrays.

The distance value is defined as the number of elements arr1[i] such that there is not any element arr2[j] where |arr1[i]-arr2[j]| <= d.

 

Example 1:

Input: arr1 = [4,5,8], arr2 = [10,9,1,8], d = 2
Output: 2
Explanation: 
For arr1[0]=4 we have: 
|4-10|=6 > d=2 
|4-9|=5 > d=2 
|4-1|=3 > d=2 
|4-8|=4 > d=2 
For arr1[1]=5 we have: 
|5-10|=5 > d=2 
|5-9|=4 > d=2 
|5-1|=4 > d=2 
|5-8|=3 > d=2
For arr1[2]=8 we have:
|8-10|=2 <= d=2
|8-9|=1 <= d=2
|8-1|=7 > d=2
|8-8|=0 <= d=2

Example 2:

Input: arr1 = [1,4,2,3], arr2 = [-4,-3,6,10,20,30], d = 3
Output: 2

Example 3:

Input: arr1 = [2,1,100,3], arr2 = [-5,-2,10,-3,7], d = 6
Output: 1

class Solution {
public:
    int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
        
    }
};


Dry Run of Final Code
Dry Run 1: arr1 = [4,5,8], arr2 = [10,9,1,8], d = 2

Sorted arr2: [1, 8, 9, 10]

x = 4:

    lower_bound(4) = iterator to 8 (index 1).

    |4-8| = 4 > 2 → OK.

    prev(it) = iterator to 1 (index 0). |4-1| = 3 > 2 → OK.

    No violation → count = 1.

x = 5:

    lower_bound(5) = iterator to 8.

    |5-8| = 3 > 2 → OK.

    prev(it) = 1. |5-1| = 4 > 2 → OK.

    count = 2.

x = 8:

    lower_bound(8) = iterator to 8.

    |8-8| = 0 <= 2 → violation, ok = false.

    count remains 2.

Result: 2 ✅
Dry Run 2: arr1 = [1,4,2,3], arr2 = [-4,-3,6,10,20,30], d = 3

Sorted arr2: [-4, -3, 6, 10, 20, 30]

x = 1:

    lower_bound(1) = iterator to 6.

    |1-6| = 5 > 3 → OK.

    prev(it) = -3. |1-(-3)| = 4 > 3 → OK.

    count = 1.

x = 4:

    lower_bound(4) = iterator to 6.

    |4-6| = 2 <= 3 → violation.

    count remains 1.

x = 2:

    lower_bound(2) = iterator to 6.

    |2-6| = 4 > 3 → OK.

    prev(it) = -3. |2-(-3)| = 5 > 3 → OK.

    count = 2.

x = 3:

    lower_bound(3) = iterator to 6.

    |3-6| = 3 <= 3 → violation.

Result: 2 ✅
Dry Run 3: arr1 = [2,1,100,3], arr2 = [-5,-2,10,-3,7], d = 6

Sorted arr2: [-5, -3, -2, 7, 10]

x = 2:

    lower_bound(2) = iterator to 7.

    |2-7| = 5 <= 6 → violation.

x = 1:

    lower_bound(1) = iterator to 7.

    |1-7| = 6 <= 6 → violation.

x = 100:

    lower_bound(100) = end.

    No check with it.

    prev(it) = 10. |100-10| = 90 > 6 → OK.

    count = 1.

x = 3:

    lower_bound(3) = iterator to 7.

    |3-7| = 4 <= 6 → violation.

Result: 1 ✅
Complexity
Approach	Time	Space
Brute force	O(n × m)	O(1)
Binary search (this)	O(m log m + n log m)	O(1)
Two pointers (after sorting both)	O(n log n + m log m)	O(1)

For n, m ≤ 500, both approaches work. Binary search is generally faster and cleaner.
Summary
Aspect	Detail
Problem	Count elements in arr1 with no element in arr2 within distance d
Key insight	The closest element in sorted arr2 to x is either lower_bound(x) or its predecessor
Approach	Sort arr2, binary search for each arr1[i]
Time	O(m log m + n log m)
Space	O(1) (in-place sort)
The "Aha" Moment

Instead of checking every pair (arr1[i], arr2[j]), we only need to check the closest element in arr2 to arr1[i]. After sorting arr2, the closest element is either the one at lower_bound(x) or the one just before it. This reduces the inner loop from O(m) to O(log m), giving an efficient solution.

*/

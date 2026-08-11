
#include <iostream>
#include <vector>
//#include <algorithm>
#include <climits> // for INT_MIN , INT_MAX

class Solution 
{ 
	public :
		double findMedianSortedArrays(std::vector<int>num1, std::vector<int>num2)
		{
			if(num1.size() > num2.size())
			{
//			return	findMedianSortedArrays(num2,num1); // num1 should smaller array to redue binary search.
				std::swap(num1,num2);
			}

			int m = num1.size();
			int n = num2.size();

			int totalleft = (m+n+1) / 2;
			int low = 0 , high = m ; // here we are working on num1.

			while (low <= high)
			{
				int i = (low + high ) / 2;
				int j = totalleft - i;

				int l1 = ( (i == 0) ? INT_MIN : num1[i - 1] );
				int l2 = ( (j == 0) ? INT_MIN : num2[j - 1] );
				int r1 = ( (i == m) ? INT_MAX : num1[i] );
				int r2 =  ( (j == n) ? INT_MAX : num2[j] );

				if (l1 <= r2 && l2 <= r1)
				{
					if((m+n) % 2 == 1 )
					{
						return std::max(l1,l2);
					}
					else
					{
						return (std::max(l1,l2)+std::min(r1,r2)) / 2.0;
					}
				}
				else if (l1>r2)
				{
					high = i - 1;
				}
				else
				{
					low = i + 1;
				}




			}
			return 0.0;	
		}

};
int main()
{
	Solution sol;
	std::vector<int> num1 = {1,2};
	std::vector<int> num2 = {3,4};
	int median = sol.findMedianSortedArrays(num1,num2);
	std::cout<<"median = "<<median<<"\n";


	// Example 1
	std::vector<int> nums1_1 = {1, 3};
	std::vector<int> nums2_1 = {2};
	std::cout << "Example 1: " << sol.findMedianSortedArrays(nums1_1, nums2_1) << std::endl;  // expected 2.0

	// Example 2
	std::vector<int> nums1_2 = {1, 2};
	std::vector<int> nums2_2 = {3, 4};
	std::cout << "Example 2: " << sol.findMedianSortedArrays(nums1_2, nums2_2) << std::endl;  // expected 2.5

	// Additional test: one empty array
	std::vector<int> nums1_3 = {};
	std::vector<int> nums2_3 = {1};
	std::cout << "Empty + [1]: " << sol.findMedianSortedArrays(nums1_3, nums2_3) << std::endl; // expected 1.0
	return 0;
}

/*


*/

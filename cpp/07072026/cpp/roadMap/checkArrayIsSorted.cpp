
#include <iostream>

class Solution 
{
	public :
		bool isSorted(int a[], int n)
		{
			for(int i = 1; i< n; ++i)
			{
				if(a[i]<a[i-1])
				{
					return false;
				}
			}
			return true;
		}
};
int main()
{
	Solution sol;
	int a[]= {1,5,6,7,9,10};
	int n = sizeof(a)/sizeof(a[0]);
	std::cout<<"is Array sorted : "<<(sol.isSorted(a,n) ? "true" : "false")<<"\n";
	return 0;
}
/*
   Time complexity: 	O(N), as it checks each adjacent pair once in a single pass through the array.
   space complexity :	O(1), as it uses constant extra space regardless of input size.
 */

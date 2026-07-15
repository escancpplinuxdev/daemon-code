
#include <iostream>
#include <climits>

class Solution
{ public :
	int secondSmallestElement(const int a[], int n )
	{
		int smallest = INT_MAX, secondSmallest = INT_MAX ;

		if (n < 2)
		{
			return -1;
		}

		for ( int i = 0 ; i< n ; ++i)
		{
			if(a[i]<smallest)
			{
				secondSmallest = smallest;
				smallest = a[i];
			}
			else if(a[i]<secondSmallest && a[i] > smallest)
			{
				secondSmallest = a[i];
			}
		}
		return secondSmallest;

	}


	int secondLargestElement(const int a[], int n )
	{
		int largest = INT_MIN, secondLargest = INT_MIN;
		if (n < 2)
		{
			return -1;
		}
		for (int i = 0; i< n ; ++i)
		{
			if(a[i]>largest)
			{
				secondLargest = largest;
				largest = a[i];
			}
			else if (a[i]> secondLargest && a[i] < largest)
			{
				secondLargest = a[i];
			}
		}
		return secondLargest;

	}
};
int main()
{
	Solution sol;
	int a[] = {5,6,48,52,19,6,1,7,2,6,8};

	int smallest1 = INT_MAX, secondSmallest1 = INT_MAX ;
	int largest1 = INT_MIN, secondLargest1 = INT_MIN;
	int n = sizeof(a) / sizeof(a[0]);
	secondSmallest1 = sol.secondSmallestElement(a,n);
	std::cout<<"secondSmallest Element = '"<<secondSmallest1<<"'\n";

	secondLargest1 = sol.secondLargestElement(a,n);
	std::cout<<"secondLargest Element = '"<<secondLargest1<<"'\n";

	return 0;
}

/*
   time complexity  : O(N), 	we do two linear traversals in our array.
   space complexity : O(1), 	as we using constant

 */


#include <iostream>

int binarySearch (char  a[],int size, int target)
{
	int low = 0;
	int high = size - 1;

	while (low <= high)	
	{
		int mid = low + (high - low ) / 2;

		if (a[mid] == target )
		{
			return mid;
		}
		else if(a[mid] < target)
		{
			low = mid +1;
		}
		else
		{
			high = mid - 1;
		}

	}
	return -1;
}

int main()
{
	char a [] = {'A','B','C','D','E','F','G','H','I','J','K','L'}; // binary must be sorted for binary search work; // sorted array.

	int size = sizeof(a) / sizeof(a[0]);
	std::cout<<"size = "<<size<<"\n";
	char target;
	std::cout<<"Enter the element to be found in sorted array : ";
	std::cin>>target;
	
	int result = binarySearch(a,size,target);

	if(result != -1 )
	std::cout<<target<<" found at "<<result<<" location. \n";
	else
	std::cout<<target<<" not found. \n";

	return 0;
}

/*


*/

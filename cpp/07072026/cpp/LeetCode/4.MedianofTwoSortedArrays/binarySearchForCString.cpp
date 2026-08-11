
#include <iostream>
#include <cstring> // for strcmp
int binarySearch (const char *a[],int size,const char *target)
{
	int low = 0;
	int high = size - 1;

	while (low <= high)	
	{
		int mid = low + (high - low ) / 2;
	
        // Compare the middle string with the target string	
		int cmp = strcmp(a[mid],target);

		if (cmp == 0 )
		{
			return mid;
		}
        // If target comes after arr[mid] alphabetically, target bigger than a[mid] , so move low forward.
		else if(cmp < 0)
		{
			low = mid +1;
		}
		else // If target comes before arr[mid] alphabetically,target smaller than a[mid] , take high back.
		{
			high = mid - 1;
		}

	}
	return -1;
}

int main()
{
// binary must be sorted for binary search work; // sorted array.
	const char *a[] = {"Apple", "Banana", "Cherry", "Date", "Grape", "Kiwi", "Orange"};

	int size = sizeof(a) / sizeof(a[0]);
	std::cout<<"size = "<<size<<"\n";
//	char target[] = ""; //*** stack smashing detected ***: terminated -> error value gonna write into target variable which has zero space allocated for it.
	char target[100] = "";
//	std::string target = "";
	std::cout<<"Enter the element to be found in sorted array : ";
	std::cin>>target;
	int result = -1;	
try
{
	result = binarySearch(a,size,target);
//	result = binarySearch(a,size,target.c_str());
}
catch (const std::exception &e)
{
	std::cout<<"[ERRROR] : "<<e.what()<<"\n";
}
	
	if(result != -1 )
	std::cout<<target<<" found at "<<result<<" location. \n";
	else
	std::cout<<target<<" not found. \n";

	return 0;
}

/*


*/

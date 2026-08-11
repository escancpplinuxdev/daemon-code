
#include <iostream>
#include <vector>

int binarySearch(std::vector<std::string> v , int n , std::string target)
{
	int low = 0;
	int high = n - 1;
	
	while(low<=high)
	{
		int mid = low + (high - low) / 2;
		if(v[mid] == target)
		{
			return mid;
		}
		else if(v[mid]<target)
		{
			low = mid+1;
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
	std::vector<std::string> v = {"Apple", "Banana", "Cherry", "Date", "Grape", "Kiwi", "Orange"};
	std::string target;
	std::cout<<"Enter string to be found : ";
	std::cin>>target;

	int result = binarySearch(v,v.size(),target);
	
	if(result != -1 )
        std::cout<<target<<" found at "<<result<<" location. \n";
        else
        std::cout<<target<<" not found. \n";

	return 0;
}

/*


*/

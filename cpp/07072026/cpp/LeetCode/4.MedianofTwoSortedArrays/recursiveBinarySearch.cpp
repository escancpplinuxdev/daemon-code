
#include <iostream>
#include <vector>
#include <algorithm> // for std::sort
int binarySerach(std::vector<int>&v,int low, int high , int target)
{
	//	int low = 0 , high = size - 1;

	//	while(low<=high)
	if(low<=high)
	{
		int mid = low + (high - low ) /2;

		if(v[mid] == target)
		{
			return mid;
		}

		if(v[mid]<target)
		{
			//low = mid + 1;
			return	binarySerach(v,mid+1,high,target);
		}
		else
		{
			//high = mid - 1;
			return	binarySerach(v,low,mid-1,target);

		}
	}

	return -1;
}
int main()
{

	std::vector<int> v = {1,3,5,6,7,9,12,21,34,46,72,86,89,91,99};

	std::sort(v.begin(),v.end());

	int target = 46;

	int location = -1;

	location = binarySerach(v,0,v.size() - 1,target);

	if(location != -1)
	{
		std::cout<<target<<" found at "<<location<<" location.\n";
	}
	else
	{
		std::cout<<target<<" not found.\n";
	}	

	return 0;
}

/*


*/

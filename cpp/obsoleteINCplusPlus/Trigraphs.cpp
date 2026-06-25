
#include <iostream>

int main()
{

	int arr??(??) = {1,2,3,4,5,67,7,8,9,0};

	for (int i = 0; i<sizeof(arr)/sizeof(arr[0]); i++)
	{
		std::cout<<arr[i]<<" ";
	}
	std::cout<<"\n";

	std::cout<<"What is this ??\n";
	return 0;
}

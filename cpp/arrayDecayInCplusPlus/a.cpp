
#include <iostream>

void printSize(int arr[])
{
	std::cout<<"Inside function : sizeof(arr) = "<<sizeof(arr)<<"\n";
}

int main()
{
	int arr[10] = {0};
	std::cout<< "Inside main	: sizeof(arr) = "<<sizeof(arr)<<"\n";
	
	printSize(arr);
	return 0;
}

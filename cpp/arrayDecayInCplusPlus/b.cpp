
#include <iostream>


int main()
{
	int arr[5] = {1,2,3,4,5};

	std::cout<<"size = "<<sizeof(arr)<<"\n";
	// Decay to int*
	int *p = arr;
	std::cout<<"arr[0] = "<<*p<<"\n";

	//Pointer arithmetic - decay required
	p = arr + 2;
	std::cout<<"arr[2] = "<<*p<<"\n";

	//auto deduction - decay to int*
	auto a = arr;
	std::cout<<"size = "<<sizeof(arr)<<"\n";
	std::cout<<"type of a is int*; size = "<<sizeof(a)<<"\n";

	return 0;
}

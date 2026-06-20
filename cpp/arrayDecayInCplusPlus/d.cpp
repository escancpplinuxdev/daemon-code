
#include <iostream>

void printArray(int (&arr)[5])
{
	std::cout<<"size preseerved: sizeof(arr) = "<<sizeof(arr)<<"\n";
	for(int x: arr)
	{
		std::cout<<x<<" ";
	}	
	std::cout<<"\n";
}

template<std::size_t N>
void printArrayTemplate(int (&arr)[N])
{
	std::cout<<"Template: size = "<<N<<"\n";
	for(int x: arr)
{
	std::cout<<x<<" ";
}
	std::cout<<"\n";
}
int main()
{
	int arr[5] = {1,2,3,4,5};
	printArray(arr);
	printArrayTemplate(arr);
	return 0;
}

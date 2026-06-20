
#include <iostream>
#include <array>
using namespace std;

void printStdArray(const std::array<int ,5>&arr)
{
	std::cout<<"size = "<<arr.size()<<"\n";
	for(int x: arr)
	{
		std::cout<<x<<" ";
	}
	std::cout<<"\n";
}
int main()
{
	std::array<int , 5>arr = {1,2,3,4,5};
	std::cout<<"sizeof(arr) = "<<sizeof(arr)<<"\n";
	printStdArray(arr);
	return 0;
}

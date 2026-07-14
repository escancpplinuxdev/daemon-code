
#include <iostream>

class A
{
	int a;
	double b;
};

struct B
{
	int a;
	double b;
};
int main()
{
	std::cout<<"sizeof(char) = \t\t"<<sizeof(char)<<"\n";
	std::cout<<"sizeof(signed char) = \t"<<sizeof(signed char)<<"\n";
	std::cout<<"sizeof(unsigned char) = "<<sizeof(unsigned char)<<"\n";
	std::cout<<"sizeof(int) = \t\t"<<sizeof(int)<<"\n";
	std::cout<<"sizeof(signed int) = \t"<<sizeof(signed int)<<"\n";
	std::cout<<"sizeof(unsigned int) = \t"<<sizeof(unsigned int)<<"\n";
	std::cout<<"sizeof(float) = \t"<<sizeof(float)<<"\n";
	std::cout<<"sizeof(double) = \t"<<sizeof(double)<<"\n";
	std::cout<<"sizeof(long) = \t\t"<<sizeof(long)<<"\n";
	std::cout<<"sizeof(long double) = \t"<<sizeof(long double)<<"\n";
	std::cout<<"sizeof(long int) = \t"<<sizeof(long int)<<"\n";
	std::cout<<"sizeof(A) = \t\t"<<sizeof(A)<<"\n";
	std::cout<<"sizeof(B) = \t\t"<<sizeof(B)<<"\n";
	return 0;
}

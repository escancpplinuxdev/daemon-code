
//  g++ -g -Wall -Wextra -Iheader source/a.cpp source/b.cpp source/c.cpp -o a.out
#include "b.h"
#include "c.h"
#include <iostream>


int print(int a, int b)
{
	return a+b;
}
int main()
{
	extern int a,b;
	std::cout<<a<<" + "<<b<<" = "<<print(a,b);
	std::cout<<std::endl;

	return 0;
}

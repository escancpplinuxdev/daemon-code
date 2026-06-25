
#include <iostream>

int main()
{

	for(register int i = 1; i< 10; i++)
	{
		std::cout<<i<<" ";
	}
	std::cout<<"\n";

	return 0;
}

//g++ --std=c++26 -pedantic-errors registeInt.cpp -o registeInt
//this show message

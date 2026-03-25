
#include <iostream>

int main()
{
	int sn = 743e323;	//warning: floating constant exceeds range of ‘double’ [-Woverflow]
				//warning: overflow in conversion from ‘double’ to ‘int’ changes value from ‘+Inf’ to ‘2147483647’ [-Woverflow]

	std::cout<<" sn = "<<sn<<"\n";

	long double ldsn = 1.22e11;

	std::cout<<" ldsn = "<<ldsn<<"\n";

	return 0;
}

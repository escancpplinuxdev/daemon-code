
#include <iostream>
#include <string>

int main()
{
	std::cout<<"Enter Marks : ";
	int n;
	std::cin>>n;
	if(n<35)
	{
		std::cout<<"F\n";
	}
	else if(n<45)
	{
		std::cout<<"E\n";
	}
	else if(n<55)
	{
		std::cout<<"D\n";
	}
	else if(n<75)
	{
		std::cout<<"C\n";
	}
	else if(n<85)
	{
		std::cout<<"B\n";
	}
	else if(n<95)
	{
		std::cout<<"A\n";
	}
	else if(n<=100 && n>=95)
	{
			std::cout<<"A+\n";
	}

	else
	{
		std::cout<<"Error\n";

	}
	return 0;
}

/*


 */

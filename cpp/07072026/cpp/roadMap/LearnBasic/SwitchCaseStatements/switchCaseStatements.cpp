
#include <iostream>
#include <string>
int main()
{
	std::cout<<"Enter number for Day : ";
	char n;
	std::cin>>n;

	switch (n)
	{
		case '1' : std::cout<<"Monday\n";
			 break;
		case '2' : std::cout<<"Tuesday\n";
			 break;
		case '3' : std::cout<<"Wendesday\n";
			 break;
		case '4' : std::cout<<"Thursday\n";
			 break;
		case '5' : std::cout<<"Friday\n";
			 break;
		case '6' : std::cout<<"Saturday\n";
			 break;
		case '7' : std::cout<<"Sunday\n";
			 break;
		default: std::cout<<"Wrong !!\n";
	}

	return 0;
}

/*


 */

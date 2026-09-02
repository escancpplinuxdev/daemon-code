
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
int main()
{
	string s = "hello world\n";
	try 
	{
			
		std::cout<<"try\n";
		char c = s.at(100);
		(void)c;
	}
	catch(std::exception& e)
	{
		std::cout<<"excepltion [Err] : "<<e.what()<<"\n";
	}
	catch(...)
	{
		std::cout<<"[Err] : catch error\n";
	}

	std::cout<<"Exit\n";

	return 0;

}

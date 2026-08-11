
#include <iostream>
#include <vector>
int main()
{
	try
	{
		std::vector<std::pair<int,int>> v = {{10,15}};
	}
	catch(const std::exception & e)
	{
		std::cout<<"ERROR : "<<e.what()<<"\n";
	}

	return 0;
}

/*


*/

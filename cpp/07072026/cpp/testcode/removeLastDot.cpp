
#include <iostream>
#include <string>

void replacetrialingDot(const std::string & szNUrl)
{
	if (!szNUrl.empty() && szNUrl.back()=='.') 
	{
		szNUrl.pop_back();
	}
	
	return;

}
int main()
{
	const char* szNUrl = "connectivity-check.ubuntu.com.";


	std::cout<<"Before = '"<<szNUrl<<"'\n";

	replacetrialingDot(szNUrl);
	
	std::cout<<"After = '"<<szNUrl<<"'\n";



	return 0;
}

/*


*/


#include <iostream>
#include <cstring>
int main()
{
//	char szDec[10] = "fsadjf";
	char szDec[10] ;
	std::cout<<"szDec = '"<<szDec<<"'\n";
	bzero(szDec,sizeof(szDec));
	std::cout<<"szDec = '"<<szDec<<"'\n";
	return 0;
}

/*


*/


#include <iostream>

bool appendExtensionInfo(const std::string & filename)
{
	bool isIni = (filename.size() >=4 && filename.substr(filename.size() - 4 ) == ".ini");

	return isIni;
}
int main()
{
//const char *fileName = "L0escanMon.ini";
const char *fileName = "L0eScanMon.ini";

	if(appendExtensionInfo(fileName))
	{
		std::cout<<"ini \n";
		
	}
	else
	{
		std::cout<<"not \n";
	}
	return 0;
}

/*


*/

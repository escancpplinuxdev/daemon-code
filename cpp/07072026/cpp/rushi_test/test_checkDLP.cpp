
#include <iostream>
#include <fstream>
#include <string>

int checkDLP()
{
	const char * filename = "/opt/MicroWorld/etc/escan-ver";
	std::ifstream file (filename);

	if (!file.is_open())
	{
		std::cerr<<"Error: Cannot open file "<<filename<<std::endl;
		return 1;
	}
	
	std::string line;
	int lineCount = 0;
	bool found = false;

	while(std::getline(file, line))
	{
		++lineCount;
		if(lineCount == 2)
		{
			if(line.find("5CN") != std::string::npos)
			{
				found = true;
			}
			break;
		}
	}


	return found;
}

int main()
{
	if(checkDLP())
	{
		std::cout<<"5CN okay.\n";
	}
	else
	{
		std::cout<<"not 5CN\n";
	}

	return 0;
}

/*


*/

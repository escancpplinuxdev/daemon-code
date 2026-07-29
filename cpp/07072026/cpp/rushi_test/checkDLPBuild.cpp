
#include <iostream>
#include <fstream>
#include <string>

int checkDLP()
{
	const char* filename = "/opt/MicroWorld/etc/escan-ver";
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open file " << filename << std::endl;
		return 1;
	}

	std::string line;
	int lineCount = 0;
	bool found = false;

	while (std::getline(file, line)) 
	{
		++lineCount;
		if (lineCount == 2) 
		{
			// Check if the second line contains "5CN"
			if (line.find("5CN") != std::string::npos) 
			{
				found = true;
			}
			break;  // no need to read further
		}
	}
	return found;

}
int main() 
{
/*
	if (checkDLP)
	When used without parentheses (), it evaluates to the function’s address (a pointer), which is always non‑null.
So the condition is always true.
*/
	if (checkDLP())
	{
		 // in dlp rtscanner depend of fim:status, so do nothing.
		std::cout << "5CN okay" << std::endl;
	}
	else
	{
		std::cout << "not 5CN" << std::endl;
	}

	return 0;
}

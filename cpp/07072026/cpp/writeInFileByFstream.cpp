
#include <iostream>
#include <fstream>
#include <cstring>

bool getVersion(std::string & Version)
{
	if(Version == "")
	{
		std::cout<<"Version is blank.\n";
		return false;
	}
	else
	{
		std::cout<<"Version = "<<Version<<"\n";
	}	

	return true;
}
int main()
{

	std::string AppName = "VLC Media Player";
	std::string Version = "";
	
	AppName += "[Snap]";
	if(getVersion(Version))
	{

	}
	else
	{
		Version += "3.102.5886";
	}

	const char * tmpFile = "/tmpdata/cpp/softwares.list_test";

	// write file
	std::ofstream fout (tmpFile, std::ios::app);
	if(!fout)
	{
		std::cerr<<"[Error] in opening file for writing\n";
	}
	else
	{
		fout<<AppName<<"|"<<Version<<"\n";
		fout.close();
	}

	// read file
	std::ifstream fin (tmpFile);
	if(!fin)
	{
		std::cerr<<"[Error] in opening file for reading\n";
	}
	else
	{
		std::string line;
			if(std::getline(fin,line))
			{
				std::cout<<line<<"\n";
				fin.close();
			}
			else
			{
				std::cout<<"Failed to read line\n";
			}
	}

	return 0;
}


/*
 * Version == "" compares addresses, not the string content.
   if(Version == "") – This compares pointers, not the contents of the strings.
   Version is a pointer to the first character of the array passed from main.
   "" is a string literal stored in a read‑only memory area.
   These two addresses are different, so the condition is always false.

*/

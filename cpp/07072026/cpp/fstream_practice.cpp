
#include <iostream>
#include <fstream>
#include <cstring>

#define PATH "/tmpdata/cpp/07072026/cpp/softwares.list"
void getPackageDetails(char * Version)
{
	if(Version[0] == '\0')
	{
		strcpy(Version,"5.0413.4620");

	}
	return ;
}

int main()
{
	char AppName [52] = "VLC media palyer";
	char Version [52] = "";

	std::string Snap = std::string(AppName) ;
	snprintf(AppName,sizeof(AppName),"%s(Snap)",Snap.c_str());

	getPackageDetails(Version);

	std::cout<<AppName<<"|"<<Version<<"\n";

	const char * fp = PATH;

	// write file (overwrite if exist or create if not)
	std::ofstream fout(fp);

	//	if(!fp)
	if(fp == NULL)
	{
		std::cerr<<"Error write opening file \n";
	}

	fout<<AppName<<"|"<<Version<<"\n";
	fout.close();

	// read file
	std::ifstream fin(fp);

	//	if(!fp)
	if(fp == NULL)
	{
		std::cerr<<"Error read opening file \n";
	}

	std::string line;
	if(std::getline(fin,line))	
	{
		std::cout<<"[Read] "<<line<<"\n";
	}
	else
	{
		std::cerr<<"Failed to read line\n";
	}
	fin.close();
	return 0;

}

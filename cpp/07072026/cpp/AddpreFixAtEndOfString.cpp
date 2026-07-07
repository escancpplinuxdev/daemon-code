
#include <iostream>
#include <cstring>
#include <fstream>

bool getPackageDetails(char * Version)
{
	if(std::strcmp(Version,"") == 0 )
	{
		strcpy(Version,"3.01.0256");
		return false;
	}
	return true;	
}


int main()
{

	char AppName[52] = "";
	char Version[20] = "";
	strcpy(AppName,"VLC media player");
	std::cout<<"AppName = '"<<AppName<<"'\n";
	std::string SnapApp = std::string(AppName) + "[Snap]";
	snprintf(AppName,sizeof(AppName),SnapApp.c_str());
	//	sprintf(AppName,"%s",SnapApp.c_str());
	std::cout<<"AppName = '"<<AppName<<"'\n";
	std::cout<<"strlen(AppName) = '"<<strlen(AppName)<<"'\n";

	char data [512] = "";
	if(0)
	{
		Start:
		std::cout<<"File present \n";
		goto FINSIH;	
	}


	std::cout<<"getPackageDetails Version = '"<<getPackageDetails(Version)<<"'\n";

	const char *tmpFile = "/tmpdata/cpp/softwares.list_test";

//	std::ofstream fout(tmpFile, std::ios::app);
	std::ofstream fout(tmpFile);
	if(!fout)
	{
		std::cout<<"Error open file in append mode \n";
	}

	fout<<AppName<<"|"<<Version<<"\n";
	fout.close();
//	remove(tmpFile);
	FILE *fp = NULL;
	fp = fopen("/tmpdata/cpp/softwares.list_test","a+");
	if(fp == NULL)
	{
		std::cout<<"file not present\n";
		fclose(fp);
	}
	else
	{
		goto Start;
	
	}
FINSIH:
		if(fgets(data,512,fp))
		{
			std::cout<<"data = '"<<data<<"'\n";
		}

	fclose(fp);

	return 0;
}

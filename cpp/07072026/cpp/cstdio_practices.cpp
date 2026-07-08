
#include <iostream>
#include <cstring>
#include <cstdio>

#define PATH "/tmpdata/cpp/07072026/cpp/sw.list"
void getVersion(char * Version)
{
	if(strcmp (Version,"") == 0)
	{
//		snprintf(Version,sizeof(Version),"%s","535.145.154"); //output 12 bytes into a destination of size 8
		strcat(Version,"5.123.546465");
	}
	return;
}

int main()
{
	char AppName[25] = "VLC Media Player";
	char Version[25] = "";

	std::string Snap = std::string(AppName);
	snprintf(AppName,sizeof(AppName),"%s(Snap)",Snap.c_str());

	getVersion(Version);
	std::cout<<AppName<<"|"<<Version<<"\n";

	// write file (overwrite or create)

	const char * tmp = PATH;
	FILE *fp = fopen(tmp,"w");
	if(fp == NULL)
	{
		std::cerr<<"Faild to write open file\n";
	}
	fprintf(fp,"%s|%s",AppName,Version);
	fclose(fp);

	// read file

	fp = fopen(tmp,"r");
	if(fp == NULL)
	{
		std::cerr<<"Failed to open read file\n";
	}
	// line buffer to store	
	char line[512];
	if( fgets (line,sizeof(line),fp))
	{
		std::cout<<"[READ] "<<line<<"\n";
	}
	else
	{
		
		std::cerr<<"Failed to read file\n";
	}
	
	fclose(fp);
	return 0;
}

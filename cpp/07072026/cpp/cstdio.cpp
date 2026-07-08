#include <iostream>
#include <cstring>
#include <cstdio>   // for fopen, fclose, fprintf, fgets

bool getPackageDetails(char * Version)
{
	if (std::strcmp(Version, "") == 0)
	{
		strcpy(Version, "3.01.0256");
		return false;   // default version set
	}
	return true;
}

int main()
{
	char AppName[52] = "VLC media player";
	char Version[20] = "";

	// Add suffix to AppName
	std::string SnapApp = std::string(AppName) + "[Snap]";
	snprintf(AppName, sizeof(AppName), "%s", SnapApp.c_str());

	// Get version (default if empty)
	getPackageDetails(Version);

	// ----- Write to file (overwrite or create) -----
	const char *tmpFile = "/tmpdata/cpp/softwares.list_test";
	FILE *fp = fopen(tmpFile, "w");
	if (!fp)
	{
		std::cerr << "Error opening file for writing.\n";
		return 1;
	}
	fprintf(fp, "%s|%s\n", AppName, Version);
	fclose(fp);   // close after writing

	// ----- Read the file back -----
	fp = fopen(tmpFile, "r");
	if (!fp)
	{
		std::cerr << "Error opening file for reading.\n";
		return 1;
	}
	char line[512];
	if (fgets(line, sizeof(line), fp))
		std::cout << "Read: " << line;   // line already contains newline
	else
		std::cerr << "Failed to read line.\n";

	fclose(fp);
	return 0;
}

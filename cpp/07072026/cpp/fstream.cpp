
#include <iostream>
#include <cstring>
#include <fstream>

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

	// Add suffix
	std::string SnapApp = std::string(AppName) + "[Snap]";
	snprintf(AppName, sizeof(AppName), "%s", SnapApp.c_str());

	// Get version (default if empty)
	getPackageDetails(Version);

	// Write to file (overwrite if exists, create if not)
	const char *tmpFile = "/tmpdata/cpp/softwares.list_test";
	std::ofstream fout(tmpFile);
	if (!fout)
	{
		std::cerr << "Error opening file for writing.\n";
		return 1;
	}
	fout << AppName << "|" << Version << "\n";
	fout.close();

	// Read the file back
	std::ifstream fin(tmpFile);
	if (!fin)
	{
		std::cerr << "Error opening file for reading.\n";
		return 1;
	}
	std::string line;
	if (std::getline(fin, line))
		std::cout << "Read: " << line << "\n";
	else
		std::cerr << "Failed to read line.\n";

	fin.close();
	return 0;
}

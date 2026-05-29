// /usr/bin/google-chrome-stable
// /usr/libexec/ibus-extension-gtk3
// /usr/libexec/xdg-desktop-portal-gtk
#include<iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <regex>
#include <memory>
#include <array>
#include <pwd.h>


std::string exec(const std::string& cmd) 
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
	if (!pipe) return "";
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
		result += buffer.data();
	result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
	return result;
}



//std::string getVersionString(const std::string& output)
std::string getVersionString(const std::string& binaryPath)
{
	std::string output = exec(binaryPath + " --version 2>/dev/null");
	std::string result;
	int dots = 0;

	for(size_t i = 0; i<output.size();++i)
	{
		char c = output[i];

		//Start of a number
		if(std::isdigit(c))
		{
			result.clear();
			dots = 0;
			while(i<output.size() && (std::isdigit(output[i]) || output[i] == '.'))
			{
				result += output[i];
				if(output[i] == '.') 
				{
					++dots;
				}
				++i;
			}
			--i;
			if(dots == 3)
			{
				if(result.back() != '.')
					return result;
			}
		}
	}
	return "";
}
/*
   int main() 
   {
   std::string binaryPath = "google-chrome-stable";
   std::string version = getVersionString(binaryPath);
//    std::string output = exec(binaryPath + " --version 2>/dev/null");
//    std::string version = getVersionString(output);
std::cout << "Version: " << version << std::endl;
return 0;
}
*/

int main()
{
//	std::string version =	getVersionString("/usr/bin/google-chrome-stable");
	std::string version =	getVersionString("google-chrome-stable");
	std::cout << "Version: " << version << std::endl;
}



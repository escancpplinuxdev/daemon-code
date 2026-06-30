
#include <iostream>
#include <string>
#include <cctype>

// trim leading/trailing spaces and collapse mid spaces into one
std::string trim(const std::string& s)
{
	// trim leading / trailing spaces
	size_t start = s.find_first_not_of(" \t");
	if(start == std::string::npos)
	{
		return "";
	}
	size_t end = s.find_last_not_of(" \t");
	std::string trimmed = s.substr(start,end - start + 1);

	std::cout<<"trimmed = '"<<trimmed<<"'\n";

	//	return trimmed;
	// collapse multiple mid spaces into one
	std::string result;
	bool inSpace = false;
	for (size_t i = 0; i < trimmed.length(); ++i )
	{
		if(trimmed[i] == ' ')
		{
			if(!inSpace)
			{
				result += ' ';
				inSpace = true;
			}
			// else skip extra spaces
			// thsy is one space take here not 2nd space
		}
		else
		{
			result += trimmed[i];
			inSpace = false;
		}
	}

	return result;	

}

// convert string to lowercase
std::string toLower(const std::string &s)
{
	std::string result = s;
	for(size_t i = 0 ; i < result.length(); ++i)
	{
		result[i] = std::tolower(static_cast<unsigned char>(result[i]));
		std::cout<<"result["<<i<<"] = '"<<result[i]<<"'\n";
	}
	std::cout<<"result = '"<<result<<"'\n\n";
	return result;
}

int main()
{
	std::string s1 = "Hello World !!";
	std::string s2 = "  hello   world   !!";

	if (toLower(trim(s1)) == toLower(trim(s2)))
		//	if (toLower((s1)) == toLower((s2)))
	{
		std::cout<<"String are equal \n";
	}
	else
	{
		std::cout<<"strings are not equal\n";
	}


	return 0;
}

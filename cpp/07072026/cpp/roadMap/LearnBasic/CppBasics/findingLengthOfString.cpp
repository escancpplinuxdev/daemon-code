
#include <iostream>
#include <cstring>
#include <string>

class Solution
{
	public : 
		size_t lengthOfCharArr(const char* c)
		{
			return strlen(c);
		}

		size_t lengthOfString(const std::string& s)
		{
			return s.length();
		}
};

int main()
{
	Solution sol;

	char c[52]="Hello msg";
	std::string s = "Hello msg";
	size_t lenc = sol.lengthOfString(c);
	size_t lens = sol.lengthOfString(s);

	std::cout<<"Length Of charArr = '"<<lenc<<"'\n";
	std::cout<<"Length Of String  = '"<<lens<<"'\n";

	return 0;
}

/*


*/

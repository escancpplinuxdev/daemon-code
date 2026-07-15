
#include <iostream>

class String
{
	public :
		int strCompare(const std::string& str1, const std::string &str2)
		{
			return (str1 == str2);
		}
};

int main()
{
	String s;
	std::cout<<"Enter str1 & str2\n";
	std::string str1 = "";
	std::cin >> str1;

	std::string str2 = "";
	std::cin >> str2;

	if(s.strCompare(str1,str2))
	{
		std::cout<<"equal\n";
	}
	else
	{
		std::cout<<"not equal\n";
	}
	return 0;
}

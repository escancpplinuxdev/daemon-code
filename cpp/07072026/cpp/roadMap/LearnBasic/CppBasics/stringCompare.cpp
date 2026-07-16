
#include <iostream>

class Solution 
{
	public : 
		bool stringCompare (const std::string s1, const std::string s2)
		{
			return (s1 == s2);
		}
};

int main()
{
	std::string s1;
	std::cin>>s1;
	std::string s2;
	std::cin>>s2;

	Solution sol;

	std::cout<<( sol.stringCompare(s1,s2) ? "true" : "false" )<<"\n";

	return 0;
}
/*


*/

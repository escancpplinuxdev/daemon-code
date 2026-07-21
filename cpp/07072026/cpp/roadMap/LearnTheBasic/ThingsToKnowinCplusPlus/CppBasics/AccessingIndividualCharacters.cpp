
#include <iostream>
#include <cstring>
#include <string>
class Solution
{
	public :
		void AccessingIndividualCharacters(const std::string s)
		{
			std::cout<<"'";
			for(int i = 0 ; i< s.length(); ++i)
			{
				std::cout<<s[i];
				if(i != s.length() - 1)
				{
					std::cout<<" ";
				}
			}
			std::cout<<"'";
			std::cout<<"\n";
			return ;
		}
};
int main()
{
	Solution sol;

//	std::string s = "helle msg\n\0";
	std::string s = "helle msg";
	
	sol.AccessingIndividualCharacters(s);

	return 0;
}

/*


*/

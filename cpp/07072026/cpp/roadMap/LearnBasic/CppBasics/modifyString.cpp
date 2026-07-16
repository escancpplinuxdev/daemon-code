
#include <iostream>
#include <string>

class Solution
{
	public :
		std::string modifyString(std::string s)
		{
			for (int i = 0; i<s.length(); ++i)	
			{
				s[i] = s[i] - 32;
			}
			return s;
		}

};

int main()
{
	Solution sol;
	std::string original = "hello";
	std::string s = sol.modifyString(original);
	std::cout<<original<<" to "<<s<<"\n";
	return 0;
}

/*


*/

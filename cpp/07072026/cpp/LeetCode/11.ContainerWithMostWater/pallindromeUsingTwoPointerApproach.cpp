
#include <iostream>

class Solution
{
public :
	bool pallindrome(std::string s)	
	{
		int left = 0, right = s.length() - 1;
		bool isP = false;
		while (left < right)
		{
			if(s[left] != s[right])
			{
				isP = false;	
			}
			else
			{
				isP = true;
			}

			left ++;
			right --;
		}
		return isP;
	}
};

int main()
{
	Solution sol;

	std::string s = "racar";
	std::cout<<"Enter string = ";
	std::getline(std::cin,s);
	std::cout<<"String "<<s<<" is palindrome = "<<(sol.pallindrome(s) ? "TRUE" : "FALSE" )<<"\n";

	return 0;
}

/*


*/

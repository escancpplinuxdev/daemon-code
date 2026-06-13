
#include <iostream>
using namespace std;

void replacementeveryspacereplacebyPercent20(string &s)
{
	size_t space_count=0;
	size_t i = 0;
	while(s[i] != '\0')
	{
		if(s[i] == ' ')
		{
			space_count++;
		}
		i++;
	}
	cout<<"space_count = '"<<space_count<<"'\n";
	cout<<"i = '"<<i<<"'\n";
	cout<<"s.length() = '"<<s.length()<<"'\n";
	i = s.length();
	cout<<"i = '"<<i<<"'\n";

	size_t j = 2 * space_count + i; // space ' ' replace by 3 character '%' '2' '0'//new length
	s.resize(j);
	cout<<"j = '"<<j<<"'\n";
	cout<<"s.length() = '"<<s.length()<<"'\n";
	while(i > 0) // if i put i >= 0 then it goes to i = 0 to direct 10^16 not -1 for checking i >= 0
	{
		if(s[i] == ' ')
		{
			s[j] = '0';
			s[--j] = '2';
			s[--j] = '%';
			
		}
		else
		{
			s[j] = s[i];
		}
		i--;
		j--;
		cout<<"i = '"<<i<<"'\n";
	}
}

int main()
{

	string s="Hello Wold by Linux c++ !!!";
	cout<<"\n'"<<s<<"'\t";
	replacementeveryspacereplacebyPercent20(s);
	cout<<"\n'"<<s<<"'\n";


	return 0;
}

//Palindrome
#include <iostream>
using namespace std;


int main()
{

	string s;
	getline(cin,s);

	cout<<"s = '"<<s<<"'\n";
//	bool isP=false;
	bool isP=true; //claude
	int size = s.length();
	cout<<"size = "<<size<<endl;
	for(int i = 0; i<size/2;i++)
	{
		if(s[i] == s[size-i-1])
		{
			isP = true;
		}
		else
		{	
			isP = false;
			break;
		}
		cout<<i<<" : "<<s[i]<<"\t"<<size-i-1<<" : "<<s[size-i-1]<<endl;
	
	}
	if(isP){cout<<"Parallendrome\n";}
	else {cout<<"not Parallendrome\n";}

	return 0;
}


#include <iostream>
#include <string>

#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

void print(const string& label, const string& s)
{
        cout << label <<"                                      : \"" << s << "\"" << endl;
}

int main()
{

	cout<<"-----------------------------------string manipulation Demo------------------------------------\n";

	string s;
	cout<<"string s	   				= "<<s<<endl;

	string s1="string";
	cout<<"string s1   				= "<<s1<<endl;
	string s21=s1;
	cout<<"string s2   				= "<<s21<<endl;


	cout<<"s1	    				= \'" << s1 << "\'\n";

	cout<<"s1.length() 				= "<<s1.length()<<endl;

	cout<<"s1.size()   				= "<<s1.size()<<endl;

	cout<<"s1.empty() Is string empty ??		= "<<s1.empty()<<endl;

	cout<<"(s1.empty() ? \"True\" : \"False\" )        = "<<(s1.empty() ? "True" : "False")<<endl;

	cout<<"s1.capacity() 				= "<<s1.capacity()<<endl;

	s1.reserve(100);

	cout<<"After reserve s1.reserve(100) s1.capacity() 				= "<<s1.capacity()<<endl;
	
	s1.shrink_to_fit();

	cout<<"After s1.shrink_to_fit() s1.capacity()  = "<<s1.capacity()<<endl;     

	s1.clear();

	cout<<"After s1.clear() (s1.empty() ? \"True\" : \"False\" )        = "<<(s1.empty() ? "True" : "False")<<endl;

	//Initialization
	string s2="World";

	print("s2",s2);
	string s3(5,'A');
	print("s3",s3);
        string s4(s2);
	print("s4",s4);
	string s5="Hello World";
	print("s5",s5);

	//Access and iteration
	cout<<"Access and iteration\n";
	cout<<"s5[0]                                   = "<<s5[0]<<endl;
	cout<<"s5.front()                              = "<<s5.front()<<endl;
	cout<<"s5.back()                               = "<<s5.back()<<endl;
	print("Before modifying front/back",s5);
	s5.front() = 'h';
	s5.back() = 'd';
	print("After modifying front/back",s5);
	s5[0]='H';
	s5[10]='D';
	print("s5",s5);

	cout<<"Iterating with iterator                 : \"";
	for(auto it = s5.begin();it!= s5.end();++it)
	{
		cout<<*it;
	}

	cout<<"\""<<endl;

	return 0;
}

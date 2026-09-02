
#include <iostream>
#include <string>
#include <cstring>
#include <exception>
using namespace std;


void string_ctor()
{
	string s = "";
	std::cout<<"string s = "<<s<<"\n";
	
	string s1 = "hello !";
	std::cout<<"string s1 = "<<s1<<"\n";
	string s2 = "why are you here, there is not point to dicuss. !";
	std::cout<<"string s2 = "<<s2<<"\n";

	std::cout<<"size of string = "<<sizeof(string)<<"\t bytes\n";
	std::cout<<"size of char   = "<<sizeof(char)<<"\t bytes\n";
	std::cout<<"size of s      = "<<sizeof(s)<<"\t bytes\n";
	std::cout<<"size of s1     = "<<sizeof(s1)<<"\t bytes\n";
	std::cout<<"size of s2     = "<<sizeof(s2)<<"\t bytes\n";

	std::cout<<"s is NULL terminate = "<<( (s[s.size()] == '\0') ? "TRUE" : "FALSE" ) <<"\n";

	string s3("hello",6);
	std::cout<<"string s3 = "<<s3<<"\n";
	std::cout<<"s3.size() = "<< s3.size() <<"\n";


	string s4("hel\0lo",6);
	std::cout<<"string s4 = "<<s4<<"\n";
	std::cout<<"s4.size() = "<< s4.size() <<"\n";

	char s44[56] ="";
	strncpy(s44,s4.c_str(),6);
	std::cout<<"string s44= "<<s44<<"\n";
	std::cout<<"strlen(s44) = "<< strlen(s44) <<"\n";
	


}



void string_elementAccess()
{

	string s="hello world !!";
	std::cout<<"s.size() = "<<s.size()<<"\n";

	try
	{ //s[] not handle exception, o/p = UB
		char m = s[100];
		std::cout<<"m = "<<m<<"\n";
	}
	catch(std::exception& e)
	{
		std::cout<<"s[] Excetion : "<<e.what()<<"\n";
	}

	try
	{ //s.at() handle exception, o/p = show exception
		char m = s.at(100);
		std::cout<<"m = "<<m<<"\n";
	}
	catch(std::exception& e)
	{
		std::cout<<"s.at() Excetion : "<<e.what()<<"\n";
	}

	if(!s.empty())
	{
		std::cout<<"s.front() = "<<s.front()<<"  s.back() = "<<s.back()<<"\n";
	}

	const char * p = s.data();
	std::cout<<"p : "<<p<<"\n";
	std::cout<<"s : "<<s<<"\n";

	char * mp = s.data(); // s.data() give char * and const char* both
	mp[0] = 'H';
	std::cout<<"mp : "<<mp<<"\n";
	std::cout<<"s  : "<<s<<"\n";

	const char * c = s.c_str();   // .c_str() give const char * it can't convert to char * c; it error.
	std::cout<<"c : "<<c<<"\n";
	std::cout<<"s : "<<s<<"\n";

	std::cout<<"s[s.size()] = "<<s[s.size()]<<"\n";

}

void printChar(const string &s)
{
	int len = s.size();

	for(int i = 0; i<len; ++i) std::cout<<s[i]<<" ";
	std::cout<<"\n";

	for(int i = 0; i<len; ++i) std::cout<<i<<" ";
	std::cout<<"\n";
}
void string_modifier()
{
	string s = "hello world !!\n";
	printChar(s);

	s.erase(6,7);
	std::cout<<"s.erase() = "<<s<<"\n";

	s.insert(6,"there");
	std::cout<<"s.insert() = "<<s<<"\n";
}

int main()
{

	//string_ctor();
	//string_elementAccess();
	
	string_modifier();

	return 0;
}

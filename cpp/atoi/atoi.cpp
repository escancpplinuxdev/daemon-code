

#include <iostream>
#include <string.h>
using namespace std;

int main()
{
	const char* s = "42" ;
	char buffer[100]="NULL";
	string b="str";
	unsigned int a=0;
	cout<<"Ascii to interger is : "<<atoi(s)<<endl;

	a=atoi(s);
	cout<<"int operation a+2 = '"<<a+2<<"'\n";


	sprintf(buffer,"this is char *buffer :'%s'\n",s);
	cout<<buffer;


//	sprintf(buffer,"this is int a =  %s\n",to_string(a)); //warning: format ‘%s’ expects argument of type ‘char*’, but argument 3 has type ‘std::string’ {aka ‘std::__cxx11::basic_string<char>’} [-Wformat=


//	sprintf(buffer,"this is int a =  %s\n",a.c_str());	 // error: request for member ‘c_str’ in ‘a’, which is of non-class type ‘unsigned int'
	
	sprintf(buffer,"this is int a = '%s'\n",to_string(a).c_str());	
	cout<<buffer;


//	sprintf(b,"For string b this is int a =  %s\n",to_string(a)); //error: cannot convert ‘std::string’ {aka ‘std::__cxx11::basic_string<char>’} to ‘char*’
	b = b+ "For string b this is int a = '"+ to_string(a)+"'";
	cout<<b<<endl;
	return 0;
}

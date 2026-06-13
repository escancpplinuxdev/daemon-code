
#include <iostream>
#include <cstring>
using namespace std;


int main()
{
	char arr[20]="1234567891011121314";
	cout<<"strlen(arr) = "<<strlen(arr)<<endl;

	char *p = arr;
	cout<<"sizeof(arr) = "<<sizeof(arr)<<endl;
	cout<<"sizeof(p)   = "<<sizeof(p)<<endl;

	string s = "1234567891011121314";
	cout<<"sizeof(s)   = "<<sizeof(s)<<endl;
	cout<<"s.size()    = "<<s.size()<<endl;
	cout<<"s.length()  = "<<s.length()<<endl;

	for(auto c: s)
	{
		cout<<c;
	}
	cout<<endl;
	return 0;
}

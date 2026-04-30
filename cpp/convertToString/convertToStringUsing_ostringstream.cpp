
#include <iostream>
#include <sstream> // ostringstream oss; ->   error: aggregate ‘std::ostringstream oss’ has incomplete type and cannot be defined
using namespace std;


int main()
{

	long double amount;
	cout<<"amount -> "<<amount<<endl;
	cin>>amount;

	ostringstream oss;

	oss<<amount;

	string input = oss.str();
	
	cout<<"amount -> "<<amount<<endl;

	cout<<"input -> "<<input<<endl;


	return 0;
}

#include <iostream>

using namespace std;
int main()
{
//	int n; 	pass ->  case 1:
//	char n; pass -> case '1':
//	string n;
//	char* n;
//	bool n; pass -> case 0:	
	cin>>n;
	switch(n)
	{
		case 0: cout<<"0\n";
			break;

		case 1: cout<<"1"<<endl;
			break;

		case 2: cout<<"2"<<endl;
			break;

		case 3: cout<<"3"<<endl;
			break;

		case 4: cout<<"4"<<endl;
			break;
			
		default: cout<<"NOT NUMBER\n";
			break;
	}
	return 0;
}


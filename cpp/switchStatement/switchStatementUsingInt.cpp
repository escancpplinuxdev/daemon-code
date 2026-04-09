// control flow structure
#include <iostream>

using namespace std;

int main()
{
	int n;
	cin>>n;
	switch(n)
	{
		case 0: cout<<"ZERO\n"; break;
		case 1: cout<<"ONE\n"; break;
		case 2: cout<<"TWO\n"; break;
		case 3: cout<<"THREE\n"; break;
		case 4: cout<<"FOUR\n"; break;
		case 5: cout<<"FIVE\n"; break;

		default:cout<<"OUT OF RANGE\n"; break;
	}
}

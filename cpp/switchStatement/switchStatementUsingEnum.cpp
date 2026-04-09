
#include <iostream>

using namespace std;

enum number {ZERO, ONE, TWO, THREE, FOUR};

int main()
{
	int n;
	cin>>n;

	switch(n)
	{
		case ZERO: cout << "Zero\n"; break;
		case ONE: cout << "One\n"; break;
		case TWO: cout << "Two\n"; break;
		case THREE: cout << "Three\n"; break;
		case FOUR: cout << "Four\n"; break;
		default: cout << "Invalid\n";
	}

	return 0;
}



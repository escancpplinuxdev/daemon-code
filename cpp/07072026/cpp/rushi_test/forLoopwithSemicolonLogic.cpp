#include <iostream>
using namespace std;

int main() 
{
	int i;
	for (i = 0; i < 10; i++); //semicolon after the loop treat as the body of the loop, so each iteration does nothing
	{  // for i == 10 for loop condition failed , control come outside
		cout << i; // then this block of code print.
	}
	return 0;
}

#include <iostream>
using namespace std;

int main() 
{
	int x = -1;			// x = -1
	try 
	{
		cout << "Try ";		//  Try
		if (x < 0)   		//  -1 < 0 condition TRUE
		{
			throw x;	// throw -1 as exception // so immediately control goes to catch block
			cout << "Throw "; //– this line is NEVER executed because when throw is encountered, control immediately jumps to the nearest matching catch block, skipping the rest of the try block.
		}
	}
	catch (int x) // catch (-1) //catch thrown integer -1 
	{
		cout << "Caught ";	// Caught
	}

	cout << "Exit";			//Exit
	return 0;
}

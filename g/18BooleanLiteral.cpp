
#include <iostream>

int main()
{
	bool t = true, f = false;

	bool  T = TRUE, F = FALSE; // error: ‘TRUE’ was not declared in this scope

	bool r = True, e = False;  // error: ‘True’ was not declared in this scope

	Bool R = true, E = false;	// error: ‘Bool’ was not declared in this scope; did you mean ‘bool’?

	
	return 0;
}

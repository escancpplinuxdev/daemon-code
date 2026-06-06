
#include <iostream>
#include <cstring>
using namespace std;

int main()
{

	//1. strlen - get string length
	
	const char * msg = "Hello, World!";
	size_t len = strlen(msg);
	cout<<"1. strlen(\""<<msg<<"\") = " << len << "\n";

	//2. strcpy - copy (no bound check)
	
	char dest[10];
	strcpy(dest,msg);
	cout<<"2. strlen(dest,msg) -> dest = \""<<dest<<"\"\n";

	//3. strncpy - copy at most n chars, manually null-terminate
	char dest2[10];
	strncpy(dest2,msg,sizeof(dest2)-1);
	dest2[sizeof(dest2) - 1] = '\0';
	cout<<"3. strncpy(dest2,msg,9) -> dest2 = \""<<dest2<<"\"\n";

	//4. strcat - concatenate (no bounds check)
	
	char dest3[50] = "Hello";
	strcat(dest3," World!");
	cout<<"4. strcat(dest3, \" World!\") -> dest3 = \""<<dest3<<"\"\n";

	//5. strncat - concatenate at most n chars (always null-terminates)
	char dest4[20] = "Hello";
	strncat(dest4,"WonderFul World !", sizeof(dest4) - strlen(dest4) -1  );
	cout << "5. strncat(dest4, \" Wonderful World!\", ...) -> dest4 = \"" << dest4 << "\"\n";
	return 0;


}

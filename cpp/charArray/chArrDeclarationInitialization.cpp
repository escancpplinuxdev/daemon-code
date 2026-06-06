
#include <iostream>
#include <cstring> // strlen() c-style string
using namespace std;

void printchArray(const char * buff )
{
	int n = strlen(buff);
	cout<<"n = "<<n<<endl;
	for(int i =0; i< n; i++)
	{
		cout<<buff[i]<<" ";
	}
	cout<<endl;
	cout<<endl;

}

int main()
{
	char buff[10];
	printchArray(buff);

	char buff1[10] = {0};
	printchArray(buff1);

	char buff2[]="2hello";
	printchArray(buff2);

	char buff3[10]="3hello";
	printchArray(buff3);

//	char buff4[5]="hello"; error: initializer-string for ‘char [5]’ is too long [-fpermissive] // error: no space for '\0' 
	char buff4[6]="hello";
	printchArray(buff4);

	char* p = new char[100];
	strcpy(p,"5hello");
	printchArray(p);

	delete[] p;

	printchArray(p);

	printf("no null\n");
	char no_null[3] = {'a','b','c'};
	printchArray(no_null);
//	char no_null = {'a','b','c','\0'};
	
	puts("with null");
	char with_null[4]={'a','b','c','\0'};
	printchArray(with_null);
	return 0;
}

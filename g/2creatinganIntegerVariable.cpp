// creating an integer variable

#include <iostream>

int main()
{	
	// creating an integer variable
	int a = 10;
//	cout<<" a = "<<a<<"\n"; // error: ‘cout’ was not declared in this scope; did you mean ‘std::cout’?
	std::cout<<" a = "<<a<<"\n";

	// using hexadecimal base value
//	int h = 0x1A;
	int h = 19;	
	std::cout<<" h = "<<h<<"\n";
	
		return 0;
}

//[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ creatinganIntegerVariable.cpp -o creatinganIntegerVariable
//[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./creatinganIntegerVariable 
// a = 10
// h = 19


//useCharacterVariable.cpp
#include <iostream>

int main()
{
//	char a = "A"; 	 // error: invalid conversion from ‘const char*’ to ‘char’
	char a = 'A'	;
	std::cout<<"a = "<<a<<"\n";
	
	
//	cout<<"a = "<<a<<"\n"; // error: ‘cout’ was not declared in this scope; did you mean ‘std::cout’?

	return 0;	

}

//[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# make useCharacterVariable  // -> make outputFileName
//g++     useCharacterVariable.cpp   -o useCharacterVariable
//
/**
 *	ASCII Character 
 *	A -> 65		a -> 97 
 *	Z -> 90		Z -> 122
 *	65 - 90		97 - 122
 * */

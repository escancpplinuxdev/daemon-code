//6hexadecimalLiteralPerfix0x.cpp
//the hexadecimal literal represent an integer in base 16 and is preceded by either 0x and 0X prefix.
#include <iostream>

int main()
{
//	int hex = 0xAB;
	int hex = 0Xab;
//	int hex = 0xaz;	//	warning: use of C++23 ‘make_signed_t<size_t>’ integer constant
	std::cout<<" hex = "<<hex<<"\n";

	return 0;
}
/*
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 6hexadecimalLiteralPerfix0x.cpp -o 6hexadecimalLiteralPerfix0x
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./6hexadecimalLiteralPerfix0x 
 hex = 171

 * */

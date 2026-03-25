//5octalLiteralPerfix0.cpp
//the octal literal represents an integer in base 8 and preceded by 0 prefix.
#include <iostream>

int main()
{
//	int oct= 089; 	//	error: invalid digit "9" in octal constant        
//	int oct= 085;	//  	error: invalid digit "8" in octal constant
	int oct= 045;

	std::cout<<" oct = "<<oct<<"\n";

	return 0;
}
/**
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 5octalLiteralPerfix0.cpp -o 5octalLiteralPerfix0
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./5octalLiteralPerfix0 
 oct = 37 // integer has in base 8
*/

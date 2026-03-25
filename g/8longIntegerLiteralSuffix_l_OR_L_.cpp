//8longIntegerLiteralSuffix_l_OR_L_.cpp
//the long integer literal can be suffix with l or L to indicate that it is of long int.
#include <iostream>

int main()
{
//	long int a = l87;	//	error: ‘l87’ was not declared in this scope
//	long int a = 87l;
//	long int a = 76L;	
	long int a = 110l;
	std::cout<<" long in a "<<a<<"\n";

	return 0;
}
/*
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 8longIntegerLiteralSuffix_l_OR_L_.cpp -o 8longIntegerLiteralSuffix_l_OR_L_
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./8longIntegerLiteralSuffix_l_OR_L_ 
 long in a 76
*/

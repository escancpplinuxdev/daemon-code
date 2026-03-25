// 7binaryLiteralPrefix0b_OB.cpp
// binary literal represent an integer in the base 2 and can have a suffix 0b or 0B 
#include <iostream>

int main()
{
//	int b = 0b18;	//	error: invalid digit "8" in binary constant
//	int b = 0b19;	//	error: invalid digit "9" in binary constant
//	int b = 0 b101;	//	error: expected ‘,’ or ‘;’ before ‘b101’
//	int b = 0	B101;	//	 error: expected ‘,’ or ‘;’ before ‘B101’
	int b = 0B101;	
	std::cout<<" b = "<<b<<"\n";

	return 0;
}

/*
 *
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 7binaryLiteralPrefix0b_OB.cpp -o 7binaryLiteralPrefix0b_OB
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./7binaryLiteralPrefix0b_OB 
 b = 5
*/

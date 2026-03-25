//9unsignedIntegerLiteralSuffix_u_OR_U_.cpp
//note: ‘std::cout’ is defined in header ‘<iostream>’; did you forget to ‘#include <iostream>’?
//  +++ |+#include <iostream>

#include <iostream>


int main()
{
//	signed int a = 42U;
	signed int a = 77u;
	std::cout<<" a = "<<a<<"\n";

	return 0;
}
/*
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 9unsignedIntegerLiteralSuffix_u_OR_U_.cpp -o 9unsignedIntegerLiteralSuffix_u_OR_U_
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./9unsignedIntegerLiteralSuffix_u_OR_U_ 
 a = 42
 */

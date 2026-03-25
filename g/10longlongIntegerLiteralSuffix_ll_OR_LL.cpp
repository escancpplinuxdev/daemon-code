//10longlongIntegerLiteralSuffix_ll_OR_LL.cpp
// long long integer literal can have suffix ll or LL to idicated that it is of type long int .
#include <iostream>

int main()
{	
	int a = 7ll;
	
	std::cout<<" a = "<<a<<" \n";
	return 0;
}
/*//error: unterminated comment
//   12 | /*  * /
//      | ^

 *
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 10longlongIntegerLiteralSuffix_ll_OR_LL.cpp -o 10longlongIntegerLiteralSuffix_ll_OR_LL
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./10longlongIntegerLiteralSuffix_ll_OR_LL 
 a = 7 
*
* / //	error: unterminated comment
//   12 | /*
//      | ^
*/

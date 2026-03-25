// 11ntergerLiteral.cpp
// integer Literal.
#include <iostream>

int main()
{
	int d = 10;
	std::cout<<" d = "<<d<<"\n";
	int o = 07;
	std::cout<<" o = "<<o<<"\n";
	int h = 0xAA, H = 0x10 ;
	std::cout<<" h = "<<h<<"\t"<<" H = "<<h<<"\n";
	int b = 0b100;
	std::cout<<" b = "<<b<<"\n";
	long int l = 10l, L = 20L;
	std::cout<<" l = "<<l<<"\t"<<" L = "<<L<<"\n";
	long long int ll = 99ll, LL = 69LL;
	std::cout<<" ll = "<<ll<<"\t"<<" LL = "<<LL<<"\n";
	unsigned int u = 77u, U = 8U;
	std::cout<<" u = "<<u<<"\t"<<" U = "<<U<<"\n";

}
//[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ 11ntergerLiteral.cpp -o 11ntergerLiteral

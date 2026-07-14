
#include <iostream>
#pragma pack(1)
struct 
{
	char 	a; //1 +3 	-> 4
	int 	b; //4 		-> 4
	double 	c; //8		-> 8
//total				  16			
	void print()
	{
		std::cout<<"struct A\n";
	}
}A;

int main()
{	
	A.print();
	std::cout<<"sizeof(A) = '"<<sizeof(A)<<"'\n";
	
	return 0;
}



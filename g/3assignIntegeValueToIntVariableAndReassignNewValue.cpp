//3assignIntegeValueToIntVariableAndReassignNewValue.cpp
//
//In C++ programming language, literals are fundamental elements used to represent fixed values. These values can include  numbers, characters, strings, and more. They are generally present as the right operand in the assignment operation.
#include <iostream>

int main()
{
	// Assign a integer value to assign variable
	int val = 20;
	std::cout<<" val = "<<val<<"\n";
	
	// Assigning new value
	val = 6;
	std::cout<<" val = "<<val<<"\n";

	return 0;
}

/**
 *
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# g++ --std=c++03 3assignIntegeValueToIntVariableAndReassignNewValue.cpp -o 3assignIntegeValueToIntVariableAndReassignNewValue
[root@PTLD-3:/tmpdata/Rushikesh_27022026/g]# ./3assignIntegeValueToIntVariableAndReassignNewValue 
 val = 20
 val = 6

 * /


#include <iostream>

void pointerIntro()
{
	int *p1 = nullptr;
	int *p2 = NULL;
	int *p3 = 0;

	std::cout<<"p1 = "<<p1<<"\n";	//p1 = 0

	if (p1 == NULL) std::cout<<"NULL\n";	//NULL

	if (p2 == NULL) std::cout<<"NULL\n";	//NULL
	
//	if (p2 != 1 ) std::cout<<"!= 1\n"; //error: ISO C++ forbids comparison between pointer and integer
/*
error: ISO C++ forbids comparison between pointer and integer [-fpermissive]
   16 |         if (p2 != 1 ) std::cout<<"!= 1\n";
      |             ~~~^~~~

*/
	if (p2 != (int*)1 ) std::cout<<"!= 1\n";  //!= 1
	else 		std::cout<<"== 1\n";

	if (p3 == NULL) std::cout<<"NULL\n";	//NULL

	if (p3 == nullptr) std::cout<<"nullptr\n";	//NULL

	int a = 42;
	int *p = &a;
	std::cout<<"*p = "<<*p<<"\n";	//*p = 42
	std::cout<<"p  = "<<p<<"\n";	//p  = 0x7ffe7454f21

	int b = 72;
//	int *q = b; //error: invalid conversion from ‘int’ to ‘int*’ //int *q = b;
/*
error: invalid conversion from ‘int’ to ‘int*’ [-fpermissive]
   27 |         int *q = b;
      |                  ^
      |                  |
      |                  int
*/
	//int *q = (int *)b; //Segmentation fault (core dumped)
	//std::cout<<"*q = "<<*q<<"\n";
	//std::cout<<"q  = "<<q<<"\n";

	return ;
}

void printArray(int *a)
{
		
	std::cout<<"sizeof(a) = "<<sizeof(a)<<"\n";

	std::cout<<"a[2]      = " << *(a + 2) << std::endl;   // 3 3

}

void arrayDecay()
{
	int a[10] = {1,2,3,4,5,6,7,8,9,0};

	std::cout<<"sizeof(a) = "<<sizeof(a)<<"\n";
	
	printArray(a);
}


void printValue(int x) 
{
    std::cout << "Integer function called: " << x << std::endl;
}

void printValue(int* ptr) 
{
    std::cout << "Pointer function called." << std::endl;
//    std::cout << "* ptr = "<<*ptr<<"\n"; // Segmentation fault (core dumped) // Can't do derefercene in nullptr
    std::cout << "printValue(nullptr); ptr = "<<ptr<<"\n";
}

void nullptrAndNULLDiff() 
{
    // Problem: NULL is just 0. The compiler picks the integer overload!
//    printValue(NULL);    // Output: Integer function called: 0
/*
error: call of overloaded ‘printValue(NULL)’ is ambiguous
   81 |     printValue(NULL);    // Output: Integer function called: 0
*/

    
    // Solution: nullptr is strictly a pointer type.
    printValue(nullptr); // Output: Pointer function called.

int number = NULL; // Compiles! Sets the integer to 0.
std::cout<<"int number = NULL;  number = "<<number<<"\n";
/*
warning: converting to non-pointer type ‘int’ from NULL [-Wconversion-null]
   95 | int number = NULL; // Compiles! Sets the integer to 0.
*/
bool flag = NULL;   // Compiles! Sets the boolean to false.
std::cout<<"bool flag = NULL;  flag = "<<flag<<"\n";

//int number = nullptr; // Compile-time ERROR! Cannot convert pointer to int.
/*
error: redeclaration of ‘int number’
   98 | int number = nullptr; // Compile-time ERROR! Cannot convert pointer to int.
      |     ^~~~~~
*/

}



int main()
{
	//pointerIntro();

	//arrayDecay();
	
	nullptrAndNULLDiff();

	return 0;	
}

/*


*/

#include <iostream>
using namespace std;

struct A 
{
	char c;
	int i;
};

struct B 
{
	int i;
	char c;
};

struct C 
{
	char c1;
	char c2;
	int i;
};
//1. The "Dangling Pointer" Warning (-Wreturn-local-addr)
int * printC()
{
	int *a=new int(9);
	cout<<"value of a="<<a<<endl;
	return a;
	// int a=9;	return &a; 	// warning: address of local variable ‘a’ returned [-Wreturn-local-addr]

}

float * printD()
{  
	float *a=new float(3.14);
	cout<<"value of a="<<a<<endl;
	return a;

	//float a=3.14;      return &a;	// warning: address of local variable ‘a’ returned [-Wreturn-local-addr]

}

double * printE()
{  
	double *a=new double(9);
	cout<<"value of a="<<a<<endl;
	return a;
	//double a=9;	return &a;	warning: address of local variable ‘a’ returned [-Wreturn-local-addr]
}

char * printF()
{  
	char* a =new char ('A');
	cout<<"value of a="<<a<<endl;
	return a;
	//char a='A';  return &a;	warning: address of local variable ‘a’ returned [-Wreturn-local-addr]

}



void * printB() // sizeof(void *) = 8 byte
{
	cout<<"size of void* = ";
	return NULL; //Because it is a void pointer, the function must return an address (a memory location) or NULL.
}
void  printA() //sizeof(void) 	 = 1 byte 
{
	cout<<"size of void = ";
	return ;
	// return void means nothing,"When function is void is return no value."
}
int main() 
{
	int a,b=5,c=0,d=-1;
	cout <<"size 0f	int a (declaration of 'int a' not assign) = "<<a<<"	              	= "<< sizeof(a) << endl;
	cout <<"size 0f	int b = "<<b<<"	    	      					= "<< sizeof(b) << endl;
	cout <<"size 0f	int c = "<<c<<"							= "<< sizeof(c) << endl;
	cout <<"size 0f	int d = "<<d<<"     		     	    				= "<< sizeof(d) << endl;
	cout<<endl;
	
//	char h='+A'; // warning: multi-character character constant [-Wmultichar]
	char e,f='C',g='-',h='+';
	cout <<"size 0f	char e (declaration of 'char e' not assign) = "<<e<<"	              	= "<< sizeof(e) << endl;
	cout <<"size 0f	char f = "<<f<<"	    	      					= "<< sizeof(f) << endl;
	cout <<"size 0f	char g = "<<g<<"							= "<< sizeof(g) << endl;
	cout <<"size 0f	char h = "<<h<<"     		     	    				= "<< sizeof(h) << endl;
	cout<<endl;

	float j,k=3.14,l=-3.14,m=+3.14;
	cout <<"size 0f	float j (declaration of 'float j' not assign) = "<<j<<"	              	= "<< sizeof(j) << endl;
	cout <<"size 0f	float k = "<<k<<"	    	      					= "<< sizeof(k) << endl;
	cout <<"size 0f	float l = "<<l<<"							= "<< sizeof(l) << endl;
	cout <<"size 0f	float m = "<<m<<"     		     	    				= "<< sizeof(m) << endl;
	cout<<endl;

	double n,o=3.14,p=-3.14,q=+3.14;
	cout <<"size 0f	double j (declaration of 'double n' not assign) = "<<n<<"	              	= "<< sizeof(n) << endl;
	cout <<"size 0f	double o = "<<o<<"	    	      					= "<< sizeof(o) << endl;
	cout <<"size 0f	double p = "<<p<<"							= "<< sizeof(p) << endl;
	cout <<"size 0f	double q = "<<q<<"     		     	    				= "<< sizeof(q) << endl;
	cout<<endl;
	// The sizeof(void) Warning (-Wpointer-arith)
	cout <<"size 0f	struct A		      = "<< sizeof(A) << endl;
	cout <<"size 0f struct B                      = "<< sizeof(B) << endl;
	cout <<"size 0f struct C                      = "<< sizeof(C) << endl;
	cout<<endl;


	// ILLEGAL: These will always trigger warnings/errors
//	cout <<"sizeof(void)                          = "<< sizeof(void)<<endl;     // ERROR: illegal //sizeof1.cpp:82:45: warning: invalid application of ‘sizeof’ to a void type [-Wpointer-arith]

//	cout <<"sizeof(printA())                      = "<<sizeof(printA())<<endl;  // ERROR: illegal (returns void)


	// LEGAL: Measuring the address (pointer)
	cout <<"Size of a pointer to a void function  = " << sizeof(&printA)  << endl; // give size of void
	cout <<"Size of a universal pointer (void*)   = " << sizeof(void*)  << endl;   // give size of void *
	cout<<endl;


	cout <<"size 0f void*   printB()              = "<< sizeof(printB())<<endl;
	cout <<"size 0f char*   printF()              = "<< sizeof(printF())<<endl;
	cout <<"size 0f int*    printC()              = "<< sizeof(printC())<<endl;
	cout <<"size 0f float*  printD()              = "<< sizeof(printD())<<endl;
	cout <<"size 0f double* printE()              = "<< sizeof(printE())<<endl;
	cout<<endl;
	return 0;
}

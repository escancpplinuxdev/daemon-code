
#include <iostream>

void printV(void *data);
void add(int &a,int &b,int &c);
void swap(int &a,int &b,int &c);
int main()
{
	printV(NULL);
	int a = 10, b= 7, c= 8;
	add(a,b,c);
	std::cout<<"a = '"<<a<<"' b = '"<<b<<"' c = '"<<c<<"'\n";
	swap(a,b,c);
	std::cout<<"a = '"<<a<<"' b = '"<<b<<"' c = '"<<c<<"'\n";
	std::swap(a,b);
	std::cout<<"a = '"<<a<<"' b = '"<<b<<"' c = '"<<c<<"'\n";
	return 0;
}

void printV(void *data)
{
	std::cout<<"print func\n";
	std::cout<<"*data = '"<<&data<<"'\t data = '"<<data<<"'\n";
}


void add(int &a,int &b,int &c)
{
	a += 10;
	b += 10;
	c += 10;
}

void swap(int &a,int &b,int &c)
{
	a += b;  
	b =a - b;  
	a -= b;
}

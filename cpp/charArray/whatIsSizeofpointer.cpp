
//array parameters decay to pointers i.e array size , dimension 
//So sizeof(s) returns the size of a pointer (8 bytes on x86_64), not the array size.
// g++ -w whatIsSizeofpointer.cpp -o whatIsSizeofpointer  -> here -w for Suppress the warning , but this only hides warning ; it does not fix underlying issue.
#include <iostream>
using namespace std;

void func(char s[])
//void func(char *s)
{
	cout<<"func sizeof(s) = "<<sizeof(s)<<endl;

	return;
}

void fun(int arr[])
//void fun(int *arr)
{
	cout<<"fun sizeof(arr) = "<<sizeof(arr)<<endl;

	return;
}

int main()
{

	char s[]="hello";
	char *p=s;
	int a =10;
	int *i=&a;
	func(s);
	cout<<"sizeof(s) = "<<sizeof(s)<<endl;
	cout<<"sizeof(p) = "<<sizeof(p)<<endl;
	cout<<"sizeof(*p) = "<<sizeof(*p)<<endl;
	cout<<"sizeof(i) = "<<sizeof(i)<<endl;
	cout<<"sizeof(*i) = "<<sizeof(*i)<<endl;
	cout<<"sizeof(a) = "<<sizeof(a)<<endl;
	func((char*)a);
//	printf("sizeof(s) = '%d'\nsizeof(p) = '%d'\n",sizeof(s),sizeof(p));
	printf("sizeof(s) = '%zu'\nsizeof(p) = '%zu'\n",sizeof(s),sizeof(p));

	int arr[10];
	int *ii=arr;
	cout<<"sizeof(arr) = "<<sizeof(arr)<<endl;
	cout<<"sizeof(ii) = "<<sizeof(ii)<<endl;
	cout<<"sizeof(*ii) = "<<sizeof(*ii)<<endl;

	fun(arr);

	return 0;
}

// i.e sizeof(pointer) = 8 byte
// So sizeof(s) returns the size of a pointer (8 bytes on x86_64), not the array size.

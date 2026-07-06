
#include <iostream>

using namespace std;

void printArray(int a[], int n)
{
	for(int i=0;i<n;i++) cout<<a[i]<<" ";
}
void swap(int &a,int &b)
{
	if(a>b)
	{
	/*	int temp= a;
		a=b;
		b=temp;
		*/
	// a=2, b=5
		a=a+b; // 2+5=7
		b=a-b; // 7-5=2 -> b
		a=a-b; // 7-2=5 -> a
	}
}
int main()
{
	int a[]={8,5,3,1,7,6,2,4};
	int n=sizeof(a)/sizeof(a[0]);

	cout<<"UnSorted array : ";
	printArray(a,n);
	cout<<endl;

	for(int i =0;i<n-1;i++)
	{
		for(int j=0;j<n-i-1;j++)
		{
			swap(a[j],a[j+1]);
		}
	}
	cout<<"Sorted array : ";
	printArray(a,n);
	cout<<endl;
	return 0;
}

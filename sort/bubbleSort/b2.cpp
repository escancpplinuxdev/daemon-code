
#include <iostream>

using namespace std;
void printArray(int a[],int n)
{
	for(int i=0;i<n;i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<"\n";
}

int main()
{
	int a[]={8,5,3,1,7,6,2,4};
	int n=sizeof(a)/sizeof(a[0]);

	cout<<"Unsorted array : ";
	printArray(a,n);

	for(int i=0;i<n-1;i++)
	{
		for(int j=0;j<n-1-i;j++)
		{
			if(a[j]>a[j+1])
			{
				int temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	cout<<"Sorted Array   : ";
	printArray(a,n);

	return 0;
}

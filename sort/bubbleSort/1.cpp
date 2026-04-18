
#include <iostream>

using namespace std;

void optimiziedRecursiveBubbleSort(int *a,int n)
{
	if(n == 1) return ; // return when n = 1 //	bestcase
	bool swap = false;
	for(int i = 0; i<n-1; i++)
	{
		if (a[i]>a[i+1])
		{
		int temp = a[i];
		a[i] = a[i+1];
		a[i+1] = temp ;
		swap = true;

		}
	}	

	if(!swap) return ; // if swap doesn not happen, that's  means array is sorted.

	optimiziedRecursiveBubbleSort(a,n);

}

void printArray(int a[],int n)
{
//	for(int x: a)		cout<<x<<" ";
	for(int i=0;i<n;i++)	
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
}



int main()
{
	int a[]={8,5,3,1,7,6,2,4,3,5};
	int n = sizeof(a)/sizeof(a[n]);
	
	cout<<"Unsorted array : ";
	printArray(a,n);

	optimiziedRecursiveBubbleSort(a,n);

	cout<<"Sorted array   : ";
	printArray(a,n);
	
	return 0;

}




















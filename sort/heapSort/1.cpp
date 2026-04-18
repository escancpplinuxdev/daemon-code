
#include <iostream>

using namespace std;

void heapify(int a[], int n, int i)
{
	int	 root = i;
	int 	left = 2*i+1;
	int	right= 2*i+2;

	if((left < n) && (a[left] > a[root]))
	{
		root=left;
	}

	if((right < n) && (a[right] > a[root]))
	{
		root=right;
	}

	if(root!=i)
	{
		swap(a[root],a[i]);
		heapify(a,n,root);
	}

}



void heapSort(int a[],int n)
{	
	// build binary heap max tree
	for (int i=n/2-1;i>=0;i--)   	// n=8 i.e i = 3 , 2, 1, 0
		heapify(a,n,i);

	for(int i=n-1;i>0;i--)	
	{
		swap(a[i],a[0]);

		heapify(a,i,0);
	}
}

void printArray(int a[],int n)
{
	//      for(int x: a)           cout<<x<<" ";
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

	heapSort(a,n);

	cout<<"Sorted array   : ";
	printArray(a,n);

	return 0;
}

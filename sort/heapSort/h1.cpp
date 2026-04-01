
#include <iostream>

using namespace std;

void swap(int &a, int &b)
{
	int t= a;
	a=b;
	b= t;
}
void heapify(int a[], int n, int i)
{
	int root =i;
	int left = 2*i+1;
	int right = 2*i+2;

	if (left < n && a[left] > a[root]) root=left;

	if (right < n && a[right] > a[root]) root = right;

	if(root != i)
	{
		swap(a[root],a[i]);

		heapify(a,n,root);
	}

}
void heapsort(int a [], int n)
{
	//build max heap
	for (int i = n/2-1; i>=0;i--)
	{
		heapify(a,n,i);
	}

	//sorting loop
	for(int i =n-1;i>=0;i--)
	{
		swap(a[i],a[0]);

		heapify(a,i,0);
	}
}
int main()
{
	int a[]= {8,5,3,1,7,6,2,4};
	int n = sizeof(a)/sizeof(a[0]);
	
	for (int i =0;i<n;i++)
	{
	cout<<a[i]<<" ";
	}
	cout<<endl;
	heapsort(a,n);

	
	for (int i =0;i<n;i++)
	{
	cout<<a[i]<<" ";
	}
	cout<<endl;
	return 0;
}

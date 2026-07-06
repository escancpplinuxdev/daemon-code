
#include <iostream>

using namespace std;

void printArray(int a[], int n)
{
	for(int i=0;i<n;i++)
		cout<<a[i]<<" ";
	cout<<endl;
}
int main()
{
	int a[]={8,5,3,4,1,2,7,6};
	int n=sizeof(a)/sizeof(a[0]);

	cout<<"Unsorted Array           :  ";
	printArray(a,n);

	for(int i =0 ;i<n-1;i++)
	{
		bool swapped=false;
		for(int j=0;j<n-1-i;j++)
		{
			if(a[j]>a[j+1])
			{
				int temp = a[j];
				a[j] = a[j+1];
				a[j+1]=temp;
				swapped=true;
			}
		}
		if(!swapped)
		{
			cout<<"Array is get sorted at iteraton i = "<<i<<endl;
			break;
		}
			cout<<"Array at  iteration  "<<i<<"]  :  ";
			printArray(a,n);
	}
	cout<<"Sorted Array             :  ";
	printArray(a,n);

}

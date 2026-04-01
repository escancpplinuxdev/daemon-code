
#include <iostream>

using namespace std;
void printArray(int a[],int n)
{
	for(int i =0;i<n;i++)
	{
		cout<<a[i]<<" ";
	}
}
int main()
{
	int a[]={8,5,3,1,7,6,2,4};
	int n = sizeof(a)/sizeof(a[0]);
	cout<<"Size of array n = "<<n<<endl;
	cout<<"Unsorted array = ";
	printArray(a,n);
	for (int i=0;i<n-1;i++)
	{
		for(int j=0;j<n-i-1;j++)
		{
			if(a[j]>a[j+1])
			{
				int temp= a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	cout<<"\nsorted array = ";
	printArray(a,n);

	cout<<endl;
	return 0;
}

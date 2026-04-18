
#include <iostream>
using namespace std;

void insertionSort(int a[], int n)
{
	for(int i=1;i<n;i++)
	{
		int key = a[i];
		int j= i-1;
		for( ;j>=0 && a[j]>key ; j--)
		{
			a[j+1] = a[j];
		}
		a[j+1]=key;
	}
}

void printArray(int a[], int n)
{
        for(int i=0;i<n;i++)
        {
                cout<<a[i]<<" ";
        }
        cout<<endl;
}

int main()
{

        int a[] = {5,8,6,3,9,1,7,2,4,0};

        int n = sizeof(a)/sizeof(a[0]);

        cout<<"UnSorted array : ";
        printArray(a,n);

        insertionSort(a,n);

        cout<<"Sorted array   : ";
        printArray(a,n);



        return 0;

}



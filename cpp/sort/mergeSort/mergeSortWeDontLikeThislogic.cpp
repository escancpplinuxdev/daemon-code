
#include <iostream>

using namespace std;

void merge(int a[], int low, int mid, int high)
{
	int size = high -low +1;
	int * temp = new int[size];

	int left = low;
	int right= mid+1;
	int k = 0;

	while ( left <= mid && right <= high)
	{
		if(a[left] <= a[right])
		{
			temp[k++] = a[left++];
		}
		else
		{
			temp[k++] = a[right++];
		}
	}
	
	while(left<=mid)
	{
		temp[k++] = a[left++];
	}

	while(right <= high)
	{
		temp[k++] = a[right++];
	}

	for(int i= 0; i< size; i++)
	{
		a[low + i] = temp[i];

	}
	delete[] temp;
}

void mergeSort(int a[],int low,int high)
{
	if( low >= high ) return; // Best case

	int mid = low + (high - low)/2; // pervents overflow

	mergeSort(a, low, mid);
	mergeSort(a, mid+1, high);
	merge(a, low, mid, high);

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

        int a[] = {5,8,6,4,3,9,1,7,5,3,4};

        int n = sizeof(a)/sizeof(a[0]);

        cout<<"UnSorted array : ";
        printArray(a,n);

        mergeSort(a,0,n-1);

        cout<<"Sorted array   : ";
        printArray(a,n);

	return 0;
}

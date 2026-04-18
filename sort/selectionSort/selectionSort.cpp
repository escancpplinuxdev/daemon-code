
#include <iostream>

using namespace std;

void selectionSort(int a[],int n)
{	
	for(int i =0;i<n-1;i++)
	{
	int min_indx=i;
	

		for(int j=i+1;j<n;j++)
		{
			if(a[min_indx]>a[j])
			{
				min_indx=j;
			}
		}

		if(min_indx!=i)
		{
			int temp=a[i];
			a[i] = a[min_indx];
			a[min_indx]=temp;
		}
	}
	return;

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

        selectionSort(a,n);

        cout<<"Sorted array   : ";
        printArray(a,n);

	return 0;
}

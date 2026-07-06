
#include <iostream>

using namespace std;

void recursiveBubbleSort(int* a, int n) 
{
	// 1] Base Case
	if (n == 1) return;

	bool swapped = false; // Flag to track if a swap happens

	// 2] One Pass
	for (int j = 0; j < n - 1; j++) 
	{
		if (a[j] > a[j + 1]) 
		{
			//			swap(a[j], a[j + 1]);
			if(a[j]>a[j+1])			
			{
				int temp = a[j];
				a[j]=a[j+1];
				a[j+1]= temp;


				swapped = true; // A swap occurred
			}
		}
	}

	// 3] Optimization: If no two elements were swapped by inner loop, then break
	if (!swapped) return;

	// 4] Recursive call
	recursiveBubbleSort(a, n - 1);
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

	recursiveBubbleSort(a,n);

	cout<<"Sorted array   : ";
	printArray(a,n);



	return 0;
}


#include <iostream>

using namespace std;

void merge(int a[], int left, int mid, int right)
{
	int n1= mid - left +1; // size of left half
	int n2=right - mid;    // size of right half

	// Create temporary arrays
	int L[n1], R[n2];

	// copy data to temporary array
	for(int i =0; i<n1; i++)
			L[i]=a[left + i];
	for(int j=0; j<n2; j++)
			R[j]=a[mid+1+j];

	// Merge the temporary arrays back into a[left .. right] by a[k] array
	int i=0, j=0, k=left;
	while(i<n1 && j<n2)
	{
		if(L[i] <= R[j])
		{
			a[k]= L[i];
			i++;
		}
		else
		{
			a[k]=R[j];
			j++;
		}
		k++;
	}

	// Copy remaining elements of L[] to a[k] if any
	while (i<n1)
	{
		a[k]=L[i];
		i++;
		k++;
	}

	//Copy remaining elements of R[] to a[k] if any
	while (j<n2)
	{
		a[k]=R[j];
		j++;
		k++;
	}
}

void mergeSort(int a[], int left, int right)
{
	if(left < right)
	{
		int mid= left + (right - left) / 2; // avoid overflow 
		// sort first and second halves
		mergeSort(a,left,mid);
		mergeSort(a,mid+1,right);

		// Merge the sorted halves
		merge(a, left, mid, right);
	}
}

void printArray(int a[], int n)
{
	for (int i=0; i<n; i++)
		cout<<a[i]<<" ";
	cout<<endl;
}


int main()
{
	int a[] = {5,8,6,3,9,1,7,2,4,0};
	int n = sizeof(a)/sizeof(a[0]);

	cout<<"Original array : ";
	printArray(a,n);

	mergeSort(a,0,n-1);

	cout<<"Sorted array   : ";
	printArray(a,n);

	return 0;
}

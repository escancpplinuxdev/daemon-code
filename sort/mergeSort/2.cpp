
#include <iostream>

using namespace std;

void merge(int a[], int left, int mid, int right)
{
	int n1= mid-left+1;
	int n2= right-mid;

	int L[n1,L[n2]];

	for(int i=0;i<n1;i++)
			L[i]=a[left+i];
}
void mergeSort(int a[], int left, int right)
{
	if(left<right)
	{
		int mid = left + (right - left) /2;

		mergeSort(a,left,mid);
		mergeSort(a,mid+1,right);

		merge(a,left,mid,right);
	}
}





int main()
{




	return 0;
}

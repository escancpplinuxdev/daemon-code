
#include <iostream>

using namespace std;

void recursiveBubbleSort(int *a,int n)
{
	if (n == 1)	return ; // 1] If array size is 1, then stop					// Best case.
				 
//	2] After this loop, the largest element is at the end.
	for(int j=0;j<n-1;j++) // this is end of recursive loop  					// one pass 
	{
		if(a[j]>a[j+1])
		{
			int temp = a[j];
			a[j]=a[j+1];
			a[j+1]=temp;
		}
	}


	recursiveBubbleSort(a,n-1); // 3] Recurssive call, this replace outer i loop. Sort the remaining n-1 elements.
	// The recursion happens after the entire for loop is done. One full pass is completed, the largest number "bubbles" to the top, and then you tell the function to do it again for a smaller sub-array. 
	
	
						      					// recursive call on smaller array	
}
/*


   void BubbleSort(int *a,int n)
   {
   for(int i=0;i<n-1;i++)
   {
   bool swap = false;
   for(int j=0;j<n-i-1;j++)
   {
   if(a[j]>a[j+1])
   {
   int temp = a[j];
   a[j]=a[j+1];
   a[j+1]=temp;
   swap = true;
   }
   }
   if(!swap)
   {
   break;
   }
   }
   }



*/
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

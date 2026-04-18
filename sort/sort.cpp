
#include <iostream>

using namespace std;

void bubbleSort(int *a, int n)
{
	for(int i=0;i<n-1;i++)
	{
		bool swap =false;
		for(int j=0 ; j<n-i-1;j++)
		{
			if(a[j] > a[j+1])
			{
				int temp = a[j];
				a[j]= a[j+1];
				a[j+1] = temp;
				swap = true;
			}
		}
		if(!swap) break;

	}

}




void recursiveBubbleSort(int *a,int n)
{
	if(n == 1)
		return;
	bool swap = false;
	for(int j =0;j<n-1;j++)
	{
		if(a[j]>a[j+1])
		{
			int temp = a[j];
			a[j] = a[j+1];
			a[j+1] = temp;
			swap = true;

		}
	}
	if (!swap) return ;

	recursiveBubbleSort(a,n-1);
}


void heapify(int a[],int n, int i)
{
	int root  = i;
	int left  = 2*i + 1;
	int right = 2*i +2;

	if(left< n && a[left]>a[root])
		root = left;
	if(right <n && a[right]>a[root])
		root=right;
	if(root!=i)
	{
		swap(a[root],a[i]);

		heapify(a,n,root);
	}
}


void heapSort(int a[], int n) 
{
	// build max heap
	for(int i=n/2-1;i>=0;i--)	// 3, 2, 1, 0
		heapify(a,n,i);

	// sort loop -> swapping , heapify	
	for(int i= n-1;i>0;i--)
	{
		swap(a[0],a[i]);

		heapify(a,i,0);
	}


}





void selectionSort(int a[], int n)
{
	for(int i=0;i<n-1;i++)
	{
		int min_indx=i;
		for(int j=i+1;j<n;j++) // j<n bez j start form '+1'
		{
			if(a[min_indx]>a[j])
			{
				min_indx=j;
			}
		}
		if(min_indx!=i)
		{
			int temp=a[min_indx];
			a[min_indx]=a[i];
			a[i]=temp;
		}
	}
}


void insertionSort(int a[], int n)
{
	for(int i=1;i<n;i++)
	{
		int key = a[i];
		int j= i-1;
		for( ;j>=0 && a[j]>key  ;j--)
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
	int choice;	
	cout<<"//------------------------------------------------------------------------------------------------//\n";
	cout<<"UnSorted array             : ";
	printArray(a,n);
	cout<<"Select sorting methods\n";
	cout<<"Bubble sort               -> 1\n";
	cout<<"Recursive Bubble sort     -> 2\n";
	cout<<"Heap sort                 -> 3\n";
	cout<<"Selection sort            -> 4\n";
	cout<<"Insertion sort            -> 5\n";
	cout<<"\nEnter the input            : ";
	cin>>choice;	
	switch(choice)
	{
		case 1 :        bubbleSort(a,n);
				break;
		case 2 :        recursiveBubbleSort(a,n);
				break;
		case 3 :        heapSort(a,n);
				break;
		case 4 :        selectionSort(a,n);
				break;
		case 5 :        insertionSort(a,n);
				break;
		default:	cout<<"Wrong input      !!! \n";
				return 0;
	}
	cout<<"Sorted array               : ";
	printArray(a,n);

	return 0;
}

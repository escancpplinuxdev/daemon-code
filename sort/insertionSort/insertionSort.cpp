
// insertion sort is conceptually : keep shitting elements left until conditions fails
#include <iostream>
using namespace std;

// i= 0;
// int key = a[0]; // key = first element 
// int j =-1;	   // j=i-1 = -1 
// inner loop condition: for(;j>=0 && a[j] > key;) -> -1>=0 is false -> loop skipped  // skipped entirely first iteration
//a[j+1] = key; // a[-1 + 1] = a[0]=key; same value, which is already there i.e 1 unnecessary/useless iteration 
void printArray(int a[], int n)
{
	for(int i=0;i<n;i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
}


void insertionSort(int a[], int n)
{
	for(int i=1;i<n;i++)   
	{
		int key = a[i];
		int j= i-1;
//		for( ;j>=0 && a[j]>key ; j--) // for clear separation of logic use while loop
		while(j>=0 && a[j]>key)		//  j>=0 prevent an out of bound error
						//                                        while loop give -> condition 
		{				//           
			a[j+1] = a[j];		//                                                        -> work
			j--;			//							  -> update	
		}
		a[j+1]=key;
		printArray(a,n);
		cout<<endl;
	}
}


int main()
{

	int a[] = {5,8,6,3,9,1,7,2,4,0};

	int n = sizeof(a)/sizeof(a[0]);

	cout<<"UnSorted array : ";
	printArray(a,n);
	cout<<"Size = "<<n<<endl;
	insertionSort(a,n);

	cout<<"Sorted array   : ";
	printArray(a,n);



	return 0;

}

/*
 *
 * Deep Dive: Efficiency Analysis (The 30 LPA Talk)

 As a senior C++ developer, you should explain the Time and Space Complexity like this:

 Worst Case (O(n2)): Occurs when the array is sorted in reverse. We perform 1+2+3...+(n−1) comparisons.

 Best Case (O(n)): This is the "Superpower" of Insertion Sort. If the array is already sorted, the inner loop condition a[j] > key fails immediately every time. It only does n−1 comparisons.

 Space Complexity (O(1)): It is an in-place algorithm. No extra memory is allocated (unlike Merge Sort).

Stability: It is stable. It never swaps two equal elements, preserving their original relative order.

Senior Developer Note on "Shifting" vs "Swapping"

You'll notice your code uses a[j+1] = a[j] (Shifting) instead of swap(a[j], a[j+1]).

Why this is better: A swap requires 3 assignments (temp=x,x=y,y=temp). 

By shifting and only placing the key once at the end, you reduce the total number of assignments by roughly a factor of 3.

This is a common performance optimization discussed in high-level interviews.

 *
 */

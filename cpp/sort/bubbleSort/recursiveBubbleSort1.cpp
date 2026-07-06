
#include <iostream>

using namespace std;
// pass 'int *a' so it points to the array
void recursiveBubbleSort(int* a,int n)
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

	// pass 'a' directly not '&a'

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

/*
 
 
Here is the complexity breakdown for your Recursive Bubble Sort.
Complexity Table
Case	Time Complexity	Space Complexity	Reason
Best Case	O(n2)	O(n)	Even if sorted, the recursive calls and loops still run.
Average Case	O(n2)	O(n)	On average, n2 comparisons are made.
Worst Case	O(n2)	O(n)	Array is sorted in reverse; max swaps and comparisons.

//---------------------------- 1. How to Calculate Time Complexity  -----------------------------------------------------//

To calculate time complexity, we look at the total number of comparisons made by the algorithm.

    First Call: The loop runs n−1 times.

    Second Call: The loop runs n−2 times.

    Third Call: The loop runs n−3 times.

    ...

    Last Call: The loop runs 1 time.

The total number of operations is the sum of the first n−1 integers:
Sum=(n−1)+(n−2)+(n−3)+...+1

Using the arithmetic progression formula (n(n+1))/2 , we get:
Total≈(n^2−n)/2

In Big O notation, we drop the constants and lower-order terms, leaving us with O(n^2).


//-------------------------------------2. How to Calculate Space Complexity----------------------------------------------//

This is the most important part for your 30 LPA interview. While an iterative bubble sort has O(1) space complexity, the recursive version has O(n).

Why? The Call Stack.
Every time recursiveBubbleSort calls itself, the operating system creates a Stack Frame. This frame stores:

    Function arguments (int* a, int n).

    Return address (where to go when the function finishes).

    Local variables (int j).

Since the function calls itself n times before reaching the base case (n=1), there will be n frames active in the Stack segment of your Linux memory simultaneously.
Senior Interview Tip: Optimization

If the interviewer asks, "How can you make the Best Case O(n)?", you should suggest a Modified Bubble Sort.

By introducing a bool swapped flag, you can check if any swap happened during the for loop. If no swap occurred, the array is already sorted, and you can return immediately without making further recursive calls.


*/

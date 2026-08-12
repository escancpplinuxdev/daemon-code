
#include <iostream>

int binarySearch (int a[],int size, int target,int &iteration)
{
	int low = 0;
	int high = size - 1;

	while (low <= high)	
	{
		++iteration;
		int mid = low + (high - low ) / 2;
		std::cout<<"mid = "<<mid<<" low = "<<low<<" high = "<<high<<"\n";
		
		if (a[mid] == target )
		{
			return mid;
		}
		else if(a[mid] < target)
		{
			low = mid +1;
		}
		else
		{
			high = mid - 1;
		}

	}

	return -1;
}

int main()
{
	int a [] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91}; // binary must be sorted for binary search work; // sorted array.
//	int a [] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91,92,93,94,95,96,97,98,99,100,101,102,103,104}; 
//	int a [] = {1,2,3,4, 5,6,7, 8, 12, 16, 23, 38, 56, 72, 91,92,93,94,95,96,100,103,104}; 

	int size = sizeof(a) / sizeof(a[0]);
	std::cout<<"size = "<<size<<"\n";
	int target;
	int iteration = 0;
	std::cout<<"Enter the element to be found in sorted array : ";
	std::cin>>target;
	
//	int result = binarySearch(a,size,target);
	int result = binarySearch(a,size,target,iteration);

	if(result != -1 )
	std::cout<<target<<" found at "<<result<<" location. \n";
	else
	std::cout<<target<<" not found. \n";
	
	std::cout<<"iteration = "<<iteration<<"\n";
	return 0;
}

/*
1] 
The goal is to find an exact value target in a sorted array .
each iterator eliminate half of the reamining search space.  -> time complexcity = O(log n).

example i have 10 elements so half of it till 1 elements remain is 10/2 -> 10, 5, 2, 1 -> i.e 4 iteration.

10/2^4 = 1 . 4 iteration.

n/2^k = 1 

n = 2^k
2^k = n 
k = log2(n) -> O(log n) .

2] strict condition required by binary search.
Binary search space must be on soted array. monotonic array.
container must support random access. O(1) jump to any index. -> i.e binary search work on vector/ array not on linked list, it take O(n) time to go mid.
well defined comparison between elements and target.


3] What is "binary search on answer"? How do you recognize a problem needs this pattern?

Answer: Instead of searching for a value within a sorted array, you binary search over the space of possible answers (a range of numbers), using a monotonic — to decide which half of the answer-range to keep.


*/

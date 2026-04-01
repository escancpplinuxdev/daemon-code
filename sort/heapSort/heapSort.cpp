#include <iostream>
using namespace std;

// Heapify subtree rooted at index i
void heapify(int arr[], int n, int i) 
{
	int root = i;        // root
	int left = 2 * i + 1;   // left child
	int right = 2 * i + 2;  // right child


	// If left child is larger
	if (left < n && arr[left] > arr[root])
		root = left;

	// If right child is larger
	if (right < n && arr[right] > arr[root])
		root = right;
	// If root is not largest
	if (root != i) 
	{

		swap(arr[i], arr[root]);

		// Recursively heapify affected subtree
		heapify(arr, n, root);
	}
}

// Heap Sort Function
void heapSort(int arr[], int n) 
{
	// Step 1: Build Max Heap
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// Step 2: Extract elements
	for (int i = n - 1; i > 0; i--) 
	{
		swap(arr[0], arr[i]);  // Move root to end

		// Heapify reduced heap
		heapify(arr, i, 0);
	}
}

// Print array
void printArray(int arr[], int n) 
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
	cout << endl;
}

int main() 
{
	int arr[] = {80,50,30,10,70,60,20,40};
	cout<<"sizeof(arr) = "<<sizeof(arr)<<endl;
	cout<<"sizeof(arr[0]) = "<<sizeof(arr[0])<<endl;
	int n = sizeof(arr) / sizeof(arr[0]);
	cout << "UnSorted array: ";
	printArray(arr, n);

	cout<<"n = "<<n<<endl;
	heapSort(arr, n);

	cout << "Sorted array: ";
	printArray(arr, n);

	return 0;
}

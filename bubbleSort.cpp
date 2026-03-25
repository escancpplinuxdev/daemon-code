#include <iostream>

using namespace std;

int main() 
{
	int a[4] = {13, 5, 3, 1};
	int n = 4;

	cout << "Initial array: ";
//	for (int i = 0; i < n; i++) cout << a[i] << " ";
	for(int n : a) std::cout<< n << " ";
	cout << endl << "-------------------" << endl;

	// Bubble Sort Logic
	for (int i = 0; i < n - 1; i++) 
	{
		bool swapped = false; // Optimization flag

		for (int j = 0; j < n - i - 1; j++) 
		{
			if (a[j] > a[j + 1]) 
			{
				// Manual swap using a temporary variable
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;

				swapped = true;
			}
		}

		// If no two elements were swapped by inner loop, then break
		if (!swapped) break;
	}

	cout << "Sorted array:  ";
	for (int i = 0; i < n; i++) cout << a[i] << " ";
	cout << endl;

	return 0;
}


#include <iostream> // library for input / output
#include <vector>   // library to use vectors
#include <algorithm> // library for general algorithms

using namespace std; // standard namespace


class Solution
{
	public:
		// This function takes a list of numbers and sorts them
		vector<int>	 selectionSort(vector<int>&nums)
		{
			int n=nums.size(); // Get the total number of elements in the vector
					   // Loop for each pass through the vector
			for(int i=0;i<n-1;i++)
			{
				int min_idx = i; // assume current position is minimum

				//			serach the actual array to find actual smallest number
				for (int j = i+1; j<n;j++)			
				{
					// if we find number is smallest than out current minimum
					if(nums[j]<nums[min_idx])
					{
						min_idx = j;//update index of the smallest number

					}
				}
				// swap the found minimum element with the first element of the unsorted part 
				if(min_idx != i)
				{
					int temp = nums[i];
					nums[i]=nums[min_idx];
					nums[min_idx]=temp;
				}

			}
			return nums;	// Return the sorted list of numbers
		}
};

int main()
{
	Solution sol; // create an instance of solution class
	vector<int> data = {64,34,25,12,22,64,11,98}; // create a sample list of numbers

	cout<<"Unsorted array : ";
	for (int x: data)
	{
		cout<<x<<" ";
	}
	cout<<endl;

	// call selectionsort function

	vector<int> sortedData = sol.selectionSort(data);

	cout<<"sorted array   : ";

	// Loop through the sorted list to print each number
	for (int x: sortedData)
	{
		cout<<x<<" ";// Print the number followed by a space
	}
	cout<<endl;

	return 0; // End the program sucessfully.
}

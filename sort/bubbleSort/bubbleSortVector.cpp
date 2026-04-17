
#include <iostream> // library for input / output
#include <vector>   // library to use vectors
#include <algorithm> // library for general algorithms

using namespace std; // standard namespace


class Solution
{
	public:
// This function takes a list of numbers and sorts them
		vector<int>	 bubbleSort(vector<int>&nums)
		{
			int n=nums.size(); // Get the total number of elements in the vector
		  // Loop for each pass through the vector
			for(int i=0;i<n-1;i++)
			{
				bool swap = false; // Flag to check if any numbers moved during this pass
				
				// Loop to compare adjacent elements
				for(int j =0; j<n-i-1;j++)
				{
					  // Check if the current number is bigger than the next one
					if(nums[j]>nums[j+1])
					{
						int temps=nums[j];
						nums[j]=nums[j+1];
						nums[j+1]=temps;
						swap=true; // Mark that we made a change
					}
				}
				   // If no numbers were swapped, the list is already sorted
				if(!swap)break; // if swap == false then !false = true --> i.e yes break;
				// // Stop the loop early to save time
			}
			return nums;	// Return the sorted list of numbers
		}
};

int main()
{
	Solution sol; // create an instance of solution class
	vector<int> data = {64,34,25,12,22,11,98}; // create a sample list of numbers

	cout<<"Unsorted array : ";
	for (int x: data)
	{
		cout<<x<<" ";
	}
	cout<<endl;

	// call bubblesort function

	vector<int> sortedData = sol.bubbleSort(data);
	
	cout<<"sorted array   : ";
	
	 // Loop through the sorted list to print each number
	for (int x: sortedData)
	{
		cout<<x<<" ";// Print the number followed by a space
	}
	cout<<endl;

	return 0; // End the program sucessfully.
}



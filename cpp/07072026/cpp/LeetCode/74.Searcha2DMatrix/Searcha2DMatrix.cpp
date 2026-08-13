
#include <iostream>
#include <vector>
using namespace std;
/*
   class Solution 
   {
   public:
   bool searchMatrix(const std::vector<std::vector<int>>& mat, int target)
   {
   int rows = (int)mat.size();
   int cols = (int)mat[0].size();

   int lo = 0, hi = rows * cols - 1;

   while (lo <= hi)
   {
   int mid = lo + (hi - lo) / 2;
   int val = mat[mid / cols][mid % cols];   // convert flat index -> (row, col)

   if (val == target)      return true;
   if (val < target)        lo = mid + 1;
   else                     hi = mid - 1;
   }

   return false;
   }
// O(log(rows*cols)) — a single binary search over the whole matrix


};
 */
/*
class Solution 
{
	public:
		bool searchMatrix(const std::vector<std::vector<int>>& mat, int target)
		{
			int rows = (int)mat.size();
			int cols = (int)mat[0].size();

			std::cout<<"rows = "<<rows<<"\n";
			std::cout<<"cols = "<<cols<<"\n";
			int lo = 0, hi = rows * cols - 1;

			while (lo <= hi)
			{
				std::cout<<"lo = "<<lo<<"\n";
				std::cout<<"hi = "<<hi<<"\n";

				int mid = lo + (hi - lo) / 2;
				int val = mat[mid / cols][mid % cols];   // convert flat index -> (row, col)
				std::cout<<"mid = "<<mid<<"\n";
				std::cout<<"val = "<<val<<"\n";

				if (val == target)    
				{  return true; }
				if (val < target)
				{
					lo = mid + 1;
				}
				else 
				{
					hi = mid - 1;
				}
			}

			return false;
		}
		// O(log(rows*cols)) — a single binary search over the whole matrix


};
*/

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) 
    {
       int row = matrix.size(), cols = matrix[0].size();
       int lo = 0, hi = row* cols - 1;

       while(lo<=hi)
       {
           int mid = lo + (hi - lo) / 2;
           int val = matrix[mid/cols] [mid % cols];
           if(val == target) {return true;}
           if(val < target) {lo = mid +1;}
           else        {hi = mid -1;}
       }
       return false;
    }
};
int main()
{
	Solution sol;
	//Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3   -> Output: true
	std::vector<std::vector<int>> matrix = {{1,3,5,7}, {10,11,16,20}, {23,30,34,60}};
	int target = 3;
	std::cout<<"o/p -> "<<sol.searchMatrix(matrix,target)<<"\n";

	
	//Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13 -> o/p = false;
	std::vector<std::vector<int>> matrix1 = {{1,3,5,7}, {10,11,16,20}, {23,30,34,60}};
	int target1 = 13;
	std::cout<<"o/p -> "<<sol.searchMatrix(matrix1,target1)<<"\n";

	return 0;
}

/*
   74. Search a 2D Matrix
   Medium
   Topics
   premium lock iconCompanies

   You are given an m x n integer matrix matrix with the following two properties:

   Each row is sorted in non-decreasing order.
   The first integer of each row is greater than the last integer of the previous row.

   Given an integer target, return true if target is in matrix or false otherwise.

   You must write a solution in O(log(m * n)) time complexity.



   Example 1:

Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
Output: true

Example 2:

Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13
Output: false


 */


#include <iostream>
#include <vector>
/*
   class Solution
   {
   public:
   bool searchMatrix(std::vector<std::vector<int>>&matrix, int target)
   {
   int rows = matrix.size(), cols = matrix[0].size();
   int r = 0, c = cols - 1;

   while(r< rows && c >= 0)
   {
   if(matrix[r][c] == target)
   {
   return true;
   }
   else if (matrix[r][c] > target)
   {
   --c;
   }
   else
   {
   ++r;
   }

   }
   return false;
   }
   };
 */


class Solution
{
	public:
		bool searchMatrix(std::vector<std::vector<int>>&matrix, int target)
		{
			int rows = matrix.size(), cols = matrix[0].size();
			int r = 0, c = cols - 1;

			while(r< rows && c >= 0)
			{
				std::cout<<"r = "<<r<<"\n";
				std::cout<<"c = "<<c<<"\n";
				std::cout<<"matrix["<<r<<"] ["<<c<<"] = "<<matrix[r][c]<<" =  "<<target<<"\n";
				if(matrix[r][c] == target)
				{
					return true;
				}
				else if (matrix[r][c] > target)
				{
					--c;
				}
				else
				{
					++r;
				}

			}
			return false;
		}
};


// Olog(m+n) 
int main()
{	
	Solution sol;
	//Input: matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 5 -> Output: true
/*	std::vector<std::vector<int>> matrix1= {{1,4,7,11,15},{2,5,8,12,19},{3,6,9,16,22},{10,13,14,17,24},{18,21,23,26,30}};
	std::cout<<"o/p = "<<sol.searchMatrix(matrix1,5)<<"\n";
*/
	//Input: matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 20 -> Output: false

	   std::vector<std::vector<int>> matrix2= {{1,4,7,11,15},{2,5,8,12,19},{3,6,9,16,22},{10,13,14,17,24},{18,21,23,26,30}};
	   std::cout<<"o/p = "<<sol.searchMatrix(matrix2,20)<<"\n";


	return 0;
}

/*
   240. Search a 2D Matrix II
   Medium
   Topics
   premium lock iconCompanies

   Write an efficient algorithm that searches for a value target in an m x n integer matrix matrix. This matrix has the following properties:

   Integers in each row are sorted in ascending from left to right.
   Integers in each column are sorted in ascending from top to bottom.



   Example 1:

Input: matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 5
Output: true

Example 2:

Input: matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 20
Output: false


 */

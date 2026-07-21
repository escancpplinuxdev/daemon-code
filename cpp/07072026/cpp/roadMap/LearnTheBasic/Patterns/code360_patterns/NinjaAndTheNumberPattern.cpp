
#include <iostream>
#include <algorithm>  //for std::min
class Solution
{
	public : 
	void getNumberPattern(int n)	
	{	
		int size = 2 *n -1;
		for(int i = 1 ; i<= size ; ++i)
		{
			for(int j = 1; j<= size ; ++j)
			{
				int top = i - 1;
				int bottom = size - i;
	
				int left = j - 1;
				int right = size - j;
				
				int min = std::min(std::min(top,bottom), std::min(left,right));

				int value = n - min;
				std::cout<<value;
			}
				std::cout<<"\n";
		}
	}
};
int main()
{
	Solution sol;
	sol.getNumberPattern(4);
	return 0;
}

/*


*/

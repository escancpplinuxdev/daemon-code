
#include <iostream>
#include <algorithm> // for std::min

class Solution
{
	public: 
		void pattern22 (int n)
		{
			int size = n *2 - 1;
			for(int i = 1; i<= size ; ++i)			
			{
				for(int j = 1 ; j<=size; ++j)
				{
					int top = i - 1 ;
					int bottom = size -i;
				
					int left = j - 1;
					int right = size - j;
				
					int minDistance =  std::min(std::min(top,bottom),std::min(left,right));
					int value = n - minDistance;
					std::cout<<value<<" ";
				}
					std::cout<<"\n";
			}
		}
};

int main()
{
	Solution sol;
	sol.pattern22(5);
	return 0;
}

/*


*/

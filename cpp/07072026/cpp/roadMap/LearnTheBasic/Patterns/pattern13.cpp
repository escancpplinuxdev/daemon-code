
#include <iostream>

class Solution 
{
	public :
		void pattern13 (int n) 
		{
			int k = 0;
			for (int i = 1; i<=n ; ++i)			
			{
				for(int j = 1 ; j<=i ; ++j)
				{
					std::cout<<++k<<" ";
				}
				std::cout<<"\n";
			}
		}

};



int main()
{
	Solution sol;
	sol.pattern13(5);
	return 0;
}

/*


*/


#include <iostream>

class Solution 
{
	public :
		void pattern2(int n)
		{
			for(int i = 1; i<=n ; ++i)
			{
				for(int j = 1; j<= i; ++j)
				{
					std::cout<<"*";
				}
					std::cout<<"\n";
			}
		}
};

int main()
{	
	Solution sol;
	sol.pattern2(5);

	return 0;
}

/*


*/

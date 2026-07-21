
#include <iostream>

class Solution 
{
	public:
		void pattern17(int n)
		{	
			for(int i = 1; i<=n ; ++i)
			{
				int k = 64;
				for(int j = n-1 ; j>= i ; --j)
				{
					std::cout<<" ";
				}
				for(int j = 1 ; j<= i ; ++j)
				{
					printf("%c",++k);
				}
				for(int j = 2 ; j<= i ; ++j)
				{
					printf("%c",--k);
				}

				std::cout<<"\n";
			}
		}
};

int main()
{
	Solution sol;
	sol.pattern17(5);
	return 0;
}

/*


 */

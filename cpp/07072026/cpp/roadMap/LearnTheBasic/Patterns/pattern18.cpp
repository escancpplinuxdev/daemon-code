
#include <iostream>
class Solution
{
	public:
		void pattern (int n)	
		{
			for(int i = 1; i<=n; ++i)
			{
				int k =64 + n;
				for(int j= 1; j<=i; ++j)
				{
					--k;
				}
				for(int j= 1; j<=i; ++j)
				{
					printf("%c ",++k);
				}
				std::cout<<"\n";
			}
		}
};
int main()
{
	Solution sol;
	sol.pattern(5);
	return 0;
}

/*


*/

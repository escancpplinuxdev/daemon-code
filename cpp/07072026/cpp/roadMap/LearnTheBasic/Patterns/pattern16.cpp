
#include <iostream>

class Solution
{
	public :	
		void pattern14 (int n)
		{
			int k = 65;
			for(int i =1;i<=n; ++i)
			{
				for(int j = 1; j<= i; ++j)
				{
					printf("%c",k);
				}
				++k;
				std::cout<<"\n";
			}
		}

};

int main()
{
	Solution sol;
	sol.pattern14(5);
	return 0;
}

/*


 */


#include <iostream>
class Solution
{
	public	:
		void pattern19(int n)
		{
			for(int i = 1 ; i<=n ; ++i)
			{
				for(int j = n ; j >= i ; --j)
				{
					std::cout<<"*";
				}
				for(int j = 1 ; j < i ; ++j)
				{
					std::cout<<" ";
				}
				for(int j = 1 ; j < i ; ++j)
				{
					std::cout<<" ";
				}
				for(int j = n ; j >= i ; --j)
				{
					std::cout<<"*";
				}

				std::cout<<"\n";
			}
			for(int i = 1 ; i<=n ; ++i)
			{
				for(int j = 1 ; j <= i ; ++j)
				{
					std::cout<<"*";
				}
				for(int j = n ; j > i ; --j)
				{
					std::cout<<" ";
				}
				for(int j = n ; j > i ; --j)
				{
					std::cout<<" ";
				}
				for(int j = 1 ; j <= i ; ++j)
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
	sol.pattern19(5);
	return 0;
}

/*


*/

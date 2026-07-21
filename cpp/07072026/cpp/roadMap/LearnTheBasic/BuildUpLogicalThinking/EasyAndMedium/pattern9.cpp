
#include <iostream>
#include <string>
class Solution 
{
	public:

		void pattern9(int n)
		{
			for(int i = 1 ; i<=n ; ++i) // vertical // upper
			{
				for(int j = n-i; j>0 ; --j) // horizantal
				{
					std::cout<<" ";
				}

				for(int j = 0; j<i ; ++j) // horizantal
				{
					std::cout<<"*";
				}
				for(int j = 0; j<i-1; ++j) // horizantal
				{
					std::cout<<"*";
				}

				std::cout<<"\n";
			}
			for(int i = 0 ; i<n ; ++i) // vertical   // lower
			{
				for(int j = 0; j<i ; ++j) // horizantal
				{
					std::cout<<" ";
				}
				for(int j = n -i ; j>0; --j) // horizantal
				{
					std::cout<<"*";
				}
				for(int j = n - i -1 ; j>0 ; --j) // horizantal
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
	int n = 5;
	sol.pattern9(n);
	return 0;
}

/*


*/

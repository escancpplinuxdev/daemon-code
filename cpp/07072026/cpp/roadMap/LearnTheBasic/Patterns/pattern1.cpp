
#include <iostream>

class Solution
{
	public: 
		void pattern1 (int n)
		{
			for(int i = 1; i<=n ; ++i)	
			{
				for(int j = 1 ; j <=n; ++j)
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
	sol.pattern1(5);
	return 0;
}

/*


*/

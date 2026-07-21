
#include <iostream>

class Solution
{
	public:
		void pattern10(int n)	
		{
			for(int i = 0; i<n-1 ;++i)
			{
				for(int j = 0; j <=i ; ++j)
				{
					std::cout<<"*";
				}
				std::cout<<"\n";
			}
		
			for(int i = n-1 ; i>=0; --i)
			{
				for(int j = 0 ; j <=i ; ++j)
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
//	std::cin>>n;
	sol.pattern10(n);

	return 0;
}

/*


 */

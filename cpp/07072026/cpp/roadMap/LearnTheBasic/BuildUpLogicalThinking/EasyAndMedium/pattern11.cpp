
#include <iostream>

class Solution
{
	public : 
		void pattern11(int n)
		{
			for(int i = 1 ; i<=n ; ++i)
			{
				for(int j = 1; j <= i ; ++j)
				{
					if(i%2==1)
					{
						if(j%2==1)
							std::cout<<"1";
						else
							std::cout<<"0";
							std::cout<<" ";
					}
					else
					{
						if(j%2==1)
							std::cout<<"0";
						else
							std::cout<<"1";
							std::cout<<" ";

					}
				}
				std::cout<<"\n";
			}
		}
};

int main()
{
	Solution sol;
	int n = 5;
	sol.pattern11(n);
	return 0;
}

/*


 */

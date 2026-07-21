
#include <iostream>
#include <string>
class Solution 
{
	public	:
		void pattern(int n)
		{
			for(int i = 1; i<=n; ++i)
			{
				for(int j =1 ; j <=i ; ++j)
				{
					std::cout<<j;
				}
				
				for(int j =n ; j >i; --j)
				{
					std::cout<<" ";
				}
				for(int j =n ; j >i; --j)
				{
					std::cout<<" ";
				}
				for(int j = 0 ; j<i ; ++j)
				{
					std::cout<<i-j;
				}
				
				std::cout<<"\n";
			}


			 for(int i = 1; i>=n; --i)
                        {
                                for(int j =1 ; j >i ; --j)
                                {
                                        std::cout<<j;
                                }

                                for(int j =n ; j <i; ++j)
                                {
                                        std::cout<<" ";
                                }
                                for(int j =n ; j <i; ++j)
                                {
                                        std::cout<<" ";
                                }
                                for(int j = 0 ; j>i ; --j)
                                {
                                        std::cout<<i+j;
                                }

                                std::cout<<"\n";
                        }


		}
};

int main()
{
	Solution sol;
//	std::cout<<"Enter size of pattern : ";
	int n = 5;
//	std::cin>>n;
	sol.pattern(n);
	return 0;
}

/*


*/

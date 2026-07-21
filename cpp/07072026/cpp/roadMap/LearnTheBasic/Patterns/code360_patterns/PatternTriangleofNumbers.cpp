
#include <iostream>

int main()
{
	int n; 
	std::cin>>n;

	int k = 1;
	for (int  i = 1; i<=n; ++i)
	{	
		for(int j = n; j> i ; --j)
		{
			std::cout<<" ";
		}
		for(int j = i; j<=2*i-1; ++j)
		{
			std::cout<<j;
		}
		for(int j = 2*i-2; j>=i ; --j)
		{
			std::cout<<j;
		}

		std::cout<<"\n";
	}
	return 0;
}

/*


 */

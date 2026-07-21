
#include <iostream>

int main()
{
	int n;
	std::cin>>n;

	for(int i = 1;i<= n; ++i)
	{
		int k = i;
		for(int j = n; j>i; --j)
		{
			std::cout<<" ";
		}
		for(int j = 1; j<= i; ++j)
		{
			
			std::cout<<k++;
		}
			std::cout<<"\n";
	}
	return 0;
}

/*


*/

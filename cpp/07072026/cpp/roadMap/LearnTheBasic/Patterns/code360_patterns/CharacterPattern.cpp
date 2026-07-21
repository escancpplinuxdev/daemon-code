
#include <iostream>

int main()
{
	int n;
	std::cin>>n;

	for(int i= 1; i<=n; ++i)
	{
		int k = 63+i;
		
		for(int j = 1; j<=i; ++j)
		{
			printf("%c",++k);
		}
		std::cout<<"\n";
	}

	return 0;
}

/*


 */

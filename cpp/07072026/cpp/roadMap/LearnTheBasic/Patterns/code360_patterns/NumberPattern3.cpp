
#include <iostream>

int main()
{
	  int n;
	        std::cin>>n;
	for(int i = 1 ; i <= n; ++i)
	{
		int k = 1;
		for(int j = 1; j<=i ; ++j)
		{
			if(j>1 && j!=i  )
				std::cout<<"2";
			else
				std::cout<<"1";

		}
			std::cout<<"\n";
	}
	return 0;
}

/*


*/

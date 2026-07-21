
#include <iostream>

int main()
{

       /*  Read input as specified in the question.
	* Print output as specified in the question.
	*/
  	int n;
	std::cin>>n;
	
	  for(int i=1; i<=n; ++i)
                        {
                                for(int j =1; j<=i; ++j)
                                {
                                        std::cout<<i;
                                }
                                        std::cout<<"\n";
                        }
	return 0;
}


#include <iostream>

void nForest(int n)
{
	// Write your code here.
	  for(int i = 1; i<=n ; ++i)
                        {
                                for(int j = 1; j<= i; ++j)
                                {       
                                        std::cout<<"* ";
                                }
                                        std::cout<<"\n";
                        }

}
 
int main()
{
	nForest(5);
	return 0;
}

/*


*/

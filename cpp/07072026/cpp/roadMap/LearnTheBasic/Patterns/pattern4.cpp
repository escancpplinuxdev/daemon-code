
#include <iostream>

class Solution 
{
	public:
		void pattern4(int n)
		{
			for(int i=1; i<n; ++i)
			{
				for(int j =1; j<=i; ++j)
				{
					std::cout<<i;
				}
					std::cout<<"\n";
			}
		}
};
int main()
{
	Solution sol;
	sol.pattern4(5);
	return 0;
}

/*


*/

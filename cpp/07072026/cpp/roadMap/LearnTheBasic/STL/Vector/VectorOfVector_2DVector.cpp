
#include <iostream>
#include <vector>

void printVector(const std::vector<std::vector<int>>&v)
{
	for(auto row : v)
	{		for(auto x: row)
		{
			std::cout<<x<<" ";
		}
			std::cout<<"\n";
	}
}
void initialize()
{
	std::vector<std::vector<int>>v = {

		{1, 2, 3},

		{4, 5, 6},

		{7, 8, 9}

	};

	printVector(v);


}
int main()
{
	initialize();
	return 0;
}

/*


 */

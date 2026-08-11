
#include <iostream>
#include <algorithm> // std::binary_search , std::find
#include <vector>    // std::vector
int main()
{
	//	std::vector<int> a  = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
	// 	int target = 56;
	std::vector<std::string> a = {"Apple", "Banana", "Cherry", "Date", "Grape", "Kiwi", "Orange"};
	std::string target = "Date";


	int result = std::binary_search(a.begin(),a.end(),target); // return false 0 -> not found
								   // return true  1 -> found

  	auto it = std::find(a.begin(),a.end(),target);
	if(it != a.end() && *it == target)
	{
		int index = std::distance(a.begin(),it);
		std::cout<<target<<" found at "<<index<<" location. \n";
	}

	if(result == 1 )
	{
		std::cout<<target<<" found . \n";
	}
	else
	{
		std::cout<<target<<" not found. \n";
	}

	return 0;
}

/*


 */

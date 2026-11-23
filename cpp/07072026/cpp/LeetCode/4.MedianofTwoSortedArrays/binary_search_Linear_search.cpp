
#include <iostream>
#include <vector>

int binarySearch (const std::vector<int >&a,  int target)
{
	int lo = 0, hi = static_cast<int>(a.size()) -1;

	while (lo<=hi)
	{
		int mid = lo + (hi -lo) /2; // avoids overflow

		if(a[mid]== target) 	return mid;
		else if( a[mid] < target )  lo = mid +1;  
		else 	hi = mid -1;
		
	}
		return -1;
}

int linearSearch (const std::vector<int>&a, int target)
{
	for(int i = 0 ; i<static_cast<int>(a.size()); ++i)
	{
		if (a[i] == target) return i;
	}
	return -1;
}
int main()
{
	std::vector <int > a = {-5, -1, 0, 3, 7, 9, 12, 20, 25, 31};

	std::cout<<"Array : ";
	for (int x :a) std::cout<<x<<"  ";
	std::cout<<"\n\n";

	//Present targets
	for(int t :{3,-5,31,12,15,17})
	{
		std::cout<<"BinarySearch   '"<<t<<"' ) = "<<binarySearch(a,t)<<"\t";
		std::cout<<((binarySearch(a,t) == -1 ) ? "Not Found" : "Present")<<"\n";

		std::cout<<"LinearSearch    '"<<t<<"') = "<<linearSearch(a,t)<<"\t";
		std::cout<<((linearSearch(a,t) == -1) ? "Not Found" : "Present")<<"\n";
	}
	return 0;
}

/*


*/

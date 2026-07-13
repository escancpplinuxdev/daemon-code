
#include <iostream>
#include <vector>
#include <string>

double medianOfTwoVectorArray (std::vector<int>&a,std::vector<int>&b)
{
	std::vector<int> merge ;
	int n1=a.size(), n2=b.size();
	int i =0, j= 0;
	merge.reserve(n1+n2);
	while(i<n1 && j<n2)
	{
		if(a[i]<b[j])
		{
			merge.push_back(a[i]);
			++i;
		}
		else
		{
			merge.push_back(b[j]);
			++j;

		}
	}
	if(i<n1)
	{
		merge.push_back(a[i]);
		++i;
	}

	if(j<n2)
	{
		merge.push_back(b[j]);
		++j;
	}

	size_t n = merge.size();
	if(n==0)	
	{
		return 0.0;
	}

	for(int k = 0 ; k<n;++k)
	{
		std::cout<<merge[k]<<" ";
	}


	//	double median =(n%2 == 1) ?   merge[n/2] :  (merge[n/2] + merge[n/2 - 1]) / 2.0;
	//	return median;


	if(n%2)	//odd
	{
		return merge[n/2];
	}
	else
	{
		return (merge[n/2] + merge[n/2 - 1]) / 2.0;
	}

}


int main()
{
	std::vector<int> a = {1, 2, 3, 4, 5};
	std::vector<int> b = {6, 7, 8, 9, 10};

	double median = medianOfTwoVectorArray(a, b);
	std::cout << "Median = " << median << "\n";

	std::vector<int> c = {1, 3, 5};
	std::vector<int> d = {2, 4, 6};
	median = medianOfTwoVectorArray(c, d);
	std::cout << "Median = " << median << "\n";
	// merged = {1,2,3,4,5,6} → even → median = (3+4)/2 = 3.5


	std::vector<int> e = {1, 3, 5, 7};
	std::vector<int> f = {2, 4, 6, 8, 10};

	median = medianOfTwoVectorArray(e, f);
	std::cout << "Median = " << median << std::endl;  // Output: 5

	// Another test with odd total length
	std::vector<int> g = {1, 2};
	std::vector<int> h = {3, 4, 5};
	std::cout << "Median = " <<medianOfTwoVectorArray(g, h) << std::endl; // Output: 3
	return 0;
}


#include <iostream>
#include <vector>


double medianOfTwoSortedVectorArrayByThirdArray(std::vector<int>&a,std::vector<int>&b)
{
	size_t n1=a.size(), n2=b.size(), n;
	n=n1+n2;
	std::vector<int>m;
	size_t i = 0,j=0;
	m.reserve(n);

	while (i<n1 && j<n2)
	{
		if(a[i]<=b[j])
		{
			m.push_back(a[i]);
			++i;
		}
		else
		{

			m.push_back(b[j]);
			++j;
		}
	}

// append remaining elements	
	while(i<n1)
	{
		m.push_back(a[i]);
		++i;
	}
	while(j<n2)
	{

		m.push_back(b[j]);
		++j;
	}
// debug output
	std::cout<<"a n1 = '"<<n1<<"' = \t";
	for(size_t k =0;k<n1;++k)
	{
		std::cout<<a[k]<<" ";
	}
	std::cout<<"\n";
	std::cout<<"b n2 = '"<<n2<<"' = \t";
	for(size_t k =0;k<n2;++k)
	{
		std::cout<<b[k]<<" ";
	}
	std::cout<<"\n";
	std::cout<<"m  n = '"<<n<<"' = \t";

	for(size_t k =0;k<n;++k)
	{
		std::cout<<m[k]<<" ";
	}

	if (n%2)
	{
		std::cout<<"cal = "<<n/2<<" element =  "<< m[n/2];
		return m[n/2];
	}
	else
	{	
		std::cout<<"cal = "<<m[n/2]<<" + "<< m[n/2 -1 ]<<"/2.0 = ";
		return (m[n/2] + m[n/2 -1 ] ) /2.0 ;
	}
	return 0.0;
}

int main()
{
	double median;
	std::vector<int>a={5,6,7,8,9};
	std::vector<int>b={4,6,8,9};

	median = medianOfTwoSortedVectorArrayByThirdArray(a,b);
	std::cout<<"\t median = '"<<median<<"'\n";

	std::vector<int> a1 = {1, 2, 3, 4, 5};
	std::vector<int> b1 = {6, 7, 8, 9, 10};

	median = medianOfTwoSortedVectorArrayByThirdArray(a, b);
	std::cout << "\tMedian = " << median << "\n";

	std::vector<int> c = {1, 3, 5};
	std::vector<int> d = {2, 4, 6};
	median = medianOfTwoSortedVectorArrayByThirdArray(c, d);
	std::cout << "\texpected = 3.5 Median = " << median << "\n";
	// merged = {1,2,3,4,5,6} → even → median = (3+4)/2 = 3.5


	std::vector<int> e = {1, 3, 5, 7};
	std::vector<int> f = {2, 4, 6, 8, 10};

	median = medianOfTwoSortedVectorArrayByThirdArray(e, f);
	std::cout << "\texpected = 5 Median = " << median << std::endl;  // Output: 5

	// Another test with odd total length
	std::vector<int> g = {1, 2};
	std::vector<int> h = {3, 4, 5};
	median = medianOfTwoSortedVectorArrayByThirdArray(g,h);
	std::cout << "\t expected = 3 Median = " <<median<< std::endl; // Output: 3

	return 0;
}

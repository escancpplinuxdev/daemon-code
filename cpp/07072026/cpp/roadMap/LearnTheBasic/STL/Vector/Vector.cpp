
#include <iostream>
#include <vector>

void Ctor()
{
//	std::vector<int>v{10}; pass -> accept as initializer list o/p -> 10

	std::cout<<"Constructs with n default‑inserted elements. \n";
	std::vector<int>v(10); // o/p -> 0 0 0 0 0 0 0 0 0 0
	
	std::vector<int>::iterator it;
	for(it=v.begin(); it!= v.end(); ++it)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<"\n";
	
	std::cout<<"Constructs with n copies of value.\n";
//	std::vector<int>v1{5,3}; pass -> accept as initializer list o/p -> 5 3
	
	std::vector<int>v1(5,3); // o/p -> 3 3 3 3 3

	std::vector<int>::const_iterator it1;
	for(it1 = v1.cbegin(); it1!= v1.cend(); ++it1)
	{
		std::cout<<*it1<<" ";
	}
	std::cout<<"\n";
	
	std::cout<<"Constructs from braced initializer list.\n";
	std::vector<int>v2 = {1,2,3,4,5,6,7,8,9};  // o/p -> 9 8 7 6 5 4 3 2 1

//	std::vector<int>v2 = (1,2,3,4,5,6,7,8,9); error: conversion from ‘int’ to non-scalar type ‘std::vector<int>’ requested

//	std::vector<int>v2 = [1,2,3,4,5,6,7,8,9];  read this as In lambda function:
						   
	std::vector<int>::const_reverse_iterator it2;
        for(it2 = v2.crbegin(); it2 != v2.crend(); ++it2)
        {
                std::cout<<*it2<<" ";
        }
        std::cout<<"\n";

	std::cout<<"Copy ctor. \n";

	std::vector<int>v3(v);
	
	std::vector<int>::reverse_iterator it3;
	for(it3 = v3.rbegin(); it3 != v3.rend(); ++it3)
	{
		std::cout<<*it3<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Move constructor.\n";

	std::vector<int>v4(std::move(v2));
	std::vector<int>::iterator it4;
	for(it4 = v4.begin(); it4 != v4.end(); ++it4)
	{
		std::cout<<*it4<<" ";
	}
	std::cout<<"\n";


	std::cout<<"Copy assignment.\n";

	std::vector<int>v5=v4;
	std::vector<int>::iterator it5;
	for( it5 = v5.begin(); it5 != v5.end(); ++it5)
	{
		std::cout<<*it5<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Move assignment.\n";
	std::vector<int>v6=std::move(v1);
	for(auto it6 = v6.begin(); it6 != v6.end(); ++it6)
	{
		std::cout<<*it6<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Assign from initializer list.\n";
	std::vector<int> v7 ;
	v7 = {5,3,6,49,3,1,6,4,91,6,4,6,9,4};
	for(auto it7 = v7.begin(); it7!= v7.end(); ++it7)
	{
		std::cout<<*it7<<" ";
	}
	std::cout<<"\n";
}

void Capacity()
{
	std::vector<int>v = {1,2,3,4,5,6,7,8,9,0};
	for(int it : v)
	{
		std::cout<<it<<" ";
	}
	std::cout<<"\n";
	std::cout<<"v.size() = '"<<v.size()<<"'\n";


	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n";

	std::cout<<"v.empty() = '"<<(v.empty() ? "true" : "false")<<"'\n";

	v.reserve(20); // return void    Cannot print void.
	std::cout<<"v.reserve(10) = '"<<v.capacity()<<"'\n";

	v.shrink_to_fit(); // return void    Cannot print void.
	std::cout<<"v.shrink_to_fit() = '"<<v.capacity()<<"'\n";

	std::cout<<"v.max_size() = '"<<v.max_size()<<"'\n";
	
	v.resize(40); // return void	Cannot print void.
	std::cout<<"v.resize(40) = '"<<v.size()<<"'\n";

}

int main()
{
	std::vector<int>v = {1,2,3,4,5};

	std::cout<<"1. Vector Ctor and Assignment \n";

	Ctor();
	
	std::cout<<"\n";
	std::cout<<"2. Vector Capacity \n";
	Capacity();


	return 0;
}

/*


*/

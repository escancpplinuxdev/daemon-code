
#include <iostream>
#include <unordered_set>
int main()
{
	std::unordered_set<int>us = {1,2,3,4,5,6,1,2,3};

	for(auto it: us)
		std::cout<<it<<" ";
	std::cout<<"\n";

	//1]	ctors

	std::unordered_set<int>s1; 			// empty
	std::unordered_set<int>s2 = {1,2,3,4,5,6,7}; 	// initializer list
	std::unordered_set<int>s3(s2);			// copy ctors 
	std::unordered_set<int>s4(std::move(s3));	// move ctor // -std=c++11
	std::unordered_set<int>s(s2.begin(),s2.end());  // range ctor)


	//2] 	size/capacity

	s.size();
	if(s.empty())
	{
		std::cout<<"empty \n";
	}
	else
	{
		std::cout<<"not empty \n";

	}
	s.max_size();

	//3] 	modifiers 

	s.insert(1);
	s.emplace(3);
	if(s.erase(5) == 1 ) // return 1 if reamoved
	{
		std::cout<<"remove\n";
	}
	else
	{
		std::cout<<"not remove\n";
	}	


	auto it = s.find(6);
	if(it != s.end()) // if not found return s.end() value
	{
		std::cout<<"found \n";
	}
	else
	{
		std::cout<<"not found \n";

	}
//	s.clear();

	//4]	 lookup

	if(s.count(2))
	{
		std::cout<<"count\n";
	}
	else
	{
		std::cout<<"count not\n";
	}

	//5]	 iterators
	for (auto it = s.begin(); it != s.end(); ++it) 
	{
		std::cout << *it << ' ';
	}
	std::cout<<"\n";
	for (auto it = s.cbegin(); it != s.cend(); ++it) 
	{
		std::cout << *it << ' ';
	}
	std::cout<<"\n";

	// or range-based for
	for (const auto& val : s) 
	{
		std::cout << val << ' ';
	}
	std::cout<<"\n";
	return 0;
}

/*
   associative	// Associative containers -> जोड़नेवाला-> data structure that map keys to value.
   that contain set of unique objects 
   unordered_set is store unique elements

   An unordered set in STL is a associative container that stores unique elements in no order. Every operation on an unordered set takes in the average case -> O(1) complexity and takes in the worst case O(n). 

   std::unordered_set is an associative container that contains a set of unique objects of type Key
   It uses a hash table for storage, so elements are not ordered
   Search, insertion, and removal have average O(1) time complexity

Header: <unordered_set> -> #include<unordered_set>
namespace: std		-> std::unordered_set

std::unordered_set<int>us = {1,2,3,4,5,6,1,2,3};

Key Characteristics
Property			Description
Unique keys			No duplicate elements allowed. Inserting an existing element has no effect.
Unordered			Elements are stored in buckets based on hash; order is non. There is no order.
Constant‑time average		Insert, erase, find – average O(1), worst-case O(n).
Forward iterators		Iterators can move forward only.
Allocator‑aware			Supports custom allocators for memory management.

 */

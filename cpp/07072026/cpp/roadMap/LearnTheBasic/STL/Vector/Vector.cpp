
#include <iostream>
#include <vector>

void printVector(const std::vector<int>v);

void Ctor()
{
	//	std::vector<int>v{10}; pass -> accept as initializer list o/p -> 10

	std::cout<<"Constructs with 10 default‑inserted elements 0 . \n";
	std::vector<int>v(10); // o/p -> 0 0 0 0 0 0 0 0 0 0

	std::vector<int>::iterator it;
	for(it=v.begin(); it!= v.end(); ++it)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Constructs with 5 copies of value 3 .\n";
	//	std::vector<int>v1{5,3}; pass -> accept as initializer list o/p -> 5 3

	std::vector<int>v1(5,3); // o/p -> 3 3 3 3 3	// // Uses () -> Calls constructor (size, value)
							// Result: 5 elements, all initialized to 3.

	std::vector<int>::const_iterator it1;
	for(it1 = v1.cbegin(); it1!= v1.cend(); ++it1)
	{
		std::cout<<*it1<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Constructs from braced initializer list.\n";
// Uses {} -> Calls std::initializer_list 
// Result: 9 elements: the number 1 to  the number 9.

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

	std::vector<int>v5;
	v5=v4;
	std::vector<int>::iterator it5;
	for( it5 = v5.begin(); it5 != v5.end(); ++it5)
	{
		std::cout<<*it5<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Move assignment.\n";
	std::vector<int>v6;
	v6=std::move(v1);
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
	std::cout<<"v.size() = '"<<v.size()<<"'\n"; // Number of elements.	// return size_t


	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n"; // Allocated storage capacity (capacity ≥ size). // return size_t

	printVector(v);
	std::cout<<"v.empty() = '"<<(v.empty() ? "true" : "false")<<"'\n"; // Checks if empty. // return bool.

	v.reserve(20); // return void    Cannot print void.	// Requests capacity to be at least 20.
	std::cout<<"v.reserve(10) = '"<<v.capacity()<<"'\n";
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<"\n";	//returns the size of the vector
	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n"; // Allocated storage capacity (capacity ≥ size). // return size_t

	v.shrink_to_fit(); // return void    Cannot print void.	// Requests removal of unused capacity.
	std::cout<<"v.shrink_to_fit() = '"<<v.capacity()<<"'\n";
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<"\n";	//returns the size of the vector
	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n"; // Allocated storage capacity (capacity ≥ size). // return size_t


	std::cout<<"v.max_size() = '"<<v.max_size()<<"'\n";	//Maximum possible elements // return type size_t

	v.resize(24); // return void	Cannot print void.	//Changes size to 24; initialises new elements to value 0.
	std::cout<<"v.resize(24) = '"<<v.size()<<"'\n";
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<"\n";	//returns the size of the vector
	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n"; // Allocated storage capacity (capacity ≥ size). // return size_t

	v.resize(27,7); // return void	Cannot print void.	//Changes size to 27; initialises new elements to value 7.
	std::cout<<"v.resize(27, 7) = '"<<v.size()<<"'\n";
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<"\n";	//returns the size of the vector
	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n"; // Allocated storage capacity (capacity ≥ size). // return size_t

	v.shrink_to_fit(); // return void    Cannot print void.	// Requests removal of unused capacity.
	std::cout<<"v.shrink_to_fit() = '"<<v.capacity()<<"'\n";
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<"\n";	//returns the size of the vector
	std::cout<<"v.capacity() = '"<<v.capacity()<<"'\n"; // Allocated storage capacity (capacity ≥ size). // return size_t

}

void ElementAccess()
{
	std::vector<int> v(10);
	
	int x = v[5];
	printVector(v);
	std::cout<<"v[5] = "<<v[5]<<"\n";	//Accesses element without bounds checking.
	std::cout<<"x = "<<x<<"\n";

	
	std::cout<<"v.size() = "<<v.size()<<"\n"; // o/p -> 10
	std::cout<<"v[15] = "<<v[15]<<"  -> undefined if out-of-bounds.\n";	//undefined if out-of-bounds.

	std::vector<int>v1 = {9,4,5,6,4,7,9,1,3,8};
	printVector(v1);
	x=v1.at(5);
	std::cout<<"v1.at(5) = "<<v1.at(5)<<"\n";
	std::cout<<"x = "<<x<<"\n";
	
	std::cout<<"v1.size() = "<<v1.size()<<"\n"; // o/p -> 10
//	std::cout<<"v1.at(15) = "<<v1.at(15)<<"\n"; // 15 > 10 throws std::out_of_range if position ≥ size. 
//terminate called after throwing an instance of 'std::out_of_range'

	try
	{
		std::cout<<"v1.at(15) = "<<v1.at(15)<<"\n";
	}
	catch (const std::exception& e)
	{
		std::cerr<<"ERROR : "<<e.what()<<"\n";
	}

	printVector(v1);
	std::cout<<"v1.front() = "<<v1.front()<<"\n";
	std::cout<<"v1.back() = "<<v1.back()<<"\n";
	std::vector<int> v2 = v1;

	v1.clear(); // return type void // cannot print
	std::cout<<"v1.clear() = \n"; 
	printVector(v1);
	std::cout<<"v1.empty() = "<<(v1.empty() ? "true" : "false")<<"\n";
	
	std::cout<<"v1.front() = "<<v1.front()<<"\n";
	std::cout<<"v1.back() = "<<v1.back()<<"\n";
	std::cout<<"v1.data() = "<<v1.data()<<"\n";
	std::cout<<"*v1.data() = "<<*v1.data()<<"\n"; ; // Returns pointer to underlying array. (if empty, pointer may be null)

	printVector(v2);
	std::cout<<"v2.data() = "<<v2.data()<<"\n";
	std::cout<<"v2.data() = "<<*v2.data()<<"\n";
	int *p = v2.data(); // Returns pointer to underlying array. (if empty, pointer may be null)
	std::cout<<"*p = "<<*p<<"\n";
}
void printVector(const std::vector<int>v)
{
	std::cout<<"[";
	for(auto it = v.begin(); it!= v.end(); ++it) 	
	{
		if(it != v.begin())
		{
			//			std::cout<<" , "; // read form beginning. v.begin()
		}
		std::cout<<*it; // iterator it like a pointer to the element, pointer which have element memory address at it , and dereference have actual element value.

		if(it != v.end() -1)  // read form v.end() -1 
		{
			std::cout<<" , ";
		}
	}
	std::cout<<"]\n";
}

void VectorOperation()
{
	std::vector<int>v = {1,2,3,4,5,6,7,8,9};
	printVector(v);

	std::cout<<"v.begin() = ";	// it returns an iterator pointing to the first element of the vector. index 0.
	std::vector<int>::iterator it = v.begin(); // vector where indices start at 0.

	std::cout<<*it<<" = ";

	auto it1 = v.begin();
	std::cout<<*it1<<"\n";

	std::cout<<"v.end() = "; 
	auto it2 = v.end() ;	// it returns an iterator pointing to the element after the last element of the vector.
	std::cout<<*it2<<"\n";

	std::cout<<"v.end() -1 = ";
	auto it3 = v.end() -1;
	std::cout<<*it3<<"\n";

	std::cout<<"v.push_back(55) = \n";
	v.push_back(55);	// insert the element at the end.
	printVector(v);

	std::cout<<"v.push_back(77) = \n";
	v.push_back(77);
	printVector(v);

	auto it4 = v.begin()+4; //vector index start form 0 just like array. so it like 0 + 4 th position.
	std::cout<<"v.insert(it4,0) = \n";
	v.insert(it4,0);	//it is used to insert an element at a specified position. insert 0 at iterator it4 position;
	printVector(v);

	auto it5 = v.end();
	std::cout<<"v.insert(it5,34) = \n";
	v.insert(it5,34);	//it insert element after last element position.
	printVector(v);

	auto it6 = v.begin();
	std::cout<<"v.erase(it6) = \n";
	v.erase(it6);		//it delete a specific element. 
	printVector(v);

	std::cout<<"v.erase(v.begin()+6) = \n";
	v.erase(v.begin()+6);	
	printVector(v);

	v.pop_back();
	std::cout<<"v.pop_back() = \n"; //it deletes the last element.
					//std::cout<<v.pop_back();//it returns void not last element, it will cause a compilation error because you cannot print void.
	printVector(v);

	std::cout<<"v.front() = "<<v.front()<<"\n";
	std::cout<<"v.back() = "<<v.back()<<"\n";
	std::cout<<"&(*v.begin()) = "<<&(*v.begin())<<"\n";	// print memory address of 1st element
	std::cout<<"*v.begin() = "<<*v.begin()<<"\n";	// print 1st element
	std::cout<<"*v.end() = "<<*v.end()<<"\n"; 	
	std::cout<<"*(v.end() -1) = "<<*(v.end() -1)<<"\n"; 	// print last element

	//	std::cout<<"v.begin() = "<<v.begin()<<"\n"; // it return std::vector<int>::iterator can't cout/print on ostream
	//v.begin() returns a std::vector<int>::iterator – a class type that does not have an overload for operator<<, so the compiler will complain (no matching operator).
	//	std::cout<<"v.end() = "<<v.end()<<"\n"; // it return std::vector<int>::iterator can't cout/print on ostream

	std::cout<<"size = "<<(v.end() - v.begin())<<"\n";

	std::cout<<"v.size() = "<<v.size()<<"\n";	//returns the size of the vector

	std::cout<<"v.empty() = "<<(v.empty() ? "true" : "false")<<"\n";
	printVector(v);

	std::cout<<"v.clear() = \n";
	v.clear();
	printVector(v);

	std::cout<<"v.empty() = "<<(v.empty() ? "true" : "false")<<"\n";
}

void Modifiers()
{
	std::cout<<"1. Construction and push_back \n";
	std::vector<int> v;
	std::cout<<"initialize empty vector \n";
	printVector(v);
	
	v.push_back(10); // copy/add element at the end.
	v.push_back(20);
	v.push_back(30);
	std::cout<<"push_back(10), 20, 30 \n";
	printVector(v);

	std::cout<<"push_back(std::move(40)) \n";
	v.push_back(std::move(40));
	printVector(v);

	std::cout<<"2. pop_back \n";
	v.pop_back(); // remove last element
	printVector(v);
	
	std::cout<<"3. insert single element \n";
	auto it1 = v.begin() + 1 ; // point to 2nd element 
	std::cout<<"it1 = begin() + 1 position of v = "<<*it1<<"\n";
	auto insert_at = v.insert(it1,60);
	std::cout<<"Return iterator to value : "<<*insert_at<<" gonna insert at position of it1 : "<<*it1<<" \n";
	printVector(v);

	std::cout<<"4. insert count , copies of element\n";
	auto it2 = v.begin() + 2;
	std::cout<<"it2 = v.begin() + 2 position of v = "<<*it2<<"\n";
	auto insert_at1 = v.insert(it2 , 3, 7); // insert 3 copies of 7 before it2. // i.e put 7 at it2 position and move element of it2 next.
	std::cout<<"Return iterator to value : "<<*insert_at1<<"\n";
	printVector(v);
	
	std::cout<<"5. insert range \n";
	std::vector<int> other = {100,200,300,400};
	printVector(other);
	
	auto it3 = v.end(); // insert at the end.
	v.insert(it3,other.begin(),other.end());
	printVector(v);

	std::cout<<"6. insert initializer list\n";
	auto it4 = v.begin(); 
	v.insert(it4,{30,40,50,60});
	std::cout<<"insert 30, 40, 50, 60 at begin initialize list\n";
	printVector(v);
	v.insert(v.end(),{500,600});
	std::cout<<"insert 500,600 at end initialize list\n";
	printVector(v);

	std::cout<<"7. emplace (construct in-place)\n";
	v.emplace(v.begin(),888); // constructs int(888) directly in place at v.begin() place.
	printVector(v);

	std::cout<<"8. emplace_back \n";
	v.emplace_back(999); // in place at end 999 contruction at end.
	printVector(v);

	std::cout<<"9. erase (single element) \n";
	auto it6 = v.begin() + 2;
	std::cout<<"v.erase(v.begin() + 2) = "<<*(v.begin() + 2)<<"\n";
	printVector(v);
	auto erase_it = v.erase(it6);
	std::cout<<"erase third elements "<<*erase_it<<"\n";
	printVector(v);
	std::cout<<"Returned iterator pointing to "<<*it6<<"  next element\n";
	
	
	std::cout<<"10. erase (range) \n";
	printVector(v);
	v.erase(v.begin() +1 , v.begin() +4); // erase begin() + 1 to upto +4 not +4 element.
	std::cout<<"v.erase(v.begin() +1 , v.begin() +4) = \n";
	printVector(v);

	std::cout<<"11. clear\n";
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";
//	v.clear();
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";

	std::cout<<"12. v.resize(24)  \n"; //resize(n) → changes the size of the vector to n.
	printVector(v);			// If n is bigger, it adds new elements (initialized with default values).
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";
	v.resize(4);			// If n is smaller, it removes elements from the end.
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";

	std::cout<<"12. v.resize(27,8)  \n";	//resize() can reduce size
	v.resize(27,8);
	printVector(v);
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";

	std::cout<<"13. v.reserve(30)\n"; // reserve(n) → changes the capacity (allocated memory) to at least n.
	v.clear();				// It does not add or remove elements.
	printVector(v);				// It just pre-allocates memory to avoid future re‑allocations. so make v.capacity() >= v.reserve() i.e memory reserved.
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";	
	v.reserve(30);			//reserve() never shrinks capacity
	printVector(v);			//reserve() does not initialize elements so std::cout<<v[30]; -> ❌ Undefined behaviour – element doesn't exist yet.
	std::cout<<"v.size() = "<<v.size()<<" v.capacity() = "<<v.capacity()<<"\n";

	std::cout<<"14. v.assign(range)\n";
	std::vector<int>  another = {1,2,3,4,5,6,7,8};
	v.assign(another.begin(),another.end());
	std::cout<<"v.assign(another.begin(),another.end())\n";
	printVector(v);

	std::cout<<"15. assign (initializer_list) \n";
	v.assign({10,20,30,40});
	printVector(v);

	std::cout<<"16. swap \n";
	std::vector<int>w = {100,200,300,400,500};
	std::cout<<"v = ";
	printVector(v);
	std::cout<<"w = ";
	printVector(w);
	v.swap(w);
	std::cout<<"v = ";
	printVector(v);
	std::cout<<"w = ";
	printVector(w);
	



}

void combine ()
{
//	std::vector<int> v = ({5,6,7}); // error: conversion from ‘int’ to non-scalar type ‘std::vector<int>’ requested
	std::vector<int> v = {(5,6,7)}; // o/p ->  [7]
	printVector(v);
	
}
int main()
{
	std::vector<int>v = {1,2,3,4,5};
	std::vector<int>w(10,7);

//	std::cout<<"1. Vector Ctor and Assignment \n";
//	Ctor();
//	std::cout<<"\n";

//	std::cout<<"2. Vector Capacity \n";
//	Capacity();

	VectorOperation();

//	std::cout<<"Element Access \n";
//	ElementAccess();

//	std::cout<<"Modifiers (Insert / Erase / Push / Pop) \n";
//	Modifiers();

//	combine();

	return 0;
}

/*

std::vector is a dynamic array that provides random access, fast insertion/removal at the end, and amortised O(1) push/pop.


*/

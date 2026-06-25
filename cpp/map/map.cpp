// in map complexity is O(log n)
#include <iostream>
#include <map>

void printMap(const std::map<int, std::string> &m)
{
	for(const auto & entry: m)
	{
		std::cout<<entry.first<<"|"<<entry.second<<"\n";
	}
}

int main()
{

	std::map<int, std::string> m;
	//Initialize map with list
	std::cout<<"Initialize map with list\n";
	m={{1,"My"},{2,"Name"},{3,"is"},{4,"khan !!"}};

	printMap(m);
	std::cout<<"\n";

	// Inserting a key value pair
	std::cout<<"Inserting a key value pair -> time complexity = O(log n)\n";
	m.insert({5,"to"}); // time complexity = O(log n)
	printMap(m);
	std::cout<<"\n";


	m.insert({6,"not"});
	printMap(m);
	std::cout<<"\n";

	// Accessing elements
	std::cout<<"Accessing elements -> time complexity = O(log n)\n"; 
	std::cout<<m[2]<<"\n";
	std::cout<<m.at(4)<<"\n";
	std::cout<<"\n";

	// Updating value
	std::cout<<"Updating value -> time complexity = O(log n)\n";
	m[4]="Rushi";
	printMap(m);
	std::cout<<"\n";


	m.at(5)="you";
	printMap(m);
	std::cout<<"\n";


	std::cout<<"Finding elements by key\n";
	auto it = m.find(6);
	if (it == m.end())
	{
		std::cout<<"element not found\n";
	}
	else
	{
		std::cout<<"element found -> \t";
		std::cout<<it->first<<"|"<<it->second<<"\n";
	}


	std::cout<<"Traversing by iterator\n";
	auto it1 = m.end();
	it1--;
	std::cout<<it1->first<<"|"<<it1->second<<"\n";


	auto it2 = m.begin();
	std::cout<<it2->first<<"|"<<it2->second<<"\n";

	std::cout<<"Deleting by key m.erase(2)\n";
	m.erase(2);
	printMap(m);
	std::cout<<"\n";

	std::cout<<"Deleting by iterator m.erase(m.begin())\n";
	m.erase(m.begin());
	printMap(m);
	std::cout<<"\n";

	std::cout<<"m.at(1) handle out of range exception\n";
	try
	{
		std::cout<<"m.at(1) = "<<m.at(1)<<"\n";
	}	
	catch ( const std::exception &e)
	{
		std::cerr<<"exception occur = "	<<e.what()<<"\n";
	}
	std::cout<<"\n";
	std::cout<<"m[1] by index square bracket operator\n";
	std::cout<<"m[1] = "<<m[1]<<"\n";

	std::cout<<"\n";
	std::cout<<"m.count = '"<<m.count(6)<<"'\n";
	return 0;
}







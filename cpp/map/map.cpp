
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

	
	return 0;
}





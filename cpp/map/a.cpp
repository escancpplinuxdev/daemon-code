#include <iostream>
#include <map>
#include <string>
#include <utility> // for std::pair

using namespace std;

int main()
{
	// 1. Declare the map (key = pair<string,string>, value = string)
	map<pair<string, string>,string> myMap;

	// 2. Declare an iterator 
	map<pair<string,string>, string>::iterator itPkgList;

	cout<< " == insertion Operations == "<<endl;

	// 2a. Inset using operator[]
	myMap[{"Alice", "Bob"}] = "Friends";
	myMap[{"Charlie","Dave"}] = "Colleagues";

	// 2b. Insert using insert() with make_pair
	myMap.insert(make_pair(make_pair("Eve","Frank"),"Partners"));

	// 2c. Insert using emplace (c++11, ctor in place)
	myMap.emplace(pair<string,string>("Grace","Heidi"),"Sisters");

	// 2d. Insert using insert with initializer list
	myMap.insert({{"Ivan","Judy"},"Neighbors"});

	cout<<"Current map size: "<<myMap.size()<<endl;

	// 3.Display all entries (iteration)
	cout<<"\n == Iteration (using iterator) ==\n";
	for(itPkgList = myMap.begin();itPkgList != myMap.end();++itPkgList)
	{
		const auto& key = itPkgList->first;
		const auto& value = itPkgList->second;
		cout<<"Key:("<<key.first<<", "<<key.second<<")-> Value: "<<value<<endl;
	}

	// 3b. Range-based for loop (C++11)
	cout<<"\n == Iteration (range based) ==="<<endl;
	for(const auto& entry: myMap)
	{
		cout<<"("<<entry.first.first<<", "<<entry.first.second<<")=>"<<entry.second<<endl;
	}

	// 4. Access values
	cout<<"\n === Access Operation ===\n";
	// using operator[] {inserts default if missing}
	cout<<"myMap[{Alice,Bob}] = "<<myMap[{"Alice","Bob"}]<<endl;
	try
	{
		cout<<"myMap.at({Charlie, Dave}) = "<<myMap.at({"Charlie","Dave"})<<endl;
	}
	catch(const out_of_ranges& e)
	{
		cout<<"Key not found \n";
	}
	// 5. lookup (find)

	cout<<"\n === Lookup (find) ==="<<endl;
	piar<string,string>searchKey = {"Eve","Frank"};
	itPkgList = myMap.find(searchKey);
	if(itPkgList != myMap.end())
	{
		cout<<"Found: "<<itPkgList->second<<endl;
	}
	else
	{
		cout<<"Key not found\n";
	}

	return 0;	
}	

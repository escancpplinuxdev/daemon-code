
#include <iostream>
#include <unordered_map>
using namespace std;

void print(unordered_map<int,string>um)
{	cout<<"{";
	
	for(auto it = um.begin();it != um.end();it++)
	{
		cout<<it->first<<":"<<it->second;
		if(next(it) != um.end())
		{
			cout<<",\n";
		}
	}
	cout<<"}\n";
}

int main()
{
	// create empty unordered_map	-> default initialization
	unordered_map<int,string>um1;

	unordered_map<int ,string>um2={{1,"Geeks"},
				      {2,"For"},
				      {3,"C++"}};
	cout<<"um1 = ";
	print(um1);
	cout<<"\n";
	cout<<"um2 = ";
	print(um2);
	cout<<"\n";
	
	um2[2]="by";
	cout<<"um2 = ";
	print(um2);
	cout<<"\n";

	um2.at(2)="touy";
	cout<<"um2 = ";
	print(um2);
	cout<<"\n";

	auto it = um2.find(3);

	cout<<"find = '"<<it->first<<"' : '"<<it->second<<"'\n\n";

	um2.erase(2);
	cout<<"um2 = ";
	print(um2);
	cout<<"\n";

	return 0;
}

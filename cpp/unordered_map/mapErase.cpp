
#include <iostream>
#include <unordered_map>
using namespace std;

void print(unordered_map<int,string>&um)
{
	for(auto it=um.begin();it!=um.end();it++)
	{
		cout<<it->first<<" : "<<it->second;
		if(it!=um.end())
		{
			cout<<",\n";
		}
		else
		{
			cout<<"\n";
		}
	}
}

int main()
{
	unordered_map<int,string>um={{1,"to"},
		{2,"you"},
		{3,"for"},
		{4,"by"},
		{5,"go"}
	};


	print(um);
	um.erase(4); //erase by key
	cout<<"um.erase(4) = \n";
	print(um);

	cout<<"eraseBYiterator = \n";
//	um.erase()

	print(um);

	return 0;
}


#include <iostream>
#include <vector>
#include <string>
//#include <algorithm>
using namespace std;

class Employee
{
	public:
		int id;
		string name;

		Employee(int i, string n)
		{
			id = i;
			name = n;
		}

		void display() const
		{
			cout<<" iD = "<<id<<" Name = "<<name<<endl;
		}
};

void printVector(const vector<int>& v, string message)
{
	cout<<"\n"<<message<<endl;

	cout<<"size	="<<v.size()<<endl;
	cout<<"capacity	="<<v.capacity()<<endl;
	
	cout<<"Elements = ";

	for(int i = 0; i<v.size();i++)
	{
		cout<<v[i]<<" ";
	}
	cout<<endl;
}
int main()
{
	cout<< "------------- Vector interview all program demo -----------\n";
	cout<<"1. vector declaration : \n";

	vector<int>v;

	cout<<"\nInitial Vector \n";

	cout<<"Size	= "<<v.size()<<endl;
	cout<<"Capacity	= "<<v.capacity()<<endl;

	cout<<"\n Push back \n";

	for(int i =1; i<=10;i++)
	{
		v.push_back(i*10);

		cout<<"Inserted : "
			<<i * 10
			<<" size = "
			<<v.size()
			<<" Capacity = "
			<<v.capacity()
			<<endl;
	}

	printVector(v, "After push_back()");

	cout<<"\nRandom Access \n";

	cout<<"v[0]	= "<<v[0]<<endl;
	cout<<"v.at(1)	= "<<v.at(1)<<endl;
	cout<<"front()	= "<<v.front()<<endl;
	cout<<"back()	= "<<v.back()<<endl;
	
	cout<<"\nIterator demo \n";

	for(vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
//cout<< it <<" ";//error: no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘std::vector<int>::iterator’)
		cout<< *it <<" ";
	}

	cout<<endl;
	
	cout<<"\nRange based loop \n";
	cout<<"by v[i] = \n";
	for(int i = 0; i<v.size(); i++)	
	{
	cout<<"v["<<i<<"] = "<<v[i]<<"\n";
	}

	cout<<"\nby const auto &x \n";
	for(const auto & x : v)
	{
		cout<<x<<" ";
	}
	cout<<endl;
	cout<<endl;
	
	cout<<"by v.at(i) = \n";
	for(int i = 0; i<v.size(); i++)	
	{
	cout<<"v["<<i<<"] = "<<v.at(i)<<"\n";
	}


	cout<<"Insert demo \n";

	v.insert(v.begin() + 2, 99);
	printVector(v,"After insert at position 2");

	v.insert(v.begin() , 15);
	printVector(v,"After insert at position v.begin()");

	v.insert(v.end() , 27);
	printVector(v,"After insert at position v.end()");
	for(int i=0;i<5;i++)
	{
	
	v.insert(v.end() , 27+i);
	printVector(v,"After insert at position v.end()");
	
	}

	v.insert(v.end() -1, 23);
	printVector(v,"After insert at position v.end() -1");

	cout<<"Erase demo\n";

	v.erase(v.begin());
	printVector(v,"After erase at v.begin()");







	return 0;
}


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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



	cout<<"\npop_back demo\n";

	v.pop_back();
	printVector(v,"After pop_back()");
	v.pop_back();
	printVector(v,"After pop_back()");
	v.pop_back();
	printVector(v,"After pop_back()");
	v.pop_back();
	printVector(v,"After pop_back()");
	v.pop_back();
	printVector(v,"After pop_back()");


	cout<<"\nreserve demo\n";

	vector <int> reserveVector;

	for(int i =1; i<16; i++)
	{
		reserveVector.push_back(i);
	}

	printVector(reserveVector,"Before reserveVector.reserve(10)");
	reserveVector.reserve(10);

	cout<<"Ater reserveVector.reserve(10) Reserved capacity = "<<reserveVector.capacity()<<endl;
	printVector(reserveVector,"After reserveVector.reserve(10)");

	cout<<"\nResize demo\n";
	
	reserveVector.resize(10);

	cout<<"size = "<<reserveVector.size()<<endl;
	printVector(reserveVector,"After resize Vector.size()");

	cout<<"\nEmplace_back demo : \n";

	vector<Employee>employees;

	employees.emplace_back(101,"Rushi");
	employees.emplace_back(102,"Amit");
	
	cout<<"employees : \n";

	for(const auto & e : employees)
	{
		e.display();
	}
/*
	for(int i =0; i<employees.size();i++)
	{
		cout<<" "<<employees[i][i];
	}
*/

	cout<<"sort demo\n";
	
	vector<int>sortVector =
	{
		55,
		11,
		96,
		99,
		22,
		60

	};

	printVector(sortVector,"sortVector = ");
	sort(sortVector.begin(), sortVector.end());
	
	printVector(sortVector,"sortVector = ");
	for(const auto & x : sortVector)
	{
		cout<< x <<" ";
	}
	cout<<endl;

	for(int i = 0;i<sortVector.size();i++)
	{
		cout<<sortVector[i]<<" ";
	}
	cout<<endl;

	cout<<"\nReverse demo\n";

	reverse(sortVector.begin(),sortVector.end());

	for(const auto &x : sortVector)
	{
		cout<< x <<" ";
	}

	printVector(sortVector,"Reverse sortVector");

	cout<<endl;

	cout<<"\nVector<string>demo"<<endl;

	vector<string>names;

	names.push_back("rushi");
	names.push_back("ayushi");
	names.push_back("mahhi");
	names.push_back("rdesmoushi");
	printVector(sortVector,"Stirng  names vector Vector");
	reverse(sortVector.begin(),sortVector.end());

	for(int i=0;i<names.size();i++)
	{
		cout<<names[i]<<" ";
	}
	cout<<endl;

	cout<<"\nPointer invalidation demo\n";

	vector<int>pointerVector;

	pointerVector.push_back(10);
	pointerVector.push_back(20);
	pointerVector.push_back(30);
	pointerVector.push_back(40);


	int *ptr = &pointerVector[0];

	cout<<"Before REallocation Address = "<<ptr<<endl;

	for(int i = 0; i<10;i++)
	{
		pointerVector.push_back(i);
	}
	
	cout<<"After Reallocation Address = "<<&pointerVector[0]<<endl;
	cout<<"old allocated Address =	 "<<ptr<<endl;

	cout<<"\nOld pointer may become dangling!"<<endl;


	return 0;
}

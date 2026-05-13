
#include <iostream>
#include <vector>
using namespace std;


int main()
{

	std::vector <int>v;
	
	int count;
	cout<<"Enter the size of vector : ";
	cin>>count;
	v.reserve(count);
	

	for(int i=0; i<count;i++)
	{
		cout<<"num ["<<i<<"] = "<<v[i]<<endl;
	}
	cout<<endl;
	for (int i =0; i< count; i++)
	{
		v.emplace_back(i);
	}
	for(int i=0; i<count;i++)
	{
		cout<<"num ["<<i<<"] = "<<v[i]<<endl;
	}
	cout<<endl;

	v.push_back(2);
	for(int i=0; i<count;i++)
	{
		cout<<"num ["<<i<<"] = "<<v[i]<<endl;
	}


	return 0;
}

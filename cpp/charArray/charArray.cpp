
#include <iostream>
using namespace std;


int main()
{
	int size;
	
	cout<<"enter size of array\n";
	cin>>size;
	char charr[size];
	cout<<"enter elements\n";
	for(int i =0; i<size;i++)
	{
		cin>>charr[i];
	}

	cout<<"print charr["<<size<<"] = ";
	for(int i = 0; i<size;i++)
	{
		cout<<charr[i]<<"";
	}
	cout<<"\n";
	return 0;
}

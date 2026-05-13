
#include <iostream>
#include <vector>
//#include <string>
//#include <algorithm>

using namespace std;


int main()
{
	int count;
	cout<<"Enter tolal numbers of element to addition : ";

	cin>>count;
	
//	vector numbers <string>; // compiler sees: 'vector numbers' as a variable declaration then <string> becomes invalid;
//error: expected initializer before ‘<’ token			
				 
	std::vector <string>	numbers; // container <data_type> variables_name;

	cout<<"Enter number of elements : \n";
	for(int i =0 ; i<count; i++)
	{
		string num;
		cout<<"num ["<<i<<"] = ";
		cin>>num;
		numbers.push_back(num);
	}

	for(int i =0; i<numbers.size();i++)	
	{
		cout<<"num ["<<i<<"] = ";
		cout<<numbers[i]<<endl;
	}

	for(const string & num: numbers)  // take each string from vector // store reference in num // avoid copying string
	{
		cout<<num<<endl;
	}

	for(int k =0;k<numbers.size();k++) //equivalent normal loop
	{
		string & i = numbers[k];
		cout<<"num ["<<k<<"] = "<<i<<endl;
	}
	return 0;
}

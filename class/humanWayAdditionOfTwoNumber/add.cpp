
#include <iostream>
using namespace std;


int main()
{

	int count;
	vector<string> numbers;

	cout<<"Enter total number of addition elements : ";
	cin>>count;
	cout<<"Enter number for element \n";

	for(int i=0; i<count; i++)
	{
		string num;
		cout<<"num["<<i+1<<"]";
		cin>>num;
		numbers.push_back(num);
	}

	string sum="0";

	for(int j=0;j<count;j++)
	{
		string currentNum = numbers[j];
		string a = sum;
		string b = currentNum;

		// making both string sam length by padding with leading zeros

		int maxLen = max(a.length(), b.length());
		while(a.length() < maxlen)
		{
			a = "0" + a;  // a.length increment operation
		}
		while(b.length() < maxLen)
		{
			b = "0" + b; // b.length increment operation
		}

		string result = "";



	}

	return 0;
}

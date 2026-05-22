
#include <iostream>
#include <vector>
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
		cout<<"num["<<i+1<<"] = ";
		cin>>num;
		numbers.push_back(num);
	}

	// Start with sum = "0"
	string sum="0";

	for(int j=0;j<count;j++)
	{
		string currentNum = numbers[j];
		string a = sum;
		string b = currentNum;

		// making both strings same length by padding with leading zeros

		int maxLen;
		if (a.length() > b.length())
		{
			maxLen=a.length();
		}
		else
		{
			maxLen=b.length();
		}
		while(a.length() < maxLen)
		{
			a = "0" + a;  // a.length increment in this "0" + a  operation
		}
		while(b.length() < maxLen)
		{
			b = "0" + b; // b.length increment in this "0" + b  operation
		}

		cout<<"maxLen = "<<maxLen<<endl;
		cout<<"a = "<<a<<endl;
		cout<<"a.length() = "<<a.length()<<endl;
		cout<<"b = "<<b<<endl;
		cout<<"b.length() = "<<b.length()<<endl;

		string result = "";
		int carry = 0;

		cout<<"Adding = "<<currentNum<<endl;
		cout<<"Current sum = "<<sum<<endl;

		for(int i = a.length() - 1; i>=0; i--)
		{
			cout<<"i = "<<i<<endl;
			int digitA = a[i] - '0';
			int digitB = b[i] - '0';
			int total = digitA + digitB + carry;
			cout<<"total = digitA + digitB + carry"<<endl;
			cout<<total <<" = "<<digitA<<" + "<<digitB<<" + "<<carry<<endl;

			//convert total to string
			string totalStr = to_string(total);

			//Get last digit

			cout<<"result = "<<result<<endl;
			cout<<"totalStr = "<<totalStr<<endl;
			cout<<"result = totalStr.back() + result"<<endl;
			cout<<"result = "<<totalStr.back()<<" + "<<result<<endl;
			result = totalStr.back() + result;
			cout<<"result = "<<result<<endl;
			if(totalStr.length() > 1)
			{
				carry = stoi(totalStr.substr(0, totalStr.length() - 1));
				cout<<"carry = "<<carry<<endl;
			}
			else
			{
				carry = 0;
			}


			cout<<"result = "<<result<<endl;
			cout<<"carry = "<<carry<<endl;

		}	
		if( carry > 0)
		{
			cout<<"Here carry > 0 \t ";
			cout<<"result = to_string(carry) + result";
			result = to_string(carry) + result;
		}
		cout<<"result = "<<result<<endl;

		sum = result;

		cout<<"result of adding  = "<<currentNum<<" = "<<sum<<endl;


	}
	cout<<"\n=========================\n";
	cout<<"Final sum = "<<sum<<endl;
	cout<<"\n=========================\n";

	return 0;
}

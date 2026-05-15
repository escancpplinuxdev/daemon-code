
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
	string x = numbers[0];
	for(int j=1;j<count;j++)
	{
		string currentNum = numbers[j];
		string a = sum;
		string b = currentNum;

		// making both string sam length by padding with leading zeros

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
	cout<<"Final sum = "<<sum;
	cout<<"\n=========================\n";

	bool nsign = false;
		if(stoi(x) > stoi(sum))
		{
			
		}
		else
		{
			nsign = true;
			string temp = x;
			x =sum;
			sum=temp;
		}

	int maxLen;
                if (x.length() > sum.length())
                {
                        maxLen=x.length();
                }
                else
                {
                        maxLen=sum.length();
                }
                while(x.length() < maxLen)
                {
                        x = "0" + x;  // a.length increment in this "0" + a  operation
                }
                while(sum.length() < maxLen)
                {
                        sum = "0" + sum; // b.length increment in this "0" + b  operation
                }
	int carry = 0;
	cout<<"x = "<<x<<endl;
	cout<<"sum = "<<sum<<endl;
	cout<<"carry = "<<carry<<endl;
	string result = "";
	for (int i =x.length()-1; i>=0; i-- )
	{
			 cout<<"i = "<<i<<endl;
                        int digitA = x[i] - '0';
                        int digitB = sum[i] - '0';
                        int total = digitA - digitB - carry;
                        cout<<"total = digitA - digitB - carry"<<endl;
                        cout<<total <<" = "<<digitA<<" - "<<digitB<<" - "<<carry<<endl;

			if(total < 0)
			{
				total = 10 + total;
				carry = 1;
			}
			else
			{
				carry = 0;
			}

			cout<<"total = "<<total<<endl;
                        cout<<"carry = "<<carry<<endl;
	
			result = to_string(total) + result;
	}
	if(nsign)
	{
		result = "-"+result;
	}
	cout<<"\n=========================\n";
	cout<<"Final result = "<<result;
	cout<<"\n=========================\n";
	return 0;
}

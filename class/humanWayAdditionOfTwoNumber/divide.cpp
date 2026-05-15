
#include <iostream>
#include <vector>
using namespace std;


int main()
{

	cout<<"Enter number for element :\n";
	string Dividend = "16";
	string Divisor = "3"; 


	cin>>Dividend;
	cin >>Divisor;

	if( Divisor  == "0" )
	{
		cout<<"wrong . Divisior should not be zero.\nundefined\n";
		return 0;
	}




	bool nsign = false;
	if(stoi(Dividend) > stoi(Divisor))
	{

	}
	else
	{
		cout<<"negative\n";
		nsign = true;
		string temp = Dividend;
		Dividend =Divisor;
		Divisor=temp;
	}

	int maxLen;
	if (Dividend.length() > Divisor.length())
	{
		maxLen=Dividend.length();
	}
	else
	{
		maxLen=Divisor.length();
	}
	while(Dividend.length() < maxLen)
	{
		Dividend = "0" + Dividend;  // a.length increment in this "0" + a  operation
	}
	while(Divisor.length() < maxLen)
	{
		Divisor = "0" + Divisor; // b.length increment in this "0" + b  operation
	}
	int count = 0;
	string result = "";
	for(int j =stoi(Dividend);j>=0;j--)
	{
		int carry = 0;
		cout<<"Dividend = "<<Dividend<<endl;
		cout<<"Divisor = "<<Divisor<<endl;
		cout<<"carry = "<<carry<<endl;
		result="";
		for (int i =Dividend.length()-1; i>=0; i-- )
		{
			cout<<"i = "<<i<<endl;
			int digitA = Dividend[i] - '0';
			int digitB = Divisor[i] - '0';
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
		count++;
		if(nsign)
		{
			result = "-"+result;
		}

		cout<<"Result    = "<<result<<endl;
		cout<<"Count     = "<<count<<endl<<endl<<endl;
		if(stoi(result) <= 0)
		{
			cout<<" rresult break;\n\n";
			break;
		}

		cout<<"\n=========================\n";
		cout<<"Final result = "<<result<<endl;
		cout<<"\n=========================\n";
		Dividend=result;
		if(stoi(Dividend) < stoi(Divisor))
		{
			break;
		}
	}
	cout<<"\n=========================\n";
	cout<<"Quotient     = "<<count<<endl;
	cout<<"Remainder    = "<<result<<endl;
	cout<<"==========================\n";
	return 0;
}

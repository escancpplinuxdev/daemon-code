
#include <iostream>
#include <vector>
using namespace std;

void callConvertFractionToDecimal(string &Dividend, string Divisior, string &q,string &r)
{
	cout <<"callConvertFractionToDecimal \n";
	bool nsign = false;
	int count = 0;
	string result = "";
	for(int j =stoi(Dividend);j>=0;j--)
	{
		int carry = 0;
		cout<<"Dividend = "<<Dividend<<endl;
		cout<<"Divisior = "<<Divisior<<endl;
		cout<<"carry = "<<carry<<endl;
		result="";

		int maxLen;
		if (Dividend.length() > Divisior.length())
		{
			maxLen=Dividend.length();
		}
		else
		{
			maxLen=Divisior.length();
		}
		while(Dividend.length() < maxLen)
		{
			Dividend = "0" + Dividend;  // a.length increment in this "0" + a  operation
		}
		while(Divisior.length() < maxLen)
		{
			Divisior = "0" + Divisior; // b.length increment in this "0" + b  operation
		}

		cout<<"Dividend.length() : "<<Dividend.length()<<endl;
		for (int i =Dividend.length()-1; i>=0; i-- )
		{
			cout<<"i = "<<i<<endl;
			cout<<"Dividend = "<<Dividend<<endl;
			int digitA = Dividend[i] - '0';
			int digitB = Divisior[i] - '0';
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
		if(stoi(Dividend) < stoi(Divisior))
		{
			break;
		}
	}
	Dividend = result ;
	q = q + to_string(count);
	r=result;
	cout<<"\n=========================\n";
	cout<<"Quotient     = "<<count<<endl;
	cout<<"Remainder    = "<<result<<endl;
	cout<<"q = "<<q<<endl;
	cout<<"==========================\n";


}

int main()
{
	string q = "0.";
	string r = "";
	cout<<"Enter number for element :\n";
	string Dividend = "5";
	string Divisior = "65"; 


	cout<<"Dividend : ";
	cin>>Dividend;
	cout<<"Divisior : ";
	cin >>Divisior;

	if( Divisior  == "0" )
	{
		cout<<"wrong !!.Division by zero '0'. \nDivisior should not be zero.\nundefined\n";
		return 0;
	}




	bool nsign = false;
	if(stoi(Dividend) > stoi(Divisior))
	{

	}
	else
	{	
		for(int i = 0; i<10;i++)
		{
			Dividend = Dividend + "0";
			while(stoi(Dividend) < stoi(Divisior))
			{
				Dividend = Dividend + "0";

				q=q+"0";
			}
			callConvertFractionToDecimal(Dividend,Divisior,q,r);

			cout<<"Dividend = "<<Dividend<<endl;
			cout<<"Divisior = "<<Divisior<<endl;
			cout<<"-------------------------------------------";
			if (i==9)
			{
				cout<<"\n=========================\n";
				cout<<"Final Quotient     = "<<q<<endl;
				cout<<"Final Remainder    = "<<r<<endl;
				cout<<"==========================\n";

								return 0;
			}
			if(stoi(Dividend) == 0 ) 
			{
				cout<<"\n=========================\n";
				cout<<"Final Quotient     = "<<q<<endl;
				cout<<"Final Remainder    = "<<r<<endl;
				cout<<"==========================\n";

								return 0;
			}


		}
		return 0;
	}

	int maxLen;
	if (Dividend.length() > Divisior.length())
	{
		maxLen=Dividend.length();
	}
	else
	{
		maxLen=Divisior.length();
	}
	while(Dividend.length() < maxLen)
	{
		Dividend = "0" + Dividend;  // a.length increment in this "0" + a  operation
	}
	while(Divisior.length() < maxLen)
	{
		Divisior = "0" + Divisior; // b.length increment in this "0" + b  operation
	}
	int count = 0;
	string result = "";
	for(int j =stoi(Dividend);j>=0;j--)
	{
		int carry = 0;
		cout<<"Dividend = "<<Dividend<<endl;
		cout<<"Divisior = "<<Divisior<<endl;
		cout<<"carry = "<<carry<<endl;
		result="";
		for (int i =Dividend.length()-1; i>=0; i-- )
		{
			cout<<"i = "<<i<<endl;
			int digitA = Dividend[i] - '0';
			int digitB = Divisior[i] - '0';
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
		cout<<"count = "<<count<<endl;
		count++;
		cout<<"count++ = "<<count<<endl;
		if(nsign)
		{
			result = "-"+result;
		}

		cout<<"Result    = "<<result<<endl;
		cout<<"Count     = "<<count<<endl<<endl<<endl;
		if(stoi(result) <= 0)
		{
			cout<<"result break;\n\n";
			break;
		}

		cout<<"\n=========================\n";
		cout<<"Final result = "<<result<<endl;
		cout<<"\n=========================\n";
		Dividend=result;
		if(stoi(Dividend) < stoi(Divisior))
		{
			break;
		}
	}
	cout<<"\n=========================\n";
	cout<<"Quotient     = "<<count<<endl;
	cout<<"Remainder    = "<<result<<endl;
	cout<<"==========================\n";
}


#include <cmath> // for round() // show error -> error: ‘round’ was not declared in this scope
//		#include <string>
#include <iostream>
#include <limits>  // for numeric limits // show error -> error: ‘numeric_limits’ was not declared in this scope
using namespace std;

//0 to 19 
string unit[]={"ZERO","ONE","TwO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN","FIFTEEN","SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"};
//20,30,...90
string tens[]={"","","TWENTY","THIRTY","FOURTY","FIFTY","SIXTY","SEVENTY","EIGHTY","NINETY"};

//handle number under 100 -> 0 to 99
//string convertToWords(int n)
string convertToWords(long n)
{
	if(n<20) 
	{	
		return unit[n];
	}
	return tens[n/10] + (n%10 != 0 ? " " + unit[n%10] : "");
}

string belowThousand(int n)
{	
	string result;
	if(n==0) 
	{
		return "";
	}
	if(n>=100)
	{
		result+=convertToWords(n/100)+" HUNDRED";
		n%=100;
		if(n>0)
		{
			result+=" ";
		}
	}
	if(n>=20)
	{
		result+=tens[n/10];
	}
	else if (n>0)
	{
		result+=convertToWords(n);
	}

	return result;
}
//string convertRupees(double n) //error: invalid operands of types ‘double’ and ‘int’ to binary ‘operator%’ --> n%=10000000;
string convertRupees(long long n)
{
	if(n==0) return "ZERO";
	string result;
	if(n>=10000000) // 10^7 crore
	{
//		int crorePart = n / 10000000; // give optput as		->  1000000
//1e+06 : TEN LAKH RUPEES ONLYerror: invalid operands of types ‘double’ and ‘int’ to binary ‘operator%’ --> n%=10000000;
		long long crorePart = n / 10000000;		
		result+=belowThousand(crorePart) + " CRORE";
		n%=10000000; 
		if(n>0)
		{
			result+=" ";
		}
	}		
	if(n>=100000) // 10^5 lakh
	{
		long long lakhPart = n / 100000;
		result+=belowThousand(lakhPart) + " LAKH";
		n%=100000;
		if(n>0)
		{
			result+=" ";
		}
	}
	if(n>=1000) // 10^3 thousand
	{
		long long thousandPart = n / 1000;
		result+=belowThousand(thousandPart) + " THOUSAND";
		n%=1000;
		if(n>0)
		{
			result+=" ";
		}

	}
	if(n>0)
	{
		result+=belowThousand(n);
	}
	return result;
}
string convertPaise(int p)
{
	if(p == 0)
	{
		return "";
	}
	if(p>=1 && p<=19) 
	{	
		return convertToWords(p);
	}
	else	
	{
		return tens[p/10];
	}
}

//string amountToWords(int amount)
string amountToWords(double amount)
{
	if(amount < 0) return "MINUS "+amountToWords(-amount); // (-'-5') -> 5
	if(amount == 0) return "ZERO RUPEES ONLY";

//	int rupees = amount;
	long long rupees =(long long) amount;
	int paise = round((amount - rupees)*100); //error: ‘round’ was not declared in this scope
	if(paise>=100)
	{
		rupees+=paise/100;
		paise=paise%100;
	}

	string result = convertRupees(rupees) + " RUPEE";
	if(rupees>1) result +="S";

	if(paise>0)  result +=" AND " + convertPaise(paise)+" PAISE";


	result+=" ONLY";
	return result;
}


//----------------------------------------------Main loop of Amount to word program--------------------------------------------//
int main()
{
//	int amount; = 5.9 // int doesn't catch .9 so we need double
	double amount	;
	cout<<"Enter Zero (0) to end the loop\n";
	while(true)
	{
	cout<<"Enter amount (e.g.., 1234.56) : \n";
		if(cin>>amount) // if other and int value enter it return as FALSE -> 0
		{
			
		}
		else
		{
			cout<<amount<<" : Invalid amount !! Please Enter numeric  amount \n";
			cin.clear(); // clear error flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n');	     // discard bad input
			//error: ‘numeric_limits’ was not declared in this scope --> add #include<limits>										      
			continue; // if not write continue then program think amount = 0 and print -> ZERO RUPEES ONLY\nExiting loop. Good bye!
		}
		if(amount == 0)
		{
			cout<<amount << " : ZERO RUPEES ONLY\nExiting loop. Good bye!\n";
	//		return 0;
			break;		
		}
		else
		{
			cout<<amount<<" : "<<amountToWords(amount)<<"\n";
		}


	
	}

	return 0;
}

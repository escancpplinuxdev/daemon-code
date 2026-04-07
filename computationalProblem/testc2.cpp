#include <iostream>

using namespace std;


//string forSingleDigit(int n,string *str) string -> void, we modified string by string pointer *str, not return string value
void forSingleDigit(int n,string *str)
{
	//	if(n%20 != 0)  // here i/p -> n =30 30%20 = 10 o/p -> TEN
	if(n>0 && n<20) //This ensures only numbers between 1 and 19 enter this function.
	{

		//		switch(n%20)
		switch(n) // n is already exact number we need i.e. 2,6,7
		{		
			case 1:
				*str +=  "ONE ";
				break;
			case 2:
				*str +=  "TWO ";
				break;
			case 3:
				*str +=  "THREE ";
				break;
			case 4:
				*str +=  "FOUR ";
				break;
			case 5:
				*str +=  "FIVE ";
				break;
			case 6:
				*str +=  "SIX ";
				break;
			case 7:
				*str +=  "SEVEN ";
				break;
			case 8:
				*str +=  "EIGHT ";
				break;
			case 9:
				*str +=  "NINE ";
				break;
			case 10:
				*str +=  "TEN ";
				break;
			case 11:
				*str +=  "ELEVEN ";
				break;
			case 12:
				*str +=  "TWELVE ";
				break;
			case 13:
				*str +=  "THIRTEEN ";
				break;
			case 14:
				*str +=  "FOURTEEN ";
				break;
			case 15:
				*str +=  "FIFTEEN ";
				break;
			case 16:
				*str +=  "SIXTEEN ";
				break;
			case 17:
				*str +=  "SEVENTEEN ";
				break;
			case 18:
				*str +=  "EIGHTEEN ";
				break;
			case 19:
				*str +=  "NINETEEN ";
				break;
		}
	}

	//	return ; //return type is void
}
//string forDoubleDigit(int n,string *str) Enter the number : 55 free(): invalid pointer Aborted (core dumped)
void forDoubleDigit(int n,string *str)
{
	if(n>=20)  //Change while to if. Since you are only dealing with the tens place right now, you only need to run this logic once.
	{
		switch(n/10)
		{		
			case 2:
				*str += "TWENTY ";
				break;
			case 3:
				*str += "THIRTY ";
				break;
			case 4:
				*str += "FOURTY ";
				break;
			case 5:
				*str += "FIFTY ";
				break;
			case 6:
				*str += "SIXTY ";
				break;
			case 7:
				*str += "SEVENTY ";
				break;
			case 8:
				*str += "EIGHTY ";
				break;
			case 9:
				*str += "NINETY ";
				break;
				/*			case 10:
				 *str += "HUNDRED ";
				 break;
				 */
		}
	}
}
// converts number less than 1000 i.e(0 to 999)
void convertBelowThousand(int n, string *str)
{
	if(n==0) return;

	if(n>=100)
	{
		forSingleDigit(n/100,str);
		*str+="HUNDRED ";

		n=n%100;
	}

	if(n>=20)
	{
		forDoubleDigit(n,str);

		n=n%10;
	}

	if(n>0)
	{
		forSingleDigit(n,str);
	}

}
//string numberToWord(int n) 
string numberToWord(long long n) //use long long upto 1 LAKHS CRORES
{
	string str = "";

	if (n == 0 )
	{
		return "ZERO";
	}

	if(n>10000000) // CRORE 10^7
	{
		long long corePart = n / 10000000;
		convertBelowThousand(corePart,&str);

		str+="CRORE ";
		n=n%10000000;

	}

	if(n>100000) // LAKH 10^5
	{
		long long lakhPart = n / 100000;

		convertBelowThousand(lakhPart,&str);

		str+="LAKH ";
		n=n%100000;

	}

	//	if(n>1000) // THOUSAND 10^3 // 1000 -> TEN HUNDRED
	if(n>=1000) // THOUSAND 10^3
	{
		long long thousandPart = n / 1000;

		convertBelowThousand(thousandPart,&str);

		str+="THOUSAND ";
		n=n%1000;

	}


	if(n>0)
	{
		convertBelowThousand(n,&str);
	}
	/*

	if(n>=20) // number form 20 to 99
	{
	forDoubleDigit(n,&str);
	int r=n%10;  // if i put n=n%10 ; the n get garbage value that's why we use r=n%10;
	if(n>0)
	{
	forSingleDigit(r,&str); // 21 
	}
	}
	else
	{

	forSingleDigit(n,&str); // 1 to 19

	}
	*/

	return str;
}

string decidePositiveYaNegative(long long n)
{
	if(n==0)	return "ZERO";

	if(n>0)
	{
		return numberToWord(n); // handle positive number
	}

	if(n<0)
	{
//		return ("MINUS " + numberToWord(n)); // here calling goes as n = -9  
//		The error occurs because when n is negative, numberToWord(n) calls decidePositiveYaNegative(n) again 
//		(or recursively calls itself if numberToWord contains the sign logic).
//		This leads to infinite recursion and eventually a stack overflow.

//		n is negative, so numberToWord will again call decidePositiveYaNegative with same negative n		
		return ("MINUS " + numberToWord(-n));  // here callong goes as n = -(-9) = 9  // pass positive +ve value
// 		Convert absolute value to words and prepend "MINUS"
	}
}
int main()
{
	//	int n=0;
	long long n=0;
	cout<<"Enter -1 to close the loop\n";
	while (n != -1)
	{
		cout<<"Enter the number : ";

		cin>>n;
		if(n == -1)
		{
			break;
		}
/*
		if(n<0 ||  n> 100000000) // n<0 -> n should not be negative
		{
			cout<<"Number is out of range\n";
			continue;
		}
*/
		//		cout<<"In the words 	 : "<<numberToWord(n)<<endl;
		cout<<"In the words 	 : "<<decidePositiveYaNegative(n)<<endl;
		continue;
	}
	return 0;
}

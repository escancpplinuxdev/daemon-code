#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;

string convertRupees(long long n);

// Convert 1 to 19
string oneToNineteen(int n) 
{
			//     0   1      2      3        4       5      6      7       8        9       10 
	const char* words[] = {"", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN","EIGHT", "NINE", "TEN",
	                 	"ELEVEN", "TWELVE", "THIRTEEN","FOURTEEN", "FIFTEEN", "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN"};
	if (n >= 1 && n <= 19)
	{
	cout<<__LINE__<<"  [n]  ->  "<<n<<endl;
	cout<<__LINE__<<"  words[n]  ->  "<<words[n]<<endl;
		return words[n];
	}
	return "";
}

// Convert tens (20-90)
string tens(int n) 
{				// 0   1   2         3         4        5       6        7          8         9  
	const char* tensWords[] = {"", "", "TWENTY", "THIRTY", "FORTY", "FIFTY","SIXTY", "SEVENTY", "EIGHTY", "NINETY"};
	if (n >= 20 && n <= 99) 
	{
		int t = n / 10;
		int r = n % 10;
		if (r == 0)
		{
	cout<<__LINE__<<"  [r]  ->  "<<r<<endl;
	cout<<__LINE__<<"  [t]  ->  "<<t<<endl;
	cout<<__LINE__<<" tensWords[t]  ->  "<<tensWords[t]<<endl;
			return tensWords[t];
		}
		else
		{
	cout<<__LINE__<<"  [r]  ->  "<<r<<endl;
	cout<<__LINE__<<"  [t]  ->  "<<t<<endl;
	cout<<__LINE__<<" tensWords[t]  ->  "<<tensWords[t]<<endl;
			return string(tensWords[t]) + " " + oneToNineteen(r);
		}
	}
	return "";
}

// Convert 1 to 999
string belowThousand(int n) 
{
	if (n == 0) return "";
	string result;
	if (n >= 100) 
	{
		result += oneToNineteen(n / 100) + " HUNDRED";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		n %= 100;
		if (n > 0)
		{
			result += " ";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		}
	}
	if (n >= 20) 
	{
		{
			result += tens(n);
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		}
	}
       	else if (n > 0) 
	{
		{
			result += oneToNineteen(n);
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		}
	}
	return result;
}

// Handles numbers >= 10^7 by recursively extracting crore groups
string aboveCrores(long long n) 
{
    if (n < 10000000)
        return convertRupees(n);   // FIXED: use convertRupees for < 1 crore
    long long crorePart = n / 10000000;
    long long remainder = n % 10000000;
    string result = aboveCrores(crorePart) + " CRORE";
    if (remainder > 0) 
    {
        result += " " + convertRupees(remainder);
    }
    return result;
}



// Convert integer part (rupees) using Indian numbering
string convertRupees(long long n) 
{
	if (n == 0) return "59  ZERO";
	string result;

	// Crore (10^7)
/*
	if (n >= 10000000)
       	{
		long long crorePart = n / 10000000;
		result += belowThousand(crorePart) + " CRORE";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		n %= 10000000;
		if (n > 0) 
		{
			result += " ";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		}
	}

*/

	    // Crore (10^7) – use aboveCrores to handle any number of crore groups
    if (n >= 10000000) 
    {
        long long crorePart = n / 10000000;
        result += aboveCrores(crorePart) + " CRORE";
        n %= 10000000;
        if (n > 0) result += " ";
    }

	// Lakh (10^5)
	if (n >= 100000) 
	{
		long long lakhPart = n / 100000;
		result += belowThousand(lakhPart) + " LAKH";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		n %= 100000;
		if (n > 0) 
		{
			result += " ";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		}
	}
	// Thousand (10^3)
	if (n >= 1000) 
	{
		long long thousandPart = n / 1000;
		result += belowThousand(thousandPart) + " THOUSAND";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		n %= 1000;
		if (n > 0) 
		{
			result += " ";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
		}
	}
	// Remaining (hundreds and below)
	if (n > 0)
       	{
		result += belowThousand(n);
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
	}
	return result;
}

// Convert paise (0-99)
string convertPaise(int p) 
{
	if (p == 0) return "";
	if (p >= 1 && p <= 19) return oneToNineteen(p);
	else return tens(p);
}

// Main function: amount to words
string amountToWords(double amount) 
{
	if (amount < 0) return "MINUS " + amountToWords(-amount);
	if (amount == 0) return "105 Rushi ->  ZERO RUPEE ONLY";

	// Separate integer and fractional parts
	long long rupees = (long long)amount;
	int paise = round((amount - rupees) * 100);
	cout<<"\n"<<__LINE__<<"  amount  ->  "<<amount<<endl;
	cout<<__LINE__<<"  rupees  ->  "<<rupees<<endl;
	cout<<"round((amount-rupees))  ->  "<<round(amount-rupees)<<endl;
	cout<<"round((amount-rupees)*100)  ->  "<<round((amount-rupees)*100)<<endl;
	cout<<__LINE__<<"  paise  ->  "<<paise<<endl;
	// Fix floating point precision
	if (paise >= 100) 
	{
		rupees += paise / 100;
		paise = paise % 100;
	cout<<__LINE__<<"  rupees  ->  "<<rupees<<endl;
	cout<<__LINE__<<"  paise  ->  "<<paise<<endl;
	}

	string result;
	// Rupees part
	result += convertRupees(rupees) + " RUPEE";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
	if (rupees > 1) 
	{
		result += "S";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
	}

	// Paise part
	if (paise > 0) 
	{
		result += " AND " + convertPaise(paise) + " PAISE";
	cout<<__LINE__<<"  result  ->  "<<result<<endl;
	}
	result += " ONLY";
	cout<<__LINE__<<"  result  ->  "<<result<<endl<<endl<<endl;
	return result;
}

int main() 
{
	long double amount=1;
	cout<<"Enter 0 Zero to end the loop\n";
	while(amount != 0)
	{
		cout << "Enter amount (e.g., 1234.56): ";
		cin >> amount;
		cout <<amount<<"  :  "<<amountToWords(amount) << endl<<endl<<endl;
	}
	return 0;
}

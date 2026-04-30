
#include <iostream>

using namespace std;

string convertRupees(long long n)
{
	if (n==0) 
	{
		return "ZERO";
	}

	string result;
	
	// Crore (10^7)
	if(n >= 10000000)
	{
		long long crorePart = n / 100000000;
		result += aboveCrores(crorePart) + "CRORE";
		n%= 100000000;
		if(n>0)
		{
			result += " ";
		}
	}

	// Lakh (10^5) 
	if(n>=100000)
	{
		long long lakhPart = n / 100000;
		result += belowThosuand(lakhPart) + " LAKH";
		n%=100000;
		if(n>0)
		{
			result += " ";
		}
	}

	if(n >= 1000)
	{
		long long thousandPart = n / 1000;
		result += belowThousand(thousandPart) + "THOUSAND";
		n%= 1000;
		if(n>0)
		{
			result += " ";
		}
	}

	if(n > 0)
	{
		result += belowThousand(n);
	}

	return result;
}

// convert paise (0-99)
string convertPaise(int p)
{
	if(p == 0) 
	{
		return "";
	}

	if(p>=1 && p<=19)
	{
		return oneToNineteen(p);
	}
	else
	{
		return tens(p);
	}


}

string amountToWords(double amount)
{
	if (amount < 0) return "MINUS" + amountToWords(-amount);
	if (amount == 0) return "ZERO RUPEE ONLY";

	// separate integer and fractional parts
	long long rupees = (long long)amount;
	int paise = round((amount - rupees)*100);

	// Fix floating point precision
	if(paise >= 100)
	{
		reupees += paise/100;
		paise=paise%100;
	}

	string result;

	result += convertRupees(rupees) + "RUPEE";
	
	// Rupees part
	if(rupees > 1)
	{
		result += "S";
	}

	// Paise part 
	if(paise > 0 )
	{
		result += " AND " + convertPaise(paise) + "PAISE";
	}
	
	result += " ONLY";
	return result;
}


int main()
{

	long double amount=1;

	cout<<"Enter 0 Zero to end the loop \n";
	while(amount !=0)
		{
			cout<<"Enter amount (e.g., 1234.56):";
			cin>>amount;
			cout<<amount<<"  :  "<<amountToWords(amount) << endl<<endl<<endl;
		}
	return 0;
}


#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void initilizeArrayToZero(int a[],int n)
{
	for(int i =0;i<=n;i++)
	{
		a[i]=0;
	}
}
void stringToArray(string aa, int a[],int n)
{
	for(int i =n;i>=0;--i)
	{
		a[i]=aa[i]-'0';
	}

}
void printArray(int s[],int n)
{
	cout<<endl;
	for(int i=0;i<=n;i++)
	{
		cout<<s[i];
	}
	cout<<endl;
}

int main()
{
	int n,sa,sb;

	string aa,bb;
	cout<<"Enter number a"<<endl;
	cin>>aa;

	cout<<"Enter number b"<<endl;
	cin>>bb;


	sa=aa.length();
	sb=bb.length();



	if (sa>=sb)
	{
		n=sa;
	}
	else
	{
		n=sb;
	}



	int a[n+1],b[n+1],c[n+1],s[n+1];

	initilizeArrayToZero(a,n);
	initilizeArrayToZero(b,n);
	initilizeArrayToZero(c,n);
	initilizeArrayToZero(s,n);


	stringToArray("0"+aa,a,n);
	printArray(a,n);
	stringToArray("0"+bb,b,n);
	printArray(b,n);
	c[n]=0;
	c[n-1]=0;
	string x="0",y="0",result;
	for(int i = n;i>=0;--i)
	{
		x=to_string(a[i]+b[i]+c[i]);

		if(x.length() > 1)
		{
			c[i-1]=1;
		}
		y += string(1, x.back());
	}
	std::reverse(y.begin(), y.end());

	for(int i =0; i<y.length()-1;i++)
	{
		cout<<y[i];

	}
	cout<<endl; 
	//   cout<<"sum -> "<<y;

	//printArray(s,n);

	return 0;

}


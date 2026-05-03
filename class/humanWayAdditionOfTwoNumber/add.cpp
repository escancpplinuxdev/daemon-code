
#include <iostream>
#include <string>
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
	for(int i =0;i<=n;i++)
	{
		a[i]=aa[n]-'0';
	}

}
void printArray(int s[],int n)
{
	for(int i=0;i<n;i++)
	{
		cout<<s[i];
	}

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

	cout<<sa<<endl;
	cout<<sb<<endl;


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
	stringToArray(bb,b,n);
	printArray(b,n);
	c[n-1]=0;
	for(int i = n-1;i>=0;i++)
	{
		s[i]=a[i]+b[i]+c[i];

		if(s[i]>=10)
		{
			c[i-1]=c[i-1]+1;
		}
	}

	cout<<"sum -> ";
	
	printArray(s,n);

	return 0;

}

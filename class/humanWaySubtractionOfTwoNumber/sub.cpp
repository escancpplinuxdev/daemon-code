
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
void stringToArray(string num, int arr[],int n)
{
	int length=num.length();
	for(int i =length-1 , j =n ;i>=0;i--, j--)
	{
		arr[j]=num[i]-'0';
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
	int n,sa,sb,sc;

	string aa,bb,cc;
	cout<<"Enter number a : ";
	cin>>aa;

	cout<<"Enter number b : ";
	cin>>bb;

	cout<<"Enter number c : ";
	cin>>cc;

	sa=aa.length();
	sb=bb.length();
	sc=cc.length();



	if( (sa>=sb) && (sa>=sc) )
	{
		n=sa;
	}
	else if  (sb>= sc) 
	{
		n=sb;
	}
	else
	{
		n=sc;
	}

	cout<<"size of array n = "<<n<<endl;

	int a[n+2],b[n+2],c[n+2],carry[n+2],sum[n+2];

	initilizeArrayToZero(a,n+1);
	initilizeArrayToZero(b,n+1);
	initilizeArrayToZero(c,n+1);
	initilizeArrayToZero(carry,n+1);
	initilizeArrayToZero(sum,n+1);


	stringToArray(aa,a,n);
	printArray(a,n);
	stringToArray(bb,b,n);
	printArray(b,n);
	stringToArray(cc,c,n);
	printArray(c,n);

	string x="0",y="0",result;

        for(int i = n;i>=0;--i)
        {
                x=to_string(b[i]+c[i]+carry[i]);

                if(x.length() > 1)
                {
                        if(i != 0)
                        {
                                carry[i-1]=stoi(x.substr(0,x.length()-1));
                        }
                }
                y += string(1, x.back());
		sum[i]=stoi(x);
		cout<<"sum["<<i<<"]"<<sum[i]<<endl;
        }
        std::reverse(y.begin(), y.end());
        cout<<endl;
	cout<<"Add = ";
        for(int i =0; i<y.length()-1;i++)
        {
                cout<<y[i];

        }

        cout<<endl;
	

	for(int i = n;i>=0;--i)
	{
		x=to_string(a[i]-sum[i]);

		result += string(1, x.back());
	}
	std::reverse(result.begin(), result.end());
	cout<<endl; 

	for(int i =0; i<result.length()-1;i++)
	{
		cout<<result[i];

	}
	cout<<endl; 

	return 0;

}

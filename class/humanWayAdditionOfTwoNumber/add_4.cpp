
#include <vector>
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
	for(int i=0;i<=n;i++)
	{
		cout<<s[i];
	}
	cout<<endl;
}

int main()
{
	int count;
	vector <string >numbers;
	cout<<"Enter total number of addition elements : ";
	cin>>count;
	cout<<"Enter number for element \n";
	for(int i=0;i<count;i++)
	{
		string num;
		cout<<"num["<<i+1<<"] : ";
		cin>>num;
		numbers.push_back(num);
	}


	int maxlen = 0;
	for (const auto& num : numbers)
	{
		int len=num.length();
		if (len>maxlen)
		{
			maxlen = len;
		}
	}


	cout<<"size of array size = "<<maxlen<<endl;
	int size = maxlen +2 ; //extra space for carry
	int a[size];
	//	cout<<"size of array size = "<<size<<endl;

	int carry[size]={0},sum[size]={0};

	initilizeArrayToZero(carry,size);
	initilizeArrayToZero(sum,size);

	for(int j = 0; j<count-1;j++)
	{

		string x="0",y="0",result;
		
	       
		initilizeArrayToZero(a,size);
		stringToArray(numbers[j],a,maxlen);
		cout<<"print array :";
	        printArray(a,maxlen);


		for(int i = maxlen;i>0;--i)
		{
			cout<<"sum["<<i<<"] = '"<<sum[i]<<"'"<<endl;
			cout<<"carry["<<i<<"] = '"<<carry[i]<<"'"<<endl;
			cout<<"a["<<i<<"] = '"<<a[i]<<"'"<<endl;
			x=to_string(a[i]+carry[i]+sum[i]);
			cout<<"x = '"<<x<<"'"<<endl;	
			if(x.length() > 1)
			{
				if(i != 0)
				{
					initilizeArrayToZero(carry,size);
					carry[i-1]=stoi(x.substr(0,x.length()-1));
				}
			
			}

//			y += string(1, x.back());
			y +=  x;
		cout<<"y = "<<y<<endl;
//		std::reverse(y.begin(), y.end());
		cout<<endl; 
		
		cout<<"y = "<<y<<endl;
		cout<<"End of 1st addition \n\n \n";

		for(int i =0; i<=y.length()-1;i++)
		{
			cout<<y[i];

			initilizeArrayToZero(sum,size);
			string digitStr = string(1, y[i]);   // Convert char to string (1 character)
			sum[i] = stoi(digitStr);  
			cout<<"sum["<<i<<"]"<<sum[i]<<endl;

		}
		}
		cout<<endl; 
	}
	return 0;

}

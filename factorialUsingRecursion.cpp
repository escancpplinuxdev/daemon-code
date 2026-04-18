
#include <iostream>
using namespace std;
int fac(int n)
{
		if(n==1)
		{
			return 1;
		}
		else
		{
			return (n*fac(n-1));   // 5 * 4 *3 *2 *1 * 0
		}
}

int main()
{
	int n;
	cout<<"Enter number : ";
	cin>>n;

	cout<<"Factorial of "<<n<<" is : "<<fac(n)<<endl;
	return 0;
}

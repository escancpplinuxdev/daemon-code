
#include <iostream>


int main()
{
	int a=90320;
	std::cout<<"preincrement -> \n";
	
	for (int i = 0,j=0; i<5; a=++i)
	{
		int c=++j;
		std::cout<<i<<" "<<a<<" "<<c<<"\n";
	}
	std::cout<<"\n";
	int b=90320;
	std::cout<<"postncrement -> \n";
	for (int i = 0,j=0; i<5; b=i++)
	{
		int d=j++;
		std::cout<<i<<" "<<b<<" "<<d<<"\n";
	}
	std::cout<<"\n";

	return 0;
}

// For for loop -> The output of both loops is exactly the same: 0 1 2 3 4.
// but for assign to variable is different
// in preincrement  -> 1st increament then assgin i.e c = ++j = ++0 = 1;
// in postincrement -> 1st assign then increment  i.e d = j++ = 0++ = 0; i.e now value j = 1 but d = 0;

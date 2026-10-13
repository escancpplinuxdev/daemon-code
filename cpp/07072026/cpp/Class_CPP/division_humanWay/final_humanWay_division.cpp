#include <iostream>
#include <string>
#include <vector>
using namespace std;

void printLongDivision(long long dividend, long long divisor)
{
    if (divisor == 0)
    {
        cout << "Error: division by zero\n";
        return;
    }
    cout<<"dividend = "<<dividend<<"\n";
    cout<<"divisor  =  "<<divisor<<"\n";
    cout<<"\n\n";

    string dvd = to_string(dividend);
    string dvs = to_string(divisor);

 	cout<<"dvd.size() = "<<(int)dvd.size()<<"\n";
    // ---- First pass: figure out the quotient string (needed for the header) ----
    string quotient;
    long long cur = 0;
    for (int i = 0; i < (int)dvd.size(); i++)
    {

if (i > 0)	cout<<cur<<"\t+ "<<(dvd[i] - '0')<<"\n";

        cur = cur * 10 + (dvd[i] - '0');
	
	cout<<"current = "<<cur<<"\n";

        int q = 0;
        while ((q + 1) * divisor <= cur)
        {
            q++;
        }

        quotient += to_string(q);

	cout<<"product  = "<<(long long)q * divisor<<"\n";
	cout<<cur<<"\n- \n"<<(long long)q * divisor<<"\n__\n";

        cur -= (long long)q * divisor;
	
    }
    if (quotient.empty())
    {
        quotient = "0";
    }

        cout<<"\n\n";
	cout<<"remainder = "<<cur<<"\n";
	cout<<"quotient = "<<quotient<<"\n";

    cout << "\n";
}

int main()
{
/*
    printLongDivision(156, 4);
    printLongDivision(17, 5);
    printLongDivision(1234, 12);
    printLongDivision(100, 25);
*/
    printLongDivision(174321, 6);
//    printLongDivision(54030, 6);
    return 0;
}

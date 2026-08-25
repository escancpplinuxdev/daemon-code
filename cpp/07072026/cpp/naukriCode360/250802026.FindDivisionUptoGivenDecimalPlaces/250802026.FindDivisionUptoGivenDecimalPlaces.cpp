
#include <bits/stdc++.h>
using namespace std;

string findDivision(int x, int y, int n) 
{
    // Handle sign
    bool isNegative = (x < 0) ^ (y < 0);
    
    // Use long long for absolute values to avoid overflow
    long long num = llabs((long long)x);
    long long den = llabs((long long)y);
    
    // Integer part
    long long integerPart = num / den;
    long long remainder = num % den;
    
    string result = "";
    
    // Add sign
    if (isNegative && integerPart == 0 && remainder != 0) 
    {
        result += "-0.";
    } 
    else if (isNegative) 
    {
        result += "-";
        result += to_string(integerPart);
        result += ".";
    } 
    else 
    {
        result += to_string(integerPart);
        result += ".";
    }
    
    // If integer part is negative but value is between -1 and 0
    if (isNegative && integerPart == 0 && remainder == 0) 
    {
        return "0." + string(n, '0');
    }
    
    // Handle the case where integer part is 0 and we have sign
    if (isNegative && integerPart == 0 && remainder != 0) 
    {
        // result already has "-0."
    } 
    else if (isNegative) 
    {
        // result already has "-X."
    } 
    else 
    {
        // result already has "X."
    }
    
    // Generate decimal digits
    for (int i = 0; i < n; i++) 
    {
        remainder *= 10;
        long long digit = remainder / den;
        result += to_string(digit);
        remainder %= den;
    }
    
    return result;
}

int main() 
{
    int T;
    cin >> T;
    
    while (T--) 
    {
        int x, y, n;
        cin >> x >> y >> n;
        cout << findDivision(x, y, n) << endl;
    }
    
    return 0;
}

/*

Find Division Upto Given Decimal Places
Easy
0/40
Average time to solve is 15m
Contributed by
Asked in companies
Problem statement

You are given three integers ‘X’, ‘Y’ and ‘N’. Find the value of division X / Y up to ‘N’ decimal places.

You should return a string that represents the value X / Y up to ‘N’ decimal places. This string must have ‘N’ digits after decimals. You don’t need to round off the result to ‘N’ decimal places, just find the first ‘N’ places after decimals.

Note:

1. It is guaranteed that division X / Y is always finite.

Example:

Consider X = 5, Y = 4 and N = 5, then you should return “1.25000”. 
Note, here we add 3 trailing zeros so that it has exactly 5 digits after decimals.    

Detailed explanation ( Input/output format, Notes, Images )
Constraints:

1 <= T <= 50
-10^8  <= X <= 10^8
-10^8  <= Y <= 10^8
Y != 0
1 <= N <= 10^4

Time limit: 1 sec

Sample Input 1:

2
5 4 1
5 4 5   

Sample Output 1:

1.2
1.25000

Explanation of Sample Input 1:

Test case 1:
Division 5/4 = 1.25, but we need to print only ‘1’ digit after decimals, thus we should print 1.2.

Test case 2:
See the problem statement for an explanation.

Sample Input 2:

3
-1 -2 1
1 -5 6
-1 1000 2

Sample Output 2:

0.5
-0.200000
-0.00



string findDivision(int x, int y, int n) {
	// Write your code here.
}

give this with int main () and header file 
alway give { in next line 
which help me in reading .
for each and every code

*/

#include <bits/stdc++.h>
using namespace std;

int findProductSumDifference(int n)
{
    int product = 1;
    int sum = 0;
    
    // Handle the case when n = 0 (though constraints say N >= 1)
    if (n == 0)
    {
        return 0;
    }
    
    while (n > 0)
    {
        int digit = n % 10;
        product *= digit;
        sum += digit;
        n /= 10;
    }
    
    return product - sum;
}

int main()
{
    int T;
    cin >> T;
    
    while (T--)
    {
        int n;
        cin >> n;
        cout << findProductSumDifference(n) << endl;
    }
    
    return 0;
}

/*

Subtract The Product And Sum Of Digits Of An Integer Easy 0/40 Average time to solve is 10m Contributed by Asked in companies Problem statement You are given an integer number ‘N’, and your task is to return the difference between the product of its digits and the sum of its digits. Note: You do not need to print anything; it has already been taken care of. Just implement the given function. For Example : Input: N = 234 Output: 15 Explanation: Product of digits = 2 * 3 * 4 = 24 Sum of digits = 2 + 3 + 4 = 9 Result = 24 - 9 = 15 Detailed explanation ( Input/output format, Notes, Images ) Constraints : 1 <= T <= 3000 1 <= N <= 10^5 Time Limit: 1 sec Sample Input 1 : 2 234 4421 Sample Output 1 : 15 21 Explanation For Sample Input 1 : Product of digits = 2 * 3 * 4 = 24 Sum of digits = 2 + 3 + 4 = 9 Result = 24 - 9 = 15 Product of digits = 4 * 4 * 2 * 1 = 32 Sum of digits = 4 + 4 + 2 + 1 = 11 Result = 32 - 11 = 21 Sample Input 2 : 1 87 Sample Output 2 : 41
#include <bits/stdc++.h> 
int findProductSumDifference(int n){
	// Write your code here
}

*/


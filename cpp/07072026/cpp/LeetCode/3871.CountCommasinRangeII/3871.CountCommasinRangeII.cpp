#include <iostream>
#include <string>
#include <climits>
using namespace std;


class Solution {
public:
    long long countCommas(long long n) {
        if (n < 1000) return 0;
        
        long long total = 0;
        long long power = 1000;
        int commas = 1;
        
        while (power <= n) {
            long long start = power;
            
            // Check overflow before multiplying
            if (power > LLONG_MAX / 1000) {
                long long count = n - start + 1;
                total += count * commas;
                break;
            }
            
            long long end = min(n, power * 1000 - 1);
            long long count = end - start + 1;
            total += count * commas;
            
            power *= 1000;
            commas++;
        }
        
        return total;
    }
};

int main() {
    Solution solution;
    
    cout << "Example 1: n=1002 -> " << solution.countCommas(1002) << endl;
    cout << "Example 2: n=998 -> " << solution.countCommas(998) << endl;
    cout << "Test large: n=1000000000000000 -> " << solution.countCommas(1000000000000000LL) << endl;
    
    return 0;
}





/*

3871. Count Commas in Range II
Medium
Topics
premium lock iconCompanies
Hint

You are given an integer n.

Return the total number of commas used when writing all integers from [1, n] (inclusive) in standard number formatting.

In standard formatting:

    A comma is inserted after every three digits from the right.
    Numbers with fewer than 4 digits contain no commas.

 

Example 1:

Input: n = 1002

Output: 3

Explanation:

The numbers "1,000", "1,001", and "1,002" each contain one comma, giving a total of 3.

Example 2:

Input: n = 998

Output: 0

Explanation:

​​​​​​​All numbers from 1 to 998 have fewer than four digits. Therefore, no commas are used.

class Solution {
public:
    long long countCommas(long long n) {
        
    }
};

give this with int main() include it's header files

*/

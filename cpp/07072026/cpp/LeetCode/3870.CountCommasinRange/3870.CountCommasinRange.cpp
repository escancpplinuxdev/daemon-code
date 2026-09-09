//Here's the complete solution with proper individual header files

#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

class Solution 
{
public:
    int countCommas(int n) 
{
        if (n < 1000) return 0;
        
        long long total = 0;
        
        // For each range of numbers with same number of digits
        // 1-3 digits: 0 commas
        // 4-6 digits: 1 comma
        // 7-9 digits: 2 commas
        // 10-12 digits: 3 commas
        // etc.
        
        long long power = 1000;  // First number with 4 digits
        
        for (int commas = 1; ; commas++) 
	{
            long long start = power;
            long long end = min<long long>(n, power * 1000 - 1);
            
            if (start > n) break;
            
            // Count numbers in this range
            long long count = end - start + 1;
            total += count * commas;
            
            power *= 1000;
        }
        
        return total;
    }
};

int main() 
{
    Solution solution;
    
    // Example 1
    cout << "Example 1: n=1002 -> " << solution.countCommas(1002) << endl;
    // Expected: 3 (1000, 1001, 1002 each have 1 comma)
    
    // Example 2
    cout << "Example 2: n=998 -> " << solution.countCommas(998) << endl;
    // Expected: 0 (all numbers < 1000 have no commas)
    
    // Additional test cases
    cout << "Test 3: n=1000 -> " << solution.countCommas(1000) << endl;
    // Expected: 1 (only 1000 has 1 comma)
    
    cout << "Test 4: n=1000000 -> " << solution.countCommas(1000000) << endl;
    // Expected: 
    // 1000-999999: 999000 numbers × 1 comma = 999000
    // 1000000: 1 number × 2 commas = 2
    // Total = 999002
    
    cout << "Test 5: n=999999 -> " << solution.countCommas(999999) << endl;
    // Expected: 999000 (all 4-6 digit numbers have 1 comma)
    
    cout << "Test 6: n=999 -> " << solution.countCommas(999) << endl;
    // Expected: 0
    
    cout << "Test 7: n=1 -> " << solution.countCommas(1) << endl;
    // Expected: 0
    
    cout << "Test 8: n=999999999 -> " << solution.countCommas(999999999) << endl;
    // Expected: 999999000 + 900000000 × 2 = 2799999000
    // Wait, let me calculate...
    // 1000-999999: 999000 numbers × 1 = 999000
    // 1000000-999999999: 999000000 numbers × 2 = 1998000000
    // Total = 1998999000
    
    return 0;
}

/*


3870. Count Commas in Range
Easy
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

All numbers from 1 to 998 have fewer than four digits. Therefore, no commas are used.

class Solution {
public:
    int countCommas(int n) {
        
    }
};

give with int main()

*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int prefixStrings(int n) 
{
        const int MOD = 1e9 + 7;
        
        // This is Catalan number C_n = (2n)! / ((n+1)! * n!)
        // Number of valid strings = Catalan number for n
        
        // Precompute factorials
        vector<long long> fact(2 * n + 1, 1);
        for (int i = 1; i <= 2 * n; i++) 
{
            fact[i] = fact[i - 1] * i % MOD;
        }
        
        // Modular inverse using Fermat's Little Theorem
        auto modPow = [&](long long a, long long b) 
{
            long long res = 1;
            while (b > 0) 
{
                if (b & 1) res = res * a % MOD;
                a = a * a % MOD;
                b >>= 1;
            }
            return res;
        };
        
        // Catalan number = (2n)! / ((n+1)! * n!)
        long long ans = fact[2 * n];
        ans = ans * modPow(fact[n + 1], MOD - 2) % MOD;
        ans = ans * modPow(fact[n], MOD - 2) % MOD;
        
        return (int)ans;
    }
};

int main() {
    Solution sol;
    
    // Example 1
    cout << "Example 1 (n=2): " << sol.prefixStrings(2) << endl;  // Expected: 2
    
    // Example 2
    cout << "Example 2 (n=3): " << sol.prefixStrings(3) << endl;  // Expected: 5
    
    // Additional test
    cout << "Test (n=4): " << sol.prefixStrings(4) << endl;  // Expected: 14
    
    return 0;
}



/*

Count Prefix-Balanced Binary Strings
Difficulty: EasyAccuracy: 44.17%Submissions: 12K+Points: 2

Given an integer n, count the number of binary strings of length 2 * n that contain exactly n ones and n zeros such that every prefix of the string contains at least as many ones as zeros. Since the answer can be very large, return it modulo 109 + 7.

Examples:

Input: n = 2
Output: 2
Explanation: "1100", "1010" are two such strings of size 4 which have exactly two 1's in each.

Input: n = 3
Output: 5
Explanation: "111000", "101100", "101010", "110010", "110100" are such 5 strings which have exactly three 1's in each.

class Solution {
  public:
    int prefixStrings(int n) {
        // code here
        
    }
};

*/

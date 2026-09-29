#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int numberOfSets(int n, int k) {
        const long long MOD = 1e9 + 7;
        
        // We need C(n + k - 1, 2k) mod MOD
        int N = n + k - 1;
        int R = 2 * k;
        
        // If R > N, no valid combination
        if (R > N) return 0;
        
        // Precompute factorials
        vector<long long> fact(N + 1, 1);
        for (int i = 1; i <= N; i++) {
            fact[i] = fact[i - 1] * i % MOD;
        }
        
        // Modular inverse using Fermat's little theorem
        auto modPow = [&](long long base, long long exp) -> long long {
            long long result = 1;
            base %= MOD;
            while (exp > 0) {
                if (exp & 1) result = result * base % MOD;
                base = base * base % MOD;
                exp >>= 1;
            }
            return result;
        };
        
        // nCr = fact[N] / (fact[R] * fact[N-R])
        long long numerator = fact[N];
        long long denominator = fact[R] * fact[N - R] % MOD;
        long long inverseDenom = modPow(denominator, MOD - 2);
        
        return (int)(numerator * inverseDenom % MOD);
    }
};

int main() {
    Solution solution;
    
    // Example 1
    cout << "Example 1: n=4, k=2 -> " << solution.numberOfSets(4, 2) << endl;
    // Expected: 5
    
    // Example 2
    cout << "Example 2: n=3, k=1 -> " << solution.numberOfSets(3, 1) << endl;
    // Expected: 3
    
    // Example 3
    cout << "Example 3: n=30, k=7 -> " << solution.numberOfSets(30, 7) << endl;
    // Expected: 796297179
    
    // Additional test cases
    cout << "Test 4: n=5, k=1 -> " << solution.numberOfSets(5, 1) << endl;
    // Expected: 10 (C(5,2) = 10)
    
    cout << "Test 5: n=5, k=2 -> " << solution.numberOfSets(5, 2) << endl;
    // Expected: C(6,4) = 15
    
    cout << "Test 6: n=6, k=2 -> " << solution.numberOfSets(6, 2) << endl;
    // Expected: C(7,4) = 35
    
    cout << "Test 7: n=6, k=3 -> " << solution.numberOfSets(6, 3) << endl;
    // Expected: C(8,6) = 28
    
    cout << "Test 8: n=2, k=1 -> " << solution.numberOfSets(2, 1) << endl;
    // Expected: C(2,2) = 1
    
    cout << "Test 9: n=1, k=1 -> " << solution.numberOfSets(1, 1) << endl;
    // Expected: 0 (not enough points)
    
    cout << "Test 10: n=10, k=3 -> " << solution.numberOfSets(10, 3) << endl;
    // Expected: C(12,6) = 924
    
    return 0;
}

/*

1621. Number of Sets of K Non-Overlapping Line Segments
Medium
Topics
premium lock iconCompanies
Hint

Given n points on a 1-D plane, where the ith point (from 0 to n-1) is at x = i, find the number of ways we can draw exactly k non-overlapping line segments such that each segment covers two or more points. The endpoints of each segment must have integral coordinates. The k line segments do not have to cover all n points, and they are allowed to share endpoints.

Return the number of ways we can draw k non-overlapping line segments. Since this number can be huge, return it modulo 109 + 7.

 

Example 1:

Input: n = 4, k = 2
Output: 5
Explanation: The two line segments are shown in red and blue.
The image above shows the 5 different ways {(0,2),(2,3)}, {(0,1),(1,3)}, {(0,1),(2,3)}, {(1,2),(2,3)}, {(0,1),(1,2)}.

Example 2:

Input: n = 3, k = 1
Output: 3
Explanation: The 3 ways are {(0,1)}, {(0,2)}, {(1,2)}.

Example 3:

Input: n = 30, k = 7
Output: 796297179
Explanation: The total number of possible ways to draw 7 line segments is 3796297200. Taking this number modulo 109 + 7 gives us 796297179.

class Solution {
public:
    int numberOfSets(int n, int k) {
        
    }
};

give this with int main() with headers

*/

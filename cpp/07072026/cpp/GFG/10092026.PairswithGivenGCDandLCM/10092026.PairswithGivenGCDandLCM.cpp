#include <iostream>
#include <cmath>
using namespace std;

class Solution {
public:
    int pairCount(int x, int y) {
        // Step 1: LCM must be divisible by GCD
        if (y % x != 0) {
            return 0;
        }
        
        // Step 2: Compute product = y / x
        // If a = x*p and b = x*q, then p*q = y/x and gcd(p,q) = 1
        int product = y / x;
        
        // Step 3: If product == 1, then p = q = 1
        // a = b = x, but since a and b are distinct pairs counted separately,
        // and here a == b, so only 1 pair (x, x)
        if (product == 1) {
            return 1;
        }
        
        // Step 4: Count coprime pairs (p, q) such that p*q = product
        int count = 0;
        
        // Iterate over all divisors of product
        for (int p = 1; p * p <= product; p++) {
            if (product % p == 0) {
                int q = product / p;
                
                // Check if gcd(p, q) == 1
                if (gcd(p, q) == 1) {
                    if (p == q) {
                        // Same pair (a == b), count once
                        count += 1;
                    } else {
                        // Different pairs (a, b) and (b, a), count twice
                        count += 2;
                    }
                }
            }
        }
        
        return count;
    }
    
private:
    // GCD function (Euclidean algorithm)
    int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    cout << "Example 1: x=2, y=12 -> " << solution.pairCount(2, 12) << endl;
    // Expected: 4
    
    // Example 2
    cout << "Example 2: x=6, y=4 -> " << solution.pairCount(6, 4) << endl;
    // Expected: 0
    
    // Additional test cases
    cout << "Test 3: x=1, y=1 -> " << solution.pairCount(1, 1) << endl;
    // Expected: 1 (only pair (1,1))
    
    cout << "Test 4: x=1, y=6 -> " << solution.pairCount(1, 6) << endl;
    // Expected: 4 ((1,6), (6,1), (2,3), (3,2))
    
    cout << "Test 5: x=3, y=9 -> " << solution.pairCount(3, 9) << endl;
    // Expected: 2 ((3,9), (9,3))
    
    cout << "Test 6: x=5, y=25 -> " << solution.pairCount(5, 25) << endl;
    // Expected: 2 ((5,25), (25,5))
    
    cout << "Test 7: x=2, y=8 -> " << solution.pairCount(2, 8) << endl;
    // Expected: 2 ((2,8), (8,2))
    
    cout << "Test 8: x=1, y=12 -> " << solution.pairCount(1, 12) << endl;
    // Expected: 4 ((1,12), (12,1), (3,4), (4,3))
    
    cout << "Test 9: x=4, y=16 -> " << solution.pairCount(4, 16) << endl;
    // Expected: 2 ((4,16), (16,4))
    
    cout << "Test 10: x=2, y=24 -> " << solution.pairCount(2, 24) << endl;
    // Expected: 4
    
    return 0;
}

/*

Pairs with Given GCD and LCM
Difficulty: EasyAccuracy: 52.61%Submissions: 4K+Points: 2

Given two integers x and y representing the GCD and LCM of two unknown positive integers a and b, count the number of valid pairs (a, b) satisfying these conditions. Note that (a, b) and (b, a) are counted as distinct pairs when a ≠ b.

Examples:

Input: x = 2, y = 12
Output: 4
Explanation: The valid pairs are (2, 12), (4, 6), (6, 4), and (12, 2), since each pair has GCD = 2 and LCM = 12.

Input: x = 6, y = 4
Output: 0
Explanation: LCM must always be a multiple of GCD. Since y is not divisible by x, no valid pair exists.

class Solution {
  public:
    int pairCount(int x, int y) {
        // code here
        
    }
};

give this int main with proper header files

*/

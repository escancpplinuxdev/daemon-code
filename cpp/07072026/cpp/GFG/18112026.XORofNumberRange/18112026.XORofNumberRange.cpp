/*
XOR of Number Range
Difficulty: EasyAccuracy: 50.0%Submissions: 96K+Points: 2Average Time: 10m

Given two integers l and r, find the bitwise XOR of all integers in the range [l, r] (both inclusive).

Examples:

Input: l = 4, r = 8 
Output: 8 
Explanation: The XOR of all integers from 4 to 8 is (4 xor 5 xor 6 xor 7 xor 8) = 8.

Input: l = 2, r = 4
Output: 5
Explanation: The XOR of all integers from 2 to 4 is (2 xor 3 xor 4) = 5.

Input: l = 5, r = 9
Output: 5
Explanation: The XOR of all integers from 5 to 9 is (5 xor 6 xor 7 xor 8 xor 9) = 5.

class Solution {
  public:
    int findXOR(int l, int r) {
        // code here
        
    }
};

*/

#include <iostream>
using namespace std;

class Solution1 {
public:
    // XOR of all integers from 0 to n inclusive
    int xorUpto(int n) {
        if (n < 0) return 0;
        switch (n % 4) {
            case 0: return n;
            case 1: return 1;
            case 2: return n + 1;
            default: return 0;  // case 3
        }
    }

    int findXOR(int l, int r) {
        return xorUpto(r) ^ xorUpto(l - 1);
    }
};

class Solution {
public:
    int xorUpto(int n) {
        if (n < 0) return 0;
        switch (n % 4) {
            case 0: return n;
            case 1: return 1;
            case 2: return n + 1;
            default: return 0;
        }
    }
    
    int findXOR(int l, int r) {
        return xorUpto(r) ^ xorUpto(l - 1);
    }
};

int main() {
    Solution sol;

    // Test 1
    cout << "Test 1: l=4, r=8 -> "
         << sol.findXOR(4, 8) << " (expected 8)" << endl;

    // Test 2
    cout << "Test 2: l=2, r=4 -> "
         << sol.findXOR(2, 4) << " (expected 5)" << endl;

    // Test 3
    cout << "Test 3: l=5, r=9 -> "
         << sol.findXOR(5, 9) << " (expected 5)" << endl;

    // Test 4: l == r
    cout << "Test 4: l=7, r=7 -> "
         << sol.findXOR(7, 7) << " (expected 7)" << endl;

    // Test 5: small range
    cout << "Test 5: l=1, r=1 -> "
         << sol.findXOR(1, 1) << " (expected 1)" << endl;

    // Test 6: starting from 0
    cout << "Test 6: l=0, r=5 -> "
         << sol.findXOR(0, 5) << " (expected 1)" << endl;
    // 0^1^2^3^4^5 = 1

    return 0;
}


/*

XOR of Number Range
Explanation

We need the XOR of all integers from l to r inclusive.
Naive Approach — Too Slow

Looping from l to r and XORing would be O(r - l + 1). For large ranges (up to 10⁹), this is too slow.
Key Insight — XOR Prefix Function

Define xorUpto(n) = 0 ^ 1 ^ 2 ^ ... ^ n.

Then, using the property a ^ a = 0:
text

XOR(l..r) = xorUpto(r) ^ xorUpto(l - 1)

Because xorUpto(l-1) cancels out the XOR of 0..l-1 from xorUpto(r).
The XOR Prefix Pattern

There's a beautiful period-4 pattern in xorUpto(n):
n mod 4	xorUpto(n)
0	n
1	1
2	n + 1
3	0

Let me verify:

    xorUpto(0) = 0 (0 mod 4 = 0, formula gives 0 ✓)

    xorUpto(1) = 0 ^ 1 = 1 (1 mod 4 = 1, formula gives 1 ✓)

    xorUpto(2) = 0 ^ 1 ^ 2 = 3 (2 mod 4 = 2, formula gives 2+1 = 3 ✓)

    xorUpto(3) = 3 ^ 3 = 0 (3 mod 4 = 3, formula gives 0 ✓)

    xorUpto(4) = 0 ^ 4 = 4 (0 mod 4 = 0, formula gives 4 ✓)

    xorUpto(5) = 4 ^ 5 = 1 (1 mod 4 = 1, formula gives 1 ✓)

    xorUpto(6) = 1 ^ 6 = 7 (2 mod 4 = 2, formula gives 6+1 = 7 ✓)

    xorUpto(7) = 7 ^ 7 = 0 (3 mod 4 = 3, formula gives 0 ✓)

Why the Pattern Holds

Group the numbers in blocks of 4: (4k, 4k+1, 4k+2, 4k+3).
text

4k       = 4k
4k ^ 4k+1 = 1           (since 4k and 4k+1 differ only in bit 0)
1 ^ 4k+2  = 4k+3
4k+3 ^ 4k+3 = 0

So the XOR of each full block of 4 is 0. Only the remainder mod 4 matters.
Algorithm
text

xorUpto(n):
    if n < 0: return 0
    switch n % 4:
        case 0: return n
        case 1: return 1
        case 2: return n + 1
        case 3: return 0

findXOR(l, r):
    return xorUpto(r) ^ xorUpto(l - 1)

Example 1: l = 4, r = 8

    xorUpto(8): 8 % 4 = 0 → 8

    xorUpto(3): 3 % 4 = 3 → 0

    Result: 8 ^ 0 = 8 ✅

Example 2: l = 2, r = 4

    xorUpto(4): 4 % 4 = 0 → 4

    xorUpto(1): 1 % 4 = 1 → 1

    Result: 4 ^ 1 = 5 ✅

Example 3: l = 5, r = 9

    xorUpto(9): 9 % 4 = 1 → 1

    xorUpto(4): 4 % 4 = 0 → 4

    Result: 1 ^ 4 = 5 ✅

Final Code
cpp

#include <iostream>
using namespace std;

class Solution {
public:
    // XOR of all integers from 0 to n inclusive
    int xorUpto(int n) {
        if (n < 0) return 0;
        switch (n % 4) {
            case 0: return n;
            case 1: return 1;
            case 2: return n + 1;
            default: return 0;  // case 3
        }
    }

    int findXOR(int l, int r) {
        return xorUpto(r) ^ xorUpto(l - 1);
    }
};

int main() {
    Solution sol;

    // Test 1
    cout << "Test 1: l=4, r=8 -> "
         << sol.findXOR(4, 8) << " (expected 8)" << endl;

    // Test 2
    cout << "Test 2: l=2, r=4 -> "
         << sol.findXOR(2, 4) << " (expected 5)" << endl;

    // Test 3
    cout << "Test 3: l=5, r=9 -> "
         << sol.findXOR(5, 9) << " (expected 5)" << endl;

    // Test 4: l == r
    cout << "Test 4: l=7, r=7 -> "
         << sol.findXOR(7, 7) << " (expected 7)" << endl;

    // Test 5: small range
    cout << "Test 5: l=1, r=1 -> "
         << sol.findXOR(1, 1) << " (expected 1)" << endl;

    // Test 6: starting from 0
    cout << "Test 6: l=0, r=5 -> "
         << sol.findXOR(0, 5) << " (expected 1)" << endl;
    // 0^1^2^3^4^5 = 1

    return 0;
}

Dry Run of Final Code
Dry Run 1: l = 4, r = 8

    xorUpto(8): 8 % 4 = 0 → return 8

    xorUpto(3): 3 % 4 = 3 → return 0

    findXOR = 8 ^ 0 = 8 ✅

Dry Run 2: l = 2, r = 4

    xorUpto(4): 4 % 4 = 0 → return 4

    xorUpto(1): 1 % 4 = 1 → return 1

    findXOR = 4 ^ 1 = 5 ✅

Dry Run 3: l = 5, r = 9

    xorUpto(9): 9 % 4 = 1 → return 1

    xorUpto(4): 4 % 4 = 0 → return 4

    findXOR = 1 ^ 4 = 5 ✅

Dry Run 4: l = 7, r = 7

    xorUpto(7): 7 % 4 = 3 → return 0

    xorUpto(6): 6 % 4 = 2 → return 7

    findXOR = 0 ^ 7 = 7 ✅

Dry Run 5: l = 1, r = 1

    xorUpto(1): 1 % 4 = 1 → return 1

    xorUpto(0): 0 % 4 = 0 → return 0

    findXOR = 1 ^ 0 = 1 ✅

Dry Run 6: l = 0, r = 5

    xorUpto(5): 5 % 4 = 1 → return 1

    xorUpto(-1): -1 < 0 → return 0

    findXOR = 1 ^ 0 = 1 ✅

Verification: 0 ^ 1 ^ 2 ^ 3 ^ 4 ^ 5 = 0 ^ 1 ^ 2 ^ 3 ^ 4 ^ 5:

    0 ^ 1 = 1

    1 ^ 2 = 3

    3 ^ 3 = 0

    0 ^ 4 = 4

    4 ^ 5 = 1 ✅

Complexity
Metric	Value	Reasoning
Time	O(1)	Constant number of arithmetic and bitwise operations
Space	O(1)	Only a few integer variables
Summary
Aspect	Detail
Problem	XOR of all integers in [l, r]
Key insight	xorUpto(n) has a period-4 pattern: n, 1, n+1, 0 for n mod 4 = 0,1,2,3
Formula	XOR(l..r) = xorUpto(r) ^ xorUpto(l-1)
Time	O(1)
Space	O(1)
The "Aha" Moment

The XOR of the first n natural numbers follows a period-4 pattern because any 4 consecutive integers (4k, 4k+1, 4k+2, 4k+3) XOR to 0 (the high bits cancel and the low 2 bits form 00, 01, 10, 11 which XOR to 00). So the prefix XOR is computable in constant time from n mod 4, and any range XOR is prefix(r) ^ prefix(l-1).
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int xorUpto(int n) {
        if (n < 0) return 0;
        switch (n % 4) {
            case 0: return n;
            case 1: return 1;
            case 2: return n + 1;
            default: return 0;
        }
    }
    
    int findXOR(int l, int r) {
        return xorUpto(r) ^ xorUpto(l - 1);
    }
};

🎯 Time: O(1) | Space: O(1)

*/

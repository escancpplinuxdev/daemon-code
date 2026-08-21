#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    long long findKthSmallest(vector<int>& coins, int k) 
{
        int n = coins.size();
        
        // Remove duplicates and multiples
        vector<long long> c;
        for (int x : coins) {
            bool add = true;
            for (long long y : c) {
                if (x % y == 0) {
                    add = false;
                    break;
                }
            }
            if (add) c.push_back(x);
        }
        
        n = c.size();
        vector<long long> lcms(1 << n, 1);
        
        // Precompute LCM for all subsets
        for (int mask = 1; mask < (1 << n); mask++) {
            int bit = __builtin_ctz(mask);
            int prevMask = mask ^ (1 << bit);
            lcms[mask] = lcm(lcms[prevMask], c[bit]);
            if (lcms[mask] > 1e18) lcms[mask] = 1e18 + 1;
        }
        
        auto count = [&](long long x) -> long long {
            long long total = 0;
            // Inclusion-Exclusion over all subsets
            for (int mask = 1; mask < (1 << n); mask++) {
                long long l = lcms[mask];
                long long cnt = x / l;
                if (__builtin_popcount(mask) % 2 == 1) {
                    total += cnt;
                } else {
                    total -= cnt;
                }
            }
            return total;
        };
        
        long long lo = 1, hi = 1e18;
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (count(mid) >= k) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    }
};

int main() 
{
    Solution sol;
    
    // Example 1
    vector<int> coins1 = {3, 6, 9};
    cout << "Example 1: " << sol.findKthSmallest(coins1, 3) << endl;  // Expected: 9
    
    // Example 2
    vector<int> coins2 = {5, 2};
    cout << "Example 2: " << sol.findKthSmallest(coins2, 7) << endl;  // Expected: 12
    
    // Additional test
    vector<int> coins3 = {2, 3};
    cout << "Example 3: " << sol.findKthSmallest(coins3, 5) << endl;  // Expected: 8 (2,3,4,6,8)
    
    return 0;
}

/*

3116. Kth Smallest Amount With Single Denomination Combination
Hard
Topics
premium lock iconCompanies
Hint

You are given an integer array coins representing coins of different denominations and an integer k.

You have an infinite number of coins of each denomination. However, you are not allowed to combine coins of different denominations.

Return the kth smallest amount that can be made using these coins.

 

Example 1:

Input: coins = [3,6,9], k = 3

Output: 9

Explanation: The given coins can make the following amounts:
Coin 3 produces multiples of 3: 3, 6, 9, 12, 15, etc.
Coin 6 produces multiples of 6: 6, 12, 18, 24, etc.
Coin 9 produces multiples of 9: 9, 18, 27, 36, etc.
All of the coins combined produce: 3, 6, 9, 12, 15, etc.

Example 2:

Input: coins = [5,2], k = 7

Output: 12

Explanation: The given coins can make the following amounts:
Coin 5 produces multiples of 5: 5, 10, 15, 20, etc.
Coin 2 produces multiples of 2: 2, 4, 6, 8, 10, 12, etc.
All of the coins combined produce: 2, 4, 5, 6, 8, 10, 12, 14, 15, etc.


class Solution {
public:
    long long findKthSmallest(vector<int>& coins, int k) {
        
    }
};

give it with int main()

*/

#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    string lexGreaterPermutation(string s, string target) 
    {
        int n = s.size();
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;
        
        // Check if the smallest permutation is already > target
        string sorted_s = s;
        sort(sorted_s.begin(), sorted_s.end());
        if (sorted_s > target) return sorted_s;
        
        string result = "";
        vector<int> tempFreq = freq;
        
        // Try to match as much of target as possible
        for (int i = 0; i < n; i++) 
        {
            int idx = target[i] - 'a';
            
            // If we can match target[i], do so
            if (tempFreq[idx] > 0) 
            {
                tempFreq[idx]--;
                result += target[i];
                continue;
            }
            
            // If we cannot match, try to place a character > target[i]
            for (int c = idx + 1; c < 26; c++) 
            {
                if (tempFreq[c] > 0) 
                {
                    tempFreq[c]--;
                    result += char('a' + c);
                    
                    // Fill rest with smallest available
                    for (int j = 0; j < 26; j++) 
                    {
                        while (tempFreq[j] > 0) 
                        {
                            result += char('a' + j);
                            tempFreq[j]--;
                        }
                    }
                    return result;
                }
            }
            
            // Cannot match and no larger character available → no valid permutation
            return "";
        }
        
        // If we matched the entire target, we need to find the next permutation
        // of the result (which is target itself) that is > target.
        // This is done by finding the rightmost position where we can increase.
        result = target; // matched the whole target
        int i = n - 2;
        while (i >= 0 && result[i] >= result[i + 1]) i--;
        if (i < 0) return "";
        int j = n - 1;
        while (result[j] <= result[i]) j--;
        swap(result[i], result[j]);
        reverse(result.begin() + i + 1, result.end());
        return result;
    }
};

int main() 
{
    Solution sol;
    
    // Example 1
    cout << "Example 1: " << sol.lexGreaterPermutation("abc", "bba") << endl;  // Expected: "bca"
    
    // Example 2
    cout << "Example 2: " << sol.lexGreaterPermutation("leet", "code") << endl;  // Expected: "eelt"
    
    // Example 3
    cout << "Example 3: " << sol.lexGreaterPermutation("baba", "bbaa") << endl;  // Expected: ""
    
    // Large test
    string s50 = "ababababababababababababababababababababababababab";
    string target50 = "bababababababababababababababababababababababababa";
    cout << "Large test: " << sol.lexGreaterPermutation(s50, target50) << endl;
    
    return 0;
}

/*

3720. Lexicographically Smallest Permutation Greater Than Target
Attempted
Medium
Topics
premium lock iconCompanies
Hint

You are given two strings s and target, both having length n, consisting of lowercase English letters.

Return the lexicographically smallest of s that is strictly greater than target. If no permutation of s is lexicographically strictly greater than target, return an empty string.

A string a is lexicographically strictly greater than a string b (of the same length) if in the first position where a and b differ, string a has a letter that appears later in the alphabet than the corresponding letter in b.

 

Example 1:

Input: s = "abc", target = "bba"

Output: "bca"

Explanation:

    The permutations of s (in lexicographical order) are "abc", "acb", "bac", "bca", "cab", and "cba".
    The lexicographically smallest permutation that is strictly greater than target is "bca".

Example 2:

Input: s = "leet", target = "code"

Output: "eelt"

Explanation:

    The permutations of s (in lexicographical order) are "eelt", "eetl", "elet", "elte", "etel", "etle", "leet", "lete", "ltee", "teel", "tele", and "tlee".
    The lexicographically smallest permutation that is strictly greater than target is "eelt".

Example 3:

Input: s = "baba", target = "bbaa"

Output: ""

Explanation:

    The permutations of s (in lexicographical order) are "aabb", "abab", "abba", "baab", "baba", and "bbaa".
    None of them is lexicographically strictly greater than target. Therefore, the answer is "".

*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution
{
public:
    string compress(string &s)
    {
        int n = s.length();
        
        // dp[i] = best encryption for prefix s[0..i-1]
        vector<string> dp(n + 1, "");
        dp[0] = "";
        
        for (int i = 1; i <= n; i++)
        {
            // Option 1: Take the character directly
            dp[i] = dp[i - 1] + s[i - 1];
            
            // Option 2: Try to insert '*' at position j (0 <= j < i-1)
            for (int j = 0; j < i - 1; j++)
            {
                // Candidate: dp[j] + "*"
                string candidate = dp[j] + "*";
                
                // Decode candidate to check if it matches s[0..i-1]
                string decoded = "";
                string prefix = "";
                
                for (char c : candidate)
                {
                    if (c == '*')
                    {
                        decoded += prefix;
                    }
                    else
                    {
                        prefix += c;
                        decoded += c;
                    }
                }
                
                // If decoded matches the prefix, this candidate is valid
                if (decoded == s.substr(0, i))
                {
                    // Choose lexicographically smallest candidate
                    if (candidate.length() < dp[i].length() ||
                        (candidate.length() == dp[i].length() && candidate < dp[i]))
                    {
                        dp[i] = candidate;
                    }
                }
            }
        }
        
        return dp[n];
    }
};

int main()
{
    Solution sol;
    
    // Test Case 1
    string s1 = "ababcababcd";
    cout << "Input: " << s1 << endl;
    cout << "Output: " << sol.compress(s1) << endl;
    cout << "Expected: ab*c*d" << endl << endl;
    
    // Test Case 2
    string s2 = "zzzzzzz";
    cout << "Input: " << s2 << endl;
    cout << "Output: " << sol.compress(s2) << endl;
    cout << "Expected: z*z*z" << endl << endl;
    
    // Test Case 3
    string s3 = "abcabcabc";
    cout << "Input: " << s3 << endl;
    cout << "Output: " << sol.compress(s3) << endl << endl;
    
    return 0;
}

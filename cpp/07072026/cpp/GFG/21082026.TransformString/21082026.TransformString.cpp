#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    int transform(string &s1, string &s2) 
{
        int n = s1.size();
        int m = s2.size();
        
        // If lengths differ, transformation is impossible
        if (n != m) return -1;
        
        // Count characters to check if transformation is possible
        int count[256] = {0};
        for (int i = 0; i < n; i++) {
            count[s1[i]]++;
            count[s2[i]]--;
        }
        for (int i = 0; i < 256; i++) {
            if (count[i] != 0) return -1;
        }
        
        // Find the minimum operations by matching from the end
        int i = n - 1, j = n - 1;
        int operations = 0;
        
        while (i >= 0 && j >= 0) {
            if (s1[i] == s2[j]) {
                i--;
                j--;
            } else {
                // Need to move this character to the front
                operations++;
                i--;
            }
        }
        
        return operations;
    }
};

int main() 
{
    Solution sol;
    
    // Example 1
    string s1_1 = "abd";
    string s2_1 = "bad";
    cout << "Example 1: " << sol.transform(s1_1, s2_1) << endl;  // Expected: 1
    
    // Example 2
    string s1_2 = "GeeksForGeeks";
    string s2_2 = "ForGeeksGeeks";
    cout << "Example 2: " << sol.transform(s1_2, s2_2) << endl;  // Expected: 3
    
    // Additional test: Already transformed
    string s1_3 = "abc";
    string s2_3 = "abc";
    cout << "Example 3: " << sol.transform(s1_3, s2_3) << endl;  // Expected: 0
    
    // Additional test: Impossible
    string s1_4 = "abc";
    string s2_4 = "def";
    cout << "Example 4: " << sol.transform(s1_4, s2_4) << endl;  // Expected: -1
    
    return 0;
}


/*

Transform String
Difficulty: MediumAccuracy: 29.76%Submissions: 51K+Points: 4

Given two strings s1 and s2. Find the minimum number of steps required to transform string s1 into string s2. The only allowed operation for the transformation is selecting a character from string s1 and inserting it in the beginning of string s1.

If transformation is not possible return -1.

Examples:

Input: s1 = "abd", s2 = "bad"
Output: 1
Explanation: The conversion can take place in 1 operation: Pick 'b' and place it at the front.

Input: s1 = "GeeksForGeeks", s2 = "ForGeeksGeeks"
Output: 3
Explanation: The conversion can take place in 3 operations: 
Pick 'r' and place it at the front.
s1 = "rGeeksFoGeeks"
Pick 'o' and place it at the front.
s1 = "orGeeksFGeeks"
Pick 'F' and place it at the front.
s1 = "ForGeeksGeeks"

class Solution {
  public:
    int transform(string &s1, string &s2) {
        // code here
        
    }
};

give this with int main ()

*/

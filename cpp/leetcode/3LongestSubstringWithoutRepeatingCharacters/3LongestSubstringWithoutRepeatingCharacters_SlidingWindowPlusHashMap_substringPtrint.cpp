#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Solution 
{
public:
    int lengthOfLongestSubstring(const string& s, string& outSubstring) 
    {

        unordered_map<char, int> lastSeen;  // char → last seen index

        int maxLen  = 0;
        int maxLeft = 0;  // start index of the best window
        int left    = 0;  // left boundary of current window

        for (int right = 0; right < (int)s.size(); right++) 
	{

            char c = s[right];

            // if duplicate found INSIDE current window → shrink from left
            if (lastSeen.count(c) && lastSeen[c] >= left) 
	    {
                left = lastSeen[c] + 1;
            }

            lastSeen[c] = right;  // update last seen index

            // update max if current window is bigger
            int currentLen = right - left + 1;
            if (currentLen > maxLen) 
	    {
                maxLen  = currentLen;
                maxLeft = left;   // save start of best window
            }
        }

        // reconstruct substring from saved position + length
        outSubstring = (maxLen > 0) ? s.substr(maxLeft, maxLen) : "";
        return maxLen;
    }
};

// ──────────────────────────────────────────
void runTest(Solution& sol, const string& input, int expected) 
{

    string substring;
    int result = sol.lengthOfLongestSubstring(input, substring);
    string pass = (result == expected) ? "✓" : "✗";

    cout << "Input     : \"" << input      << "\"\t"
         << "Output    :  "  << result     << "\t"
         << "Expected  :  "  << expected   << "  " << pass << "\t"
         << "Substring : \"" << substring  << "\"\n";
//         << "─────────────────────────────────\n";
}

int main() 
{

    Solution sol;
/*
    runTest(sol, "abcabcbb",     3);
    runTest(sol, "bbbbb",        1);
    runTest(sol, "pwwkew",       3);
    runTest(sol, "",             0);
    runTest(sol, "a",            1);
    runTest(sol, "abcdefg",      7);
    runTest(sol, "dvdf",         3);
*/
    runTest(sol, "abcabcabcdbb", 4);

    return 0;
}

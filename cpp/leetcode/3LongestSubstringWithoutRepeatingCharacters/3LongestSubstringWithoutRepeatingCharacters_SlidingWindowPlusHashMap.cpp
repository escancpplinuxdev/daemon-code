#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution 
{
	public:
		int lengthOfLongestSubstring(string s) 
		{
			// stores: character → last seen index
			unordered_map<char, int> lastSeen;

			int maxLen = 0;
			int left   = 0;   // left boundary of current window

			for (int right = 0; right < (int)s.size(); right++) 
			{
				char c = s[right];

				// If c was seen AND it's inside current window [left, right]
				if (lastSeen.count(c) && lastSeen[c] >= left) 
				{
					// shrink window: move left past the duplicate
					left = lastSeen[c] + 1;
				}

				lastSeen[c] = right;                      // update last seen index
				maxLen = max(maxLen, right - left + 1);   // update max window size
			}

			return maxLen;
		}
};

int main() 
{
	Solution sol;

	// --- Test Cases ---
	string tests[] = 
	{
		"abcabcbb",   // expected: 3
		"bbbbb",      // expected: 1
		"pwwkew",     // expected: 3
		"",           // expected: 0 (empty string)
		"a",          // expected: 1 (single char)
		"abcdefg",    // expected: 7 (all unique)
		"dvdf"        // expected: 3 ("vdf")
	};
	int expected[] = {3, 1, 3, 0, 1, 7, 3};

	for (int i = 0; i < 7; i++) 
	{
		int result = sol.lengthOfLongestSubstring(tests[i]);
		cout << "Input: \""  << tests[i]   << "\""
			<< "  Output: " << result
			<< "  Expected: "<< expected[i]
			<< (result == expected[i] ? "  ✓" : "  ✗")
			<< "\n";
	}

	return 0;
}

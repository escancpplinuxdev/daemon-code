
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class Solution 
{
	public:
		int lengthOfLongestSubstring (string &s, string & longestSubstring)
		{
			unordered_map<char,char>lastIndex;
			int left=0;
			int maxLen=0;
			int bestStart=0;//start index of longestSubstring substring

			for(int right = 0; right <s.length();++right)
			{
				char c=s[right];
				// if character already seen and it's last index is within current window
				if(lastIndex.find(c) != lastIndex.end() && lastIndex[c]>=left)
				{
					left = lastIndex[c] + 1;
				}
				lastIndex[c]=right;
				int curLen=right - left +1 ;
				if(curLen>maxLen)
				{
					maxLen=curLen;
					bestStart=left;
				}
			}
			longestSubstring = s.substr(bestStart,maxLen);
			return maxLen;

		}
};

int main()
{

	Solution sol;

	string s; 
	string longestSubstring;
	int len;

	s = "abcabcbb";
	len = sol.lengthOfLongestSubstring(s, longestSubstring);
	cout << "Input: " << s << "\nLongest substring: \"" << longestSubstring << "\"\nLength: " << len << "\n\n";


	s = "bbbbb";
	len = sol.lengthOfLongestSubstring(s, longestSubstring);
	cout << "Input: " << s << "\nLongest substring: \"" << longestSubstring << "\"\nLength: " << len << "\n\n";

	s = "pwwkew";
	len = sol.lengthOfLongestSubstring(s, longestSubstring);
	cout << "Input: " << s << "\nLongest substring: \"" << longestSubstring << "\"\nLength: " << len << "\n\n";

	s = "dvdf";
	len = sol.lengthOfLongestSubstring(s, longestSubstring);
	cout << "Input: " << s << "\nLongest substring: \"" << longestSubstring << "\"\nLength: " << len << "\n\n";

	s = "abcabcabcdbb";
	len = sol.lengthOfLongestSubstring(s, longestSubstring);
	cout << "Input: " << s << "\nLongest substring: \"" << longestSubstring << "\"\nLength: " << len << "\n";

	return 0;
}

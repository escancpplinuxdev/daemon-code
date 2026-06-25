
#include <string>
#include <unordered_map>
#include <iostream>
using namespace std;

class Solution
{
	public :
		int lengthOfLongestSubstring(string s)
		{
			unordered_map<char,int> lastIndex; // character -> last seen index

			int left = 0;
			int maxLen = 0;

			for(int right=0; right<s.length();++right)
			{
				char c= s[right];
				//if character already seen and it's last index is >= left, move left
				if(lastIndex.count(c) && lastIndex[c]>= left)
				{
					left = lastIndex[c] + 1;
				}

				lastIndex[c]=right;
				maxLen=max(maxLen,right-left+1);


			}
			return maxLen;

		}
};

int main()
{
	Solution sol;
/*
	string s1= "abcabcbb";
	cout<<"Input: "<<s1<<" -> Output: "<<sol.lengthOfLongestSubstring(s1)<<endl;

	string s2 = "bbbbb";
	cout << "Input: " << s2 << " -> Output: " << sol.lengthOfLongestSubstring(s2) << endl;

	string s3 = "pwwkew";
	cout << "Input: " << s3 << " -> Output: " << sol.lengthOfLongestSubstring(s3) << endl;

	string s4 = "abcabcabcabcdbb";
	cout << "Input: " << s4 << " -> Output: " << sol.lengthOfLongestSubstring(s4) << endl;
*/
	string s;
	getline(cin,s);
	cout << "Input: " <<s<<" -> Output: " << sol.lengthOfLongestSubstring(s) << endl;

	return 0;
}

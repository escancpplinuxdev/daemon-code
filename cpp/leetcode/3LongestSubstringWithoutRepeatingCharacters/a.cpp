
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class Solution 
{
	public:
		int getSubStringLength(const string &s)
		{
			int right = 0, left = 0, curLen = 0, maxLen = 0;
			unordered_map<char,int>lastIndex;

			for(right = 0; right<s.length();++right)
			{
				char ch = s[right];
				if (lastIndex.count(ch) && lastIndex[ch] >= left )
				{
					left = lastIndex[ch] + 1;
				}
				lastIndex[ch] = right;
				curLen = right - left + 1;
				maxLen = max(curLen,maxLen);

			}	
			return maxLen;
		}
};

int main()
{
	Solution sol;

	string s="";
	getline(cin,s);
	cout<<"Input -> "<<s<<"  output -> "<<sol.getSubStringLength(s)<<"\n";


	return 0;
}


#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>

using namespace std;

class Solution
{
	public :
		int lengthOfLongestSubstring(string s)
		{
			unordered_map<char,int> lastIndex; // character -> last seen index

			int left = 0;
			int maxLen = 0;
			int curLen = 0;

			for(int right=0; right<s.length();++right)
			{
				cout<<setw(2)<<"right = "<<right<<"\tleft = "<<left<<"\n";
				char character= s[right];
				cout<<"character= s["<<right<<"] : " <<character<<"\n";
				//if character already seen and it's last index is >= left, move left
				cout<<"lastIndex.count("<<character<<") : " <<lastIndex.count(character)<<"\n";
				cout<<"lastIndex["<<character<<"]>= left : "<<lastIndex[character]<<" >= "<<left;
				if(lastIndex.count(character) && lastIndex[character]>= left)
				{
					left = lastIndex[character] + 1;
					cout<<"\t-> true \nleft = lastIndex["<<character<<"] + 1 : " <<left<<"\n";
				}
				else
				{
					cout<<"\t-> false";
				}

				lastIndex[character]=right;
				cout<<"\nlastIndex["<<character<<"]=right : " <<lastIndex[character]<<"\n";
				curLen = right - left + 1;
				cout<<"curLen = right - left + 1 = "<<right<<" - "<<left<<" + 1 : " <<curLen<<"\n";
				cout<<"maxLen : " <<maxLen<<"\n";
				cout<<"maxLen=max("<<maxLen<<","<<curLen<<") = ";
				maxLen=max(maxLen,curLen);
				cout<<"maxLen : " <<maxLen<<"\n";
				for(auto &it : lastIndex)				
				{
					cout<<it.first<<"|"<<it.second<<" ";
				}
				cout<<"\n\n\n";
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
	cout << "Input: " <<s<<" -> Output: \n" << sol.lengthOfLongestSubstring(s) << endl;

	return 0;
}

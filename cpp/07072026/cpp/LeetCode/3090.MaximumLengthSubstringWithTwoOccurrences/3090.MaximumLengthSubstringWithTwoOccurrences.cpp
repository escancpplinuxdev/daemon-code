
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <cstring>
#include <string>



class Solution 
{
	public:
		int maximumLengthSubstring(std::string s) 
		{
			std::unordered_map<char,int> freq;   // frequency of each character in current window
			std::string a = "";
			int left = 0, ans = 0;
			int n = s.size();

			for (int right = 0; right < n; ++right) 
			{
				// 1. Add new character to window
				 freq[s[right]] ++ ;

				// 2. Shrink window if invalid (current character appears > 2 times)
				while (freq[s[right]] > 2) 
				{
					freq[s[left]]--;
					++left;                  // move left pointer forward
				}

				// 3. Update answer with current valid window length
				ans = std::max(ans, right - left + 1);
				a = s.substr(left, right - left + 1); // left to left of string
			}
			std::cout<<"a = "<<a<<"\n";
			return ans;
		}
};


/*
class Solution 
{
	public:
		int maximumLengthSubstring(std::string s) 
		{
			std::vector<int> freq(26, 0);   // frequency of each character in current window
			int left = 0, ans = 0;
			int n = s.size();

			for (int right = 0; right < n; ++right) 
			{
				// 1. Add new character to window
				int idx = s[right] - 'a';
				++freq[idx];

				// 2. Shrink window if invalid (current character appears > 2 times)
				while (freq[idx] > 2) 
				{
					--freq[s[left] - 'a'];   // remove leftmost character
					++left;                  // move left pointer forward
				}

				// 3. Update answer with current valid window length
				ans = std::max(ans, right - left + 1);
			}
			return ans;
		}
};
*/
/*
   class Solution
   {
   public:
   int maximumLengthSubstring (std::string s)
   {
   std::vector<int> freq(26, 0);
   int left = 0, ans = 0;
   int n = s.size();

   for (int right = 0; right < n; ++right) 
   {
   std::cout<<"left = "<<left<<"\n";
   std::cout<<"right = "<<right<<"\n";
   int idx = s[right] - 'a';
   std::cout<<"idx = "<<idx<<"\n";
   ++freq[idx];
   std::cout<<"++freq[idx] = "<<freq[idx]<<"\n";

   while (freq[idx] > 2) 
   {
   --freq[s[left] - 'a'];
   ++left;
   std::cout<<"++freq[s[left] - 'a'] = "<<freq[s[left] - 'a']<<"\n";
   std::cout<<"++left = "<<left<<"\n";
   }

   std::cout<<"left = "<<left<<"\n";
   std::cout<<"right = "<<right<<"\n";
   std::cout<<"right -left + 1 =  "<<right - left + 1<<"\n";

   ans = std::max(ans, right - left + 1);
   std::cout<<"ans = "<<ans<<"\n\n\n";
   }
   return ans;
   }
   };
 */
int main()
{
	Solution sol;
	std::string s1 = "bcbbbcba"; // o/p substring = bcba = 4
	std::cout<<"o/p = "<<sol.maximumLengthSubstring(s1)<<"\n";

	std::string s2 = "aaaa"; // o/p substring = aa = 2 
	std::cout<<"o/p = "<<sol.maximumLengthSubstring(s2)<<"\n"; 

	return 0;
}

/*
3090. Maximum Length Substring With Two Occurrences
Easy
Topics
premium lock iconCompanies
Hint
Given a string s, return the maximum length of a  such that it contains at most two occurrences of each character.

 

Example 1:

Input: s = "bcbbbcba"

Output: 4

Explanation:
The following substring has a length of 4 and contains at most two occurrences of each character: "bcbbbcba".

Example 2:

Input: s = "aaaa"

Output: 2

Explanation:
The following substring has a length of 2 and contains at most two occurrences of each character: "aaaa".

class Solution {
public:
    int maximumLengthSubstring(string s) {
        
    }
};

*/


#include <iostream>
#include <bits/stdc++.h>
using namespace std;

bool validAbbreviation(string &str, string &abbr) 
{
	int i = 0, j = 0;
	int n = str.size(), m = abbr.size();

	while (i < n && j < m) 
	{
		if (isdigit(abbr[j])) 
		{
			int num = 0;
			while (j < m && isdigit(abbr[j])) 
			{
				num = num * 10 + (abbr[j] - '0');
				j++;
			}
			i += num;
			if (i > n) 
			{
				return false;
			}
		}
		else
		{
			if (str[i] != abbr[j]) return false;
			i++;
			j++;
		}
	}

	return i == n && j == m;
}

int main()
{
	string str = "abbreviations";
	string abbr = "a11s";
	std::cout<<"o/p = "<<validAbbreviation(str,abbr)<<"\n";

	return 0;
}

/*
Problem statement

You are given a string ‘STR’ consisting of English lowercase letters. You are also provided another string ‘ABBR’ consisting of lowercase letters and digits.

We say ‘ABBR’ matches ‘STR’ if it fulfils the following condition:

1) While matching, if we encounter a number ‘X’ in ‘ABBR’, then we have to skip ‘X’ characters in ‘STR’ and keep on matching.

For example: For ‘STR’ = “abc”, all valid matching ‘ABBR’ are: [“abc”, “1bc”, “1b1”, “2c”, “3”, “a1c”, “a2”, “ab1”].

Your task is to check whether ‘STR’ matches with the given ‘ABBR’ or not. Return TRUE if ‘STR’ matches with ‘ABBR’ else return FALSE.

For example :

If ‘STR’ = “hello” and ‘ABBR’= “1e2o”.
1. As ‘STR[0]’=’h’ but ‘ABBR[0]’=1 which means we can skip 1 character from ‘STR’ so continue matching.
2. ‘STR[1]’=’e’ and ‘ABBR[1]’=’e’ (matches) so continue matching.
3.‘STR[2]’=’l’ and ‘ABBR[2]’=2 which means we can skip 2 characters from ‘STR’ so continue matching.
4. We will not match the 3rd index as skipped in the earlier step.   
4.‘STR[4]’=’o’ and ‘ABBR[4]’=’o’ (matches). 
So we can say ‘STR’ matches with ‘ABBR’ and return TRUE.

Detailed explanation ( Input/output format, Notes, Images )
Sample input 1 :

1
abbreviations a11s

Sample output 1 :

YES

Explanation for sample output 1 :

For the given ‘STR’ = “abbreviations” and ‘ABBR’ = “a11s”, the character at the 0th index of both the strings matches, so we can continue matching for next indices.
For the next step, we encounter a number 11 in ‘ABBR’, which means we have to skip 11 characters from ‘STR’ and continue matching.
After skipping 11 indices, we move to the 12th index of ‘STR’. Now,  both ‘STR[12]’ = ‘s’ and ‘ABBR[3]’ = ‘s’ and match.
Finally, we reach the end of both strings. So we print YES as an answer.

Sample input 2 :

2
xyzzy 4
ninja 8inja

Sample output 2 :

NO
NO

Explanation for sample output 2 :

(i) For the first test case, ‘ABBR’ has only one digit that is 4 which means we have to skip 4 characters in ‘STR’ from the starting. And after skipping 4 characters, we are still left with one character in ‘STR’ and zero characters in ‘ABBR’. So, we can say that ‘STR’ does not match with ‘ABBR’.

(ii) For the second test case, the 0th index of ‘ABBR’ has a digit that is 8 which means we have to skip 8 characters in ‘STR’ from the starting. But the length of ‘STR’ is less than 8 which means we can not skip 8 characters. So, we can say that ‘STR’ does not match with ‘ABBR’.


#include <bits/stdc++.h> 
bool validAbbreviation (string &str, string &abbr) {
	// Write your code here.
}

*/

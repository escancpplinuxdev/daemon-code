#include <bits/stdc++.h>
using namespace std;

bool areMetaStrings(string str1, string str2) {
    int n = str1.length();
    int m = str2.length();
    
    // If lengths are different, cannot be meta strings
    if (n != m) return false;
    
    // If strings are equal, they are not meta strings
    if (str1 == str2) return false;
    
    vector<int> diff;
    for (int i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            diff.push_back(i);
        }
    }
    
    // For meta strings, there must be exactly 2 mismatches
    // and swapping them should make the strings equal
    if (diff.size() != 2) return false;
    
    // Check if swapping the two mismatched characters in str1 makes it equal to str2
    swap(str1[diff[0]], str1[diff[1]]);
    return str1 == str2;
}

int main() 
{
    int T;
    cin >> T;
    
    while (T--) 
	{
        string str1, str2;
        cin >> str1 >> str2;
        
        if (areMetaStrings(str1, str2)) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    
    return 0;
}


/*

Meta Strings
Easy
0/40
Average time to solve is 10m
Contributed by
Asked in company
Problem statement

You are given two strings 'STR1' and 'STR2'. You need to tell whether the strings are meta strings or not.

Meta strings are strings that can be made equal by swapping exactly one pair of distinct characters in one of the strings.
Note:

Equal strings are not considered as meta strings. 

Detailed explanation ( Input/output format, Notes, Images )
Constraints:

1 <= T <= 10
2 <= length of STR1<= 10 ^ 5
2 <= length of STR2 <= 10 ^ 5

Sample Input 1:

2
Coding
Codnig
Ninjas
Niaxns

Sample Output 1:

YES
NO

Explanation for Input 1:

For test case 1: By swapping ‘i’ and ‘n’ in the second string, it becomes equal to the first string.

For test case 2: There is no way we can make both the strings equals by swapping one pair of characters.

Sample Input 2:

3
Hello
Hello
Play
Playes
Seek
Seke

Sample Output 2:

NO
NO
YES

*/

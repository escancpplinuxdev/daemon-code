#include <bits/stdc++.h>
using namespace std;

string convertSentence(string s, int n)
{
    // Keypad mapping: each character maps to the keypress sequence
    // a -> 2, b -> 22, c -> 222
    // d -> 3, e -> 33, f -> 333
    // g -> 4, h -> 44, i -> 444
    // j -> 5, k -> 55, l -> 555
    // m -> 6, n -> 66, o -> 666
    // p -> 7, q -> 77, r -> 777, s -> 7777
    // t -> 8, u -> 88, v -> 888
    // w -> 9, x -> 99, y -> 999, z -> 9999
    
    string keypad[] = {
        "2", "22", "222",      // a, b, c
        "3", "33", "333",      // d, e, f
        "4", "44", "444",      // g, h, i
        "5", "55", "555",      // j, k, l
        "6", "66", "666",      // m, n, o
        "7", "77", "777", "7777", // p, q, r, s
        "8", "88", "888",      // t, u, v
        "9", "99", "999", "9999" // w, x, y, z
    };
    
    string result = "";
    
    for (char c : s)
    {
        int idx = c - 'a';
        result += keypad[idx];
    }
    
    return result;
}

int main()
{
    int T;
    cin >> T;
    
    while (T--)
    {
        int n;
        cin >> n;
        
        string s;
        cin >> s;
        
        cout << convertSentence(s, n) << endl;
    }
    
    return 0;
}

/*
Convert Sentence
Easy
0/40
Average time to solve is 15m
Contributed by
Asked in companies
Problem statement

You are given a sentence in the form of a string ‘S’. You have to convert ‘S’ into its equivalent mobile numeric keypad sequence, i.e. print the sequence in such a way that if it is typed on the given keypad, the output should be the string ‘S’. A keypad for the reference is shown below.

Note:

1. There will be only lower case letters.
2. There will not be any special characters or capital letters in the input string.
3. There will be no spaces in the string.

Detailed explanation ( Input/output format, Notes, Images )
Sample Input 1:

2
12
codingninjas
1
f

Sample Output 1:

22266634446646644466527777
333

Explanation for Sample Output 1:

For the first test case, for ‘c’ we have to press 2 three times, for ‘o’ we have to press 6 three times and so on. Hence we get the desired output.

For the second test case, to type the letter ‘f’, we can press 6 three times.

#include <bits/stdc++.h> 
string convertSentence(string s, int n){
	// Write your code here.
}

give this with int main()

*/

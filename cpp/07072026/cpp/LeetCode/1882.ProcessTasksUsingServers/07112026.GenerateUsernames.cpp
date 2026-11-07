#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Solution 
{
	public:
		vector<string> generateUsernames(vector<string> arr) 
		{
			unordered_set<string> reserved(arr.begin(), arr.end());
			unordered_set<string> assigned;
			unordered_map<string, int> nextSuffix;
			vector<string> ans;
			ans.reserve(arr.size());

			for (const string& s : arr) 
			{
				if (assigned.find(s) == assigned.end()) 
				{
					ans.push_back(s);
					assigned.insert(s);
				} 
				else
				{
					int k = nextSuffix.count(s) ? nextSuffix[s] : 1;
					string cand;
					while (true) 
					{
						cand = s + to_string(k);
						if (!reserved.count(cand) && !assigned.count(cand)) break;
						k++;
					}
					ans.push_back(cand);
					assigned.insert(cand);
					nextSuffix[s] = k + 1;
				}
			}
			return ans;
		}
};

// Helper to print a vector<string>
void printVec(const vector<string>& v) 
{
	cout << "[";
	for (int i = 0; i < (int)v.size(); i++) 
	{
		cout << "\"" << v[i] << "\"";
		if (i + 1 < (int)v.size()) cout << ", ";
	}
	cout << "]";
}

// Helper to compare and print checkmark
/*
   printVec — helper to format a vector of strings as ["a", "b", "c"].

   check — compares expected vs actual using == (vector equality), prints both, and appends ✔ if they match or ✘ if they don't.

   Each test case is wrapped in its own {} block to avoid variable name collisions.
 */
void check(const vector<string>& expected, const vector<string>& actual) 
{
	cout << "Expected: ";
	printVec(expected);
	cout << "\nOutput:   ";
	printVec(actual);
	if (expected == actual) 
	{
		cout << "  ✔\n" << endl;
	} 
	else
	{
		cout << "  ✘\n" << endl;
	}
}

int main() 
{
	Solution sol;

	// Test 1
	{
		vector<string> arr = {"aba", "ab", "aba", "aba", "ab"};
		vector<string> expected = {"aba", "ab", "aba1", "aba2", "ab1"};
		cout << "Test 1:" << endl;
		check(expected, sol.generateUsernames(arr));
	}

	// Test 2
	{
		vector<string> arr = {"a", "a", "a1", "a"};
		vector<string> expected = {"a", "a2", "a1", "a3"};
		cout << "Test 2:" << endl;
		check(expected, sol.generateUsernames(arr));
	}

	// Test 3
	{
		vector<string> arr = {"aba1", "aba", "aba"};
		vector<string> expected = {"aba1", "aba", "aba2"};
		cout << "Test 3:" << endl;
		check(expected, sol.generateUsernames(arr));
	}

	// Test 4: Single element
	{
		vector<string> arr = {"hello"};
		vector<string> expected = {"hello"};
		cout << "Test 4:" << endl;
		check(expected, sol.generateUsernames(arr));
	}

	// Test 5: All duplicates
	{
		vector<string> arr = {"x", "x", "x", "x"};
		vector<string> expected = {"x", "x1", "x2", "x3"};
		cout << "Test 5:" << endl;
		check(expected, sol.generateUsernames(arr));
	}

	return 0;
}

/*
   class Solution 
   {
   public:
   vector<string> generateUsernames(vector<string> arr) 
   {
   unordered_set<string> reserved;
   for (const string& s : arr) reserved.insert(s);

   unordered_set<string> assigned;
   unordered_map<string, int> nextSuffix;
   vector<string> ans;
   ans.reserve(arr.size());

   for (const string& s : arr) {
   if (assigned.find(s) == assigned.end()) {
// First occurrence: keep as-is
ans.push_back(s);
assigned.insert(s);
} else {
// Duplicate: find smallest free suffix
int k = nextSuffix.count(s) ? nextSuffix[s] : 1;
string candidate;
while (true) {
candidate = s + to_string(k);
if (reserved.find(candidate) == reserved.end() &&
assigned.find(candidate) == assigned.end()) {
break;
}
k++;
}
ans.push_back(candidate);
assigned.insert(candidate);
nextSuffix[s] = k + 1;
}
}

return ans;
}
};

int main() 
{
Solution sol;

// Test 1
vector<string> arr1 = {"aba", "ab", "aba", "aba", "ab"};
vector<string> ans1 = sol.generateUsernames(arr1);
cout << "Test 1: [";
for (int i = 0; i < (int)ans1.size(); i++) {
cout << "\"" << ans1[i] << "\"";
if (i + 1 < (int)ans1.size()) cout << ", ";
}
cout << "] (expected [\"aba\", \"ab\", \"aba1\", \"aba2\", \"ab1\"])" << endl;

// Test 2
vector<string> arr2 = {"a", "a", "a1", "a"};
vector<string> ans2 = sol.generateUsernames(arr2);
cout << "Test 2: [";
for (int i = 0; i < (int)ans2.size(); i++) {
cout << "\"" << ans2[i] << "\"";
if (i + 1 < (int)ans2.size()) cout << ", ";
}
cout << "] (expected [\"a\", \"a2\", \"a1\", \"a3\"])" << endl;

// Test 3
vector<string> arr3 = {"aba1", "aba", "aba"};
vector<string> ans3 = sol.generateUsernames(arr3);
cout << "Test 3: [";
for (int i = 0; i < (int)ans3.size(); i++) {
cout << "\"" << ans3[i] << "\"";
if (i + 1 < (int)ans3.size()) cout << ", ";
}
cout << "] (expected [\"aba1\", \"aba\", \"aba2\"])" << endl;

return 0;
}
*/
/*

Generate Usernames
Difficulty: EasyAccuracy: 70.79%Submissions: 2K+Points: 2

Given an array of strings arr[]. The array represents first names of users, generate a unique username for each entry in the order it appears.

    Initially, consider all usernames present in the input array as reserved.
    If a username has not already been assigned to any previous user, keep it unchanged.
    Otherwise, append the smallest positive integer suffix that produces a username that has not been reserved or assigned to any previous user.

Return the array of generated usernames.

Examples:

Input: arr[] = ["aba", "ab", "aba", "aba", "ab"]
Output: ["aba", "ab", "aba1", "aba2", "ab1"]
Explanation: The first occurrences of "aba" and "ab" are new, so they remain unchanged. The next "aba" is already used, so "aba1" is assigned. When "aba" appears again, "aba1" is already used, so "aba2" is assigned. Similarly, the second occurrence of "ab" becomes "ab1".

Input: arr[] = ["a", "a", "a1", "a"]
Output: ["a", "a2", "a1", "a3"]
Explanation: Initially, "a" and "a1" are reserved. The first occurrence of "a" retains its name. For the second occurrence, "a1" is already reserved, so the smallest available username is "a2". The original "a1" retains its name when processed. Finally, the last occurrence of "a" gets "a3".

Input: arr[] = ["aba1", "aba", "aba"]
Output: ["aba1", "aba", "aba2"]
Explanation: Initially, "aba1" and "aba" are reserved. The first occurrence of each retains its original name. For the second occurrence of "aba", "aba1" is already reserved, so "aba2" is assigned.

class Solution {
  public:
    vector<string> generateUsernames(vector<string> arr) {
        // code here
        
    }
};


*/


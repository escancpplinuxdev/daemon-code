
#include <iostream>

#include <bits/stdc++.h>
using namespace std;

class Solution 
{
    struct Node 
{
        char lc, rc;
        int pref, suff, best, len;
    };

    vector<Node> tree;
    string str;

    Node merge(const Node& left, const Node& right) {
        if (left.len == 0) return right;
        if (right.len == 0) return left;

        Node res;
        res.lc = left.lc;
        res.rc = right.rc;
        res.len = left.len + right.len;

        // prefix
        res.pref = left.pref;
        if (left.pref == left.len && left.rc == right.lc) {
            res.pref = left.len + right.pref;
        }

        // suffix
        res.suff = right.suff;
        if (right.suff == right.len && right.lc == left.rc) {
            res.suff = right.len + left.suff;
        }

        // best
        res.best = max(left.best, right.best);
        if (left.rc == right.lc) {
            res.best = max(res.best, left.suff + right.pref);
        }

        return res;
    }

    void build(int node, int l, int r) {
        if (l == r) {
            tree[node].lc = tree[node].rc = str[l];
            tree[node].pref = tree[node].suff = tree[node].best = tree[node].len = 1;
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid);
        build(node * 2 + 1, mid + 1, r);
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    void update(int node, int l, int r, int idx, char ch) {
        if (l == r) {
            str[idx] = ch;
            tree[node].lc = tree[node].rc = ch;
            tree[node].pref = tree[node].suff = tree[node].best = tree[node].len = 1;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid) update(node * 2, l, mid, idx, ch);
        else update(node * 2 + 1, mid + 1, r, idx, ch);
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

public:
    vector<int> longestRepeating(string s, string queryCharacters, vector<int>& queryIndices) {
        int n = s.size();
        str = s;
        tree.resize(4 * n + 5);
        if (n > 0) build(1, 0, n - 1);

        vector<int> ans;
        int k = queryCharacters.size();
        for (int i = 0; i < k; ++i) {
            int idx = queryIndices[i];
            char ch = queryCharacters[i];
            update(1, 0, n - 1, idx, ch);
            ans.push_back(tree[1].best);
        }
        return ans;
    }
};


int main()
{
//	Input: s = "babacc", queryCharacters = "bcb", queryIndices = [1,3,3] -> Output: [3,3,4]
	Solution sol;
	string s = "babacc";
	string queryCharacters = "bcb";
	vector<int> queryIndices = {1,3,3};
	vector v = sol.longestRepeating(s,queryCharacters,queryIndices);

	for (auto x : v)
	{
		std::cout<<x<<" ";
	}
	
		std::cout<<"\n";
	
	return 0;
}

/*
2213. Longest Substring of One Repeating Character
Hard
Topics
premium lock iconCompanies
Hint

You are given a 0-indexed string s. You are also given a 0-indexed string queryCharacters of length k and a 0-indexed array of integer indices queryIndices of length k, both of which are used to describe k queries.

The ith query updates the character in s at index queryIndices[i] to the character queryCharacters[i].

Return an array lengths of length k where lengths[i] is the length of the longest substring of s consisting of only one repeating character after the ith query is performed.

 

Example 1:

Input: s = "babacc", queryCharacters = "bcb", queryIndices = [1,3,3]
Output: [3,3,4]
Explanation: 
- 1st query updates s = "bbbacc". The longest substring consisting of one repeating character is "bbb" with length 3.
- 2nd query updates s = "bbbccc". 
  The longest substring consisting of one repeating character can be "bbb" or "ccc" with length 3.
- 3rd query updates s = "bbbbcc". The longest substring consisting of one repeating character is "bbbb" with length 4.
Thus, we return [3,3,4].

Example 2:

Input: s = "abyzz", queryCharacters = "aa", queryIndices = [2,1]
Output: [2,3]
Explanation:
- 1st query updates s = "abazz". The longest substring consisting of one repeating character is "zz" with length 2.
- 2nd query updates s = "aaazz". The longest substring consisting of one repeating character is "aaa" with length 3.
Thus, we return [2,3].

class Solution {
public:
    vector<int> longestRepeating(string s, string queryCharacters, vector<int>& queryIndices) {
        
    }
};

*/

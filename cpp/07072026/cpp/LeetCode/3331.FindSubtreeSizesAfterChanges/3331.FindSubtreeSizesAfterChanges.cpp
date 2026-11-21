/*
3331. Find Subtree Sizes After Changes
Medium
Topics
premium lock iconCompanies
Hint

You are given a tree rooted at node 0 that consists of n nodes numbered from 0 to n - 1. The tree is represented by an array parent of size n, where parent[i] is the parent of node i. Since node 0 is the root, parent[0] == -1.

You are also given a string s of length n, where s[i] is the character assigned to node i.

We make the following changes on the tree one time simultaneously for all nodes x from 1 to n - 1:

    Find the closest node y to node x such that y is an ancestor of x, and s[x] == s[y].
    If node y does not exist, do nothing.
    Otherwise, remove the edge between x and its current parent and make node y the new parent of x by adding an edge between them.

Return an array answer of size n where answer[i] is the size of the subtree rooted at node i in the final tree.

 

Example 1:

Input: parent = [-1,0,0,1,1,1], s = "abaabc"

Output: [6,3,1,1,1,1]

Explanation:

The parent of node 3 will change from node 1 to node 0.

Example 2:

Input: parent = [-1,0,4,0,1], s = "abbba"

Output: [5,2,1,1,1]

Explanation:

The following changes will happen at the same time:

    The parent of node 4 will change from node 1 to node 0.
    The parent of node 2 will change from node 4 to node 1.


class Solution {
public:
    vector<int> findSubtreeSizes(vector<int>& parent, string s) {
        
    }
};




3331. Find Subtree Sizes After Changes
Explanation

We need to rewire edges in a tree and then compute subtree sizes.
Step 1: Reparenting

For each node x (from 1 to n-1), we find the closest ancestor y of x (including possibly x itself? No — y must be a proper ancestor, but the problem says "ancestor" — let me check) such that s[y] == s[x]. If found, change x's parent to y. This is done simultaneously for all nodes, meaning the new parent is based on the original tree, not the updated one.

Wait, "closest node y to node x such that y is an ancestor of x" — ancestor typically includes x itself? If y = x, we'd be setting parent of x to itself, which is invalid. So y must be a proper ancestor (strict ancestor).

The example confirms: for parent = [-1,0,0,1,1,1], s = "abaabc":

    Node 3 has s[3] = 'a', original parent is 1 (s[1] = 'b'). Ancestors of 3: 1 (b), 0 (a). Closest with s = 'a' is 0. So parent changes from 1 to 0. ✅

Step 2: Computing the Closest Ancestor with Same Character

We can do a DFS from the root, maintaining the last occurrence of each character along the root-to-current path.

For each node x:

    Before recursing into children, record lastSeen[s[x]] = x (save old value to restore later).

    For node x itself, its closest ancestor with same char is lastSeen[s[x]] before updating lastSeen for x. Wait — we want the closest proper ancestor, so we need lastSeen from before x was added.

So the order is:

    Query y = lastSeen[s[x]] (this is the closest proper ancestor with same char).

    If y != -1, set newParent[x] = y.

    Update lastSeen[s[x]] = x.

    Recurse into children.

    Restore lastSeen[s[x]] = old (for sibling branches).

Step 3: Compute Subtree Sizes

After building the newParent array, we can compute subtree sizes by processing nodes in reverse topological order (from leaves to root). Since parent indices are always less than child indices? Not necessarily after reparenting — newParent[x] is an ancestor of x, so it's always less than x in depth, but not necessarily in index. However, in a tree rooted at 0, if we process nodes in decreasing depth order, we can accumulate sizes.

Simpler: Build children lists from newParent, then do a post-order DFS to compute sizes.
Algorithm

    Build adjacency list children from original parent.

    DFS from root, maintaining lastSeen[26] = last node on path with each character.

        For node u:

            y = lastSeen[s[u] - 'a']

            newParent[u] = (y != -1) ? y : parent[u] (if no ancestor, keep original parent)

            Save old lastSeen[s[u]], set to u

            Recurse children

            Restore lastSeen[s[u]]

    Build newChildren from newParent.

    Post-order DFS to compute subtree sizes.

    Return sizes.

Example 1: parent = [-1,0,0,1,1,1], s = "abaabc"

Original tree:
text

    0(a)
   / \
  1(b) 2(a)
 /|\
3(a)4(a)5(b)   wait, s = "a b a a b c" → indices 0:a, 1:b, 2:a, 3:a, 4:b, 5:c

Let me redo: s = "abaabc" → s[0]='a', s[1]='b', s[2]='a', s[3]='a', s[4]='b', s[5]='c'.

Parent array: parent[0]=-1, parent[1]=0, parent[2]=0, parent[3]=1, parent[4]=1, parent[5]=1.

Tree:
text

        0(a)
       /    \
     1(b)   2(a)
    / | \
  3(a)4(b)5(c)

DFS with lastSeen:

    Node 0 (a): lastSeen[a] before = -1 → newParent[0] = -1 (root). Set lastSeen[a] = 0.

        Node 1 (b): lastSeen[b] before = -1 → newParent[1] = parent[1] = 0. Set lastSeen[b] = 1.

            Node 3 (a): lastSeen[a] before = 0 → newParent[3] = 0 (changed!). Set lastSeen[a] = 3. Restore to 0.

            Node 4 (b): lastSeen[b] before = 1 → newParent[4] = 1 (same as original). Set lastSeen[b] = 4. Restore to 1.

            Node 5 (c): lastSeen[c] before = -1 → newParent[5] = parent[5] = 1. Set lastSeen[c] = 5. Restore to -1.

            Restore lastSeen[b] = -1.

        Node 2 (a): lastSeen[a] before = 0 → newParent[2] = 0 (same as original). Set lastSeen[a] = 2. Restore to 0.

        Restore lastSeen[a] = -1.

New parent: [-1, 0, 0, 0, 1, 1].

New tree:
text

        0
      / | \
     1  2  3
    / \
   4   5

Subtree sizes:

    Node 3: 1

    Node 4: 1

    Node 5: 1

    Node 1: 1 + 1 + 1 = 3

    Node 2: 1

    Node 0: 1 + 3 + 1 + 1 = 6

Result: [6, 3, 1, 1, 1, 1] ✅
Example 2: parent = [-1,0,4,0,1], s = "abbba"

Tree:
text

    0(a)
   / \
  1(b)3(b)
 /
4(a)
|
2(b)

Wait, parent[2]=4, so 2 is child of 4. Let me redo:

    0: root

    1: parent 0

    2: parent 4

    3: parent 0

    4: parent 1

Tree:
text

      0(a)
     / \
   1(b) 3(b)
   /
 4(a)
 /
2(b)

lastSeen DFS:

    Node 0 (a): newParent[0] = -1. Set lastSeen[a] = 0.

        Node 1 (b): newParent[1] = 0. Set lastSeen[b] = 1.

            Node 4 (a): lastSeen[a] = 0 → newParent[4] = 0 (changed from 1). Set lastSeen[a] = 4. Restore to 0.

                Node 2 (b): lastSeen[b] = 1 → newParent[2] = 1 (changed from 4). Set lastSeen[b] = 2. Restore to 1.

            Restore lastSeen[a] = 0.

            Restore lastSeen[b] = -1.

        Node 3 (b): lastSeen[b] = -1 → newParent[3] = 0. Set lastSeen[b] = 3. Restore to -1.

        Restore lastSeen[a] = -1.

New parent: [-1, 0, 1, 0, 0].

New tree:
text

      0
    / | \
   1  3  4
  /
 2

Subtree sizes:

    Node 2: 1

    Node 1: 1 + 1 = 2

    Node 3: 1

    Node 4: 1

    Node 0: 1 + 2 + 1 + 1 = 5

Result: [5, 2, 1, 1, 1] ✅
Final Code
cpp
*/
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution {
    vector<vector<int>> children;
    vector<int> newParent;
    int lastSeen[26];
    
    void dfs(int u, vector<int>& parent, string& s) {
        int c = s[u] - 'a';
        int old = lastSeen[c];
        
        // Closest proper ancestor with same char
        if (old != -1) {
            newParent[u] = old;
        } else {
            newParent[u] = parent[u];  // keep original parent
        }
        
        // Update lastSeen for children
        lastSeen[c] = u;
        
        for (int v : children[u]) {
            dfs(v, parent, s);
        }
        
        // Restore for siblings
        lastSeen[c] = old;
    }
    
    int computeSize(int u, vector<vector<int>>& newChildren, vector<int>& ans) {
        int size = 1;
        for (int v : newChildren[u]) {
            size += computeSize(v, newChildren, ans);
        }
        ans[u] = size;
        return size;
    }
    
public:
    vector<int> findSubtreeSizes(vector<int>& parent, string s) {
        int n = (int)parent.size();
        
        // Build original children
        children.assign(n, {});
        for (int i = 1; i < n; i++) {
            children[parent[i]].push_back(i);
        }
        
        // Step 1: compute new parents via DFS with lastSeen
        newParent.assign(n, -1);
        fill(begin(lastSeen), end(lastSeen), -1);
        dfs(0, parent, s);
        
        // Step 2: build new children lists
        vector<vector<int>> newChildren(n);
        for (int i = 1; i < n; i++) {
            newChildren[newParent[i]].push_back(i);
        }
        
        // Step 3: compute subtree sizes via post-order DFS
        vector<int> ans(n, 0);
        computeSize(0, newChildren, ans);
        
        return ans;
    }
};

int main() {
    Solution sol;

    // Test 1
    vector<int> parent1 = {-1, 0, 0, 1, 1, 1};
    string s1 = "abaabc";
    vector<int> ans1 = sol.findSubtreeSizes(parent1, s1);
    cout << "Test 1: [";
    for (int i = 0; i < (int)ans1.size(); i++) {
        cout << ans1[i];
        if (i + 1 < (int)ans1.size()) cout << ",";
    }
    cout << "] (expected [6,3,1,1,1,1])" << endl;

    // Test 2
    vector<int> parent2 = {-1, 0, 4, 0, 1};
    string s2 = "abbba";
    vector<int> ans2 = sol.findSubtreeSizes(parent2, s2);
    cout << "Test 2: [";
    for (int i = 0; i < (int)ans2.size(); i++) {
        cout << ans2[i];
        if (i + 1 < (int)ans2.size()) cout << ",";
    }
    cout << "] (expected [5,2,1,1,1])" << endl;

    return 0;
}
/*

Dry Run of Final Code
Dry Run 1: parent = [-1,0,0,1,1,1], s = "abaabc"

Step 1: DFS with lastSeen

Trace (see above). newParent = [-1, 0, 0, 0, 1, 1].

Step 2: Build newChildren

    newChildren[0] = {1, 2, 3} (from newParent[1]=0, newParent[2]=0, newParent[3]=0)

    newChildren[1] = {4, 5} (from newParent[4]=1, newParent[5]=1)

    newChildren[2..5] = {}

Step 3: Post-order size computation

    computeSize(0):

        computeSize(1):

            computeSize(4) = 1 (leaf)

            computeSize(5) = 1 (leaf)

            size[1] = 1 + 1 + 1 = 3

        computeSize(2) = 1

        computeSize(3) = 1

        size[0] = 1 + 3 + 1 + 1 = 6

Result: [6, 3, 1, 1, 1, 1] ✅
Dry Run 2: parent = [-1,0,4,0,1], s = "abbba"

Step 1: DFS

newParent = [-1, 0, 1, 0, 0].

Step 2: newChildren

    newChildren[0] = {1, 3, 4}

    newChildren[1] = {2}

    newChildren[2..4] = {}

Step 3: Sizes

    computeSize(0):

        computeSize(1):

            computeSize(2) = 1

            size[1] = 1 + 1 = 2

        computeSize(3) = 1

        computeSize(4) = 1

        size[0] = 1 + 2 + 1 + 1 = 5

Result: [5, 2, 1, 1, 1] ✅
Complexity
Time Complexity
Step	Cost
Build original children	O(n)
DFS for reparenting	O(n) — each node visited once
Build new children	O(n)
Post-order DFS for sizes	O(n)
Total	O(n)
Space Complexity
Item	Cost
children adjacency list	O(n)
newParent array	O(n)
newChildren adjacency list	O(n)
ans array	O(n)
DFS recursion stack	O(h) ≤ O(n)
lastSeen array	O(26) = O(1)
Total auxiliary	O(n)
How to Explain This to an Interviewer

    Problem restatement: "For each node, find its closest ancestor with the same character and reparent it there. Then compute subtree sizes."

    Step 1 — Reparenting: "I do a DFS from the root, maintaining lastSeen[c] = the most recent node on the current root-to-node path with character c. When entering node x, I query lastSeen[s[x]] to find its closest proper ancestor with the same character. Then I set lastSeen[s[x]] = x before recursing, and restore it after — so siblings don't see each other's updates. This gives the closest ancestor correctly."

    Step 2 — Subtree sizes: "After building the new parent array, I build the new children lists and do a post-order DFS to compute subtree sizes."

    Time complexity: "Both DFS passes visit each node once, so O(n) total."

    Space complexity: "We use O(n) for the adjacency lists, parent array, and answer array. The recursion stack is O(h) ≤ O(n). The lastSeen array is O(26) = O(1). So total auxiliary space is O(n)."

Summary
Aspect	Detail
Problem	Reparent nodes to closest same-char ancestor, then compute subtree sizes
Step 1	DFS with lastSeen[26] to find closest same-char ancestor
Step 2	Build new children, post-order DFS for sizes
Time	O(n)
Space	O(n)
The "Aha" Moment

The lastSeen array during DFS is the key trick: it tracks the most recent node on the current path for each character. Since DFS explores one root-to-leaf path at a time, lastSeen[c] at node x is exactly the closest ancestor of x with character c. Restoring lastSeen after recursing ensures siblings are processed with the correct path state. This turns an O(n²) ancestor search into O(n) overall.
Final Code (Copy-Paste)
cpp

class Solution {
    vector<vector<int>> children;
    vector<int> newParent;
    int lastSeen[26];
    
    void dfs(int u, vector<int>& parent, string& s) {
        int c = s[u] - 'a';
        int old = lastSeen[c];
        newParent[u] = (old != -1) ? old : parent[u];
        lastSeen[c] = u;
        for (int v : children[u]) dfs(v, parent, s);
        lastSeen[c] = old;
    }
    
    int computeSize(int u, vector<vector<int>>& newChildren, vector<int>& ans) {
        int size = 1;
        for (int v : newChildren[u]) size += computeSize(v, newChildren, ans);
        ans[u] = size;
        return size;
    }
    
public:
    vector<int> findSubtreeSizes(vector<int>& parent, string s) {
        int n = parent.size();
        children.assign(n, {});
        for (int i = 1; i < n; i++) children[parent[i]].push_back(i);
        
        newParent.assign(n, -1);
        fill(begin(lastSeen), end(lastSeen), -1);
        dfs(0, parent, s);
        
        vector<vector<int>> newChildren(n);
        for (int i = 1; i < n; i++) newChildren[newParent[i]].push_back(i);
        
        vector<int> ans(n, 0);
        computeSize(0, newChildren, ans);
        return ans;
    }
};

🎯 Time: O(n) | Space: O(n)

*/

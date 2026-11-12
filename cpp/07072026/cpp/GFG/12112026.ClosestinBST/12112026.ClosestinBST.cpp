#include <iostream>
#include <climits>
#include <cstdlib>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = right = nullptr;
    }
};

class Solution {
public:
    int minDiff(Node *root, int k) {
        int minDiff = INT_MAX;
        Node* curr = root;

        while (curr != nullptr) {
            int diff = abs(curr->data - k);
            if (diff < minDiff) minDiff = diff;
            if (minDiff == 0) return 0;  // early exit

            if (curr->data > k) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        return minDiff;
    }
};

int main() {
    Solution sol;

    // Test 1
    {
        // Build tree: [10, 2, 11, 1, 5, N, N, N, N, 3, 6, N, 4]
        Node* root = new Node(10);
        root->left = new Node(2);
        root->right = new Node(11);
        root->left->left = new Node(1);
        root->left->right = new Node(5);
        root->left->right->left = new Node(3);
        root->left->right->right = new Node(6);
        root->left->right->left->right = new Node(4);

        cout << "Test 1: " << sol.minDiff(root, 13) << " (expected 2)" << endl;
    }

    // Test 2
    {
        // Build tree: [8, 1, 9, N, 4, N, 10, 3]
        Node* root = new Node(8);
        root->left = new Node(1);
        root->right = new Node(9);
        root->left->right = new Node(4);
        root->right->right = new Node(10);
        root->left->right->left = new Node(3);

        cout << "Test 2: " << sol.minDiff(root, 9) << " (expected 0)" << endl;
    }

    // Test 3: single node
    {
        Node* root = new Node(5);
        cout << "Test 3: " << sol.minDiff(root, 10) << " (expected 5)" << endl;
    }

    // Test 4: k smaller than all
    {
        Node* root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        cout << "Test 4: " << sol.minDiff(root, 1) << " (expected 4)" << endl;
        // Closest is 5, diff = 4.
    }

    // Test 5: k larger than all
    {
        Node* root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        cout << "Test 5: " << sol.minDiff(root, 20) << " (expected 5)" << endl;
        // Closest is 15, diff = 5.
    }

    return 0;
}


/*

Closest in BST
Difficulty: MediumAccuracy: 47.51%Submissions: 93K+Points: 4Average Time: 30m

Given root of a Binary Search Tree (BST) and an integer k, find the minimum absolute difference between k and the value of any node in the BST.

Examples:

Input: root = [10, 2, 11, 1, 5, N, N, N, N, 3, 6, N, 4], k = 13

Output: 2
Explanation: The node with value 11 is closest to k = 13. So, the minimum absolute difference is |11 - 13| = 2.

Input: root = [8, 1, 9, N, 4, N, 10, 3], k = 9

Output: 0
Explanation: The node with value 9 is closest to k = 9. So, the minimum absolute difference is |9 - 9| = 0.

 Structure of a Binary Search Tree node
class Node {
  public:
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = right = nullptr;
    }
};


class Solution {
  public:
    int minDiff(Node *root, int k) {
        // code here
        
    }
};


Closest in BST
Explanation

We need to find the node in a BST whose value has the minimum absolute difference with a given integer k. In other words, find the value closest to k.
Key Insight — BST Property for Pruning

A BST has the property:

    All values in the left subtree are smaller than the node.

    All values in the right subtree are greater than the node.

This lets us prune the search:

    If the current node's value is > k, then k is somewhere to the left (or the current node is the closest), so we should go left.

    If the current node's value is < k, then k is somewhere to the right, so we should go right.

    If the current node's value == k, the difference is 0 — return immediately.

Algorithm

We can do this iteratively or recursively. The idea is:

    Initialize minDiff = INT_MAX.

    Start at root.

    While current != nullptr:

        Update minDiff = min(minDiff, abs(current->data - k)).

        If current->data == k → return 0 (best possible).

        If current->data > k → go left (smaller values might be closer).

        Else → go right (larger values might be closer).

    Return minDiff.

Complexity

    Time: O(h) where h is the height of the BST. For a balanced BST, O(log n); for a skewed BST, O(n).

    Space: O(1) iterative, O(h) recursive.

Example 1: root = [10, 2, 11, 1, 5, N, N, N, N, 3, 6, N, 4], k = 13

Tree structure (level-order):
text

        10
       /  \
      2    11
     / \
    1   5
       / \
      3   6
       \
        4

    Start at 10. |10-13| = 3. minDiff = 3. 10 < 13 → go right to 11.

    At 11. |11-13| = 2. minDiff = min(3, 2) = 2. 11 < 13 → go right → null.

    Loop ends. Return 2 ✅

Example 2: root = [8, 1, 9, N, 4, N, 10, 3], k = 9
text

        8
       / \
      1   9
       \   \
        4   10
       /
      3

    Start at 8. |8-9| = 1. minDiff = 1. 8 < 9 → go right to 9.

    At 9. |9-9| = 0. minDiff = 0. Return 0 ✅


Dry Run of Final Code
Dry Run 1: k = 13, tree from Example 1

Initial: minDiff = INT_MAX, curr = 10.

| Step | curr->data | diff = |curr - 13| | minDiff | Compare | Next |
|------|------------|---------------------|---------|---------|------|
| 1 | 10 | 3 | 3 | 10 < 13 | go right → 11 |
| 2 | 11 | 2 | 2 | 11 < 13 | go right → null |
| 3 | null | — | — | loop ends | — |

Result: 2 ✅
Dry Run 2: k = 9, tree from Example 2

Initial: minDiff = INT_MAX, curr = 8.
Step	curr->data	diff	minDiff	Compare	Next
1	8	1	1	8 < 9	go right → 9
2	9	0	0	minDiff == 0 → return 0	

Result: 0 ✅
Dry Run 3: Single node 5, k = 10

    curr = 5, diff = |5-10| = 5, minDiff = 5.

    5 < 10 → go right → null.

    Return 5 ✅

Dry Run 4: k = 1, tree: 10 (root), left 5, right 15

    curr = 10, diff = 9, minDiff = 9. 10 > 1 → go left → 5.

    curr = 5, diff = 4, minDiff = 4. 5 > 1 → go left → null.

    Return 4 ✅

Dry Run 5: k = 20, same tree

    curr = 10, diff = 10, minDiff = 10. 10 < 20 → go right → 15.

    curr = 15, diff = 5, minDiff = 5. 15 < 20 → go right → null.

    Return 5 ✅

Complexity
Metric	Value	Reasoning
Time	O(h)	Follows one path from root to leaf; h = height of BST
Space	O(1)	Iterative, no recursion stack

For a balanced BST, h = O(log n). For a skewed BST, h = O(n).
Alternative — In-Order Traversal

An in-order traversal of a BST visits nodes in sorted order. We could find the closest value by tracking the predecessor and successor of k during in-order traversal. But that's O(n) time and O(h) space — worse than the O(h) BST-search approach.
Summary
Aspect	Detail
Problem	Find node in BST closest to k (min absolute difference)
Key insight	BST property allows pruning: go left if curr > k, right if curr < k
Approach	Iterative BST search, track min difference
Time	O(h) — path from root to leaf
Space	O(1) iterative
Early exit	Return 0 if exact match found
The "Aha" Moment

The BST property isn't just for searching exact values — it also tells us which direction to move to get closer to k. If the current node is greater than k, any node further right is even greater, so the closest value must be in the left subtree (or the current node itself). This pruning reduces the search to a single root-to-leaf path, giving O(h) time instead of O(n).
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int minDiff(Node *root, int k) {
        int minDiff = INT_MAX;
        Node* curr = root;
        while (curr != nullptr) {
            int diff = abs(curr->data - k);
            if (diff < minDiff) minDiff = diff;
            if (minDiff == 0) return 0;
            if (curr->data > k) curr = curr->left;
            else curr = curr->right;
        }
        return minDiff;
    }
};

🎯 Time: O(h) | Space: O(1)


*/

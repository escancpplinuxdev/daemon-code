#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

// Definition for a binary tree node
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    int averageOfSubtree(TreeNode* root) {
        int count = 0;
        dfs(root, count);
        return count;
    }
    
private:
    // Returns pair: {sum of subtree, number of nodes in subtree}
    pair<int, int> dfs(TreeNode* node, int& count) {
        if (node == nullptr) {
            return {0, 0};
        }
        
        // Get left subtree info
        pair<int, int> left = dfs(node->left, count);
        // Get right subtree info
        pair<int, int> right = dfs(node->right, count);
        
        // Calculate total sum and count for this subtree
        int totalSum = node->val + left.first + right.first;
        int totalCount = 1 + left.second + right.second;
        
        // Check if node value equals average of subtree
        if (node->val == totalSum / totalCount) {
            count++;
        }
        
        return {totalSum, totalCount};
    }
};

// ============================================================
// Helper Functions for Testing
// ============================================================

// Build tree from level-order array (LeetCode format)
TreeNode* buildTree(vector<int>& values) {
    if (values.empty() || values[0] == -1) return nullptr;
    
    TreeNode* root = new TreeNode(values[0]);
    queue<TreeNode*> q;
    q.push(root);
    
    int i = 1;
    while (!q.empty() && i < values.size()) {
        TreeNode* current = q.front();
        q.pop();
        
        // Left child
        if (i < values.size() && values[i] != -1) {
            current->left = new TreeNode(values[i]);
            q.push(current->left);
        }
        i++;
        
        // Right child
        if (i < values.size() && values[i] != -1) {
            current->right = new TreeNode(values[i]);
            q.push(current->right);
        }
        i++;
    }
    
    return root;
}

// Print tree (in-order) for verification
void printInOrder(TreeNode* root) {
    if (!root) return;
    printInOrder(root->left);
    cout << root->val << " ";
    printInOrder(root->right);
}

// Free tree memory
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// ============================================================
// Main Function
// ============================================================

int main() {
    Solution solution;
    
    // ============================================================
    // Example 1: [4,8,5,0,1,null,6]
    // ============================================================
    cout << "========== Example 1 ==========" << endl;
    vector<int> values1 = {4, 8, 5, 0, 1, -1, 6};
    TreeNode* root1 = buildTree(values1);
    
    cout << "Tree (in-order): ";
    printInOrder(root1);
    cout << endl;
    
    int result1 = solution.averageOfSubtree(root1);
    cout << "Output: " << result1 << endl;
    cout << "Expected: 5" << endl;
    cout << endl;
    
    freeTree(root1);
    
    // ============================================================
    // Example 2: [1]
    // ============================================================
    cout << "========== Example 2 ==========" << endl;
    vector<int> values2 = {1};
    TreeNode* root2 = buildTree(values2);
    
    cout << "Tree (in-order): ";
    printInOrder(root2);
    cout << endl;
    
    int result2 = solution.averageOfSubtree(root2);
    cout << "Output: " << result2 << endl;
    cout << "Expected: 1" << endl;
    cout << endl;
    
    freeTree(root2);
    
    // ============================================================
    // Additional Test Cases
    // ============================================================
    cout << "========== Additional Tests ==========" << endl;
    
    // Test 3: [1,2,3]
    vector<int> values3 = {1, 2, 3};
    TreeNode* root3 = buildTree(values3);
    cout << "Test 3: [1,2,3] -> " << solution.averageOfSubtree(root3) << endl;
    // Expected: 2 (node 2: avg=2/1=2, node 1: avg=(1+2+3)/3=2, no; node 3: avg=3)
    // Wait: node 1 has val=1, subtree avg = (1+2+3)/3 = 2 ≠ 1
    // node 2 has val=2, subtree avg = 2/1 = 2 ✓
    // node 3 has val=3, subtree avg = 3/1 = 3 ✓
    // So result = 2
    freeTree(root3);
    
    // Test 4: [1,2,3,4,5,6,7]
    vector<int> values4 = {1, 2, 3, 4, 5, 6, 7};
    TreeNode* root4 = buildTree(values4);
    cout << "Test 4: [1,2,3,4,5,6,7] -> " << solution.averageOfSubtree(root4) << endl;
    freeTree(root4);
    
    // Test 5: All same values [5,5,5]
    vector<int> values5 = {5, 5, 5};
    TreeNode* root5 = buildTree(values5);
    cout << "Test 5: [5,5,5] -> " << solution.averageOfSubtree(root5) << endl;
    // Expected: 3 (all nodes have avg = 5)
    freeTree(root5);
    
    // Test 6: [0]
    vector<int> values6 = {0};
    TreeNode* root6 = buildTree(values6);
    cout << "Test 6: [0] -> " << solution.averageOfSubtree(root6) << endl;
    // Expected: 1
    freeTree(root6);
    
    return 0;
}

/*

2265. Count Nodes Equal to Average of Subtree
Medium
Topics
premium lock iconCompanies
Hint

Given the root of a binary tree, return the number of nodes where the value of the node is equal to the average of the values in its subtree.

Note:

    The average of n elements is the sum of the n elements divided by n and rounded down to the nearest integer.
    A subtree of root is a tree consisting of root and all of its descendants.

 

Example 1:

Input: root = [4,8,5,0,1,null,6]
Output: 5
Explanation: 
For the node with value 4: The average of its subtree is (4 + 8 + 5 + 0 + 1 + 6) / 6 = 24 / 6 = 4.
For the node with value 5: The average of its subtree is (5 + 6) / 2 = 11 / 2 = 5.
For the node with value 0: The average of its subtree is 0 / 1 = 0.
For the node with value 1: The average of its subtree is 1 / 1 = 1.
For the node with value 6: The average of its subtree is 6 / 1 = 6.

Example 2:

Input: root = [1]
Output: 1
Explanation: For the node with value 1: The average of its subtree is 1 / 1 = 1.
*/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
/*
class Solution {
public:
    int averageOfSubtree(TreeNode* root) {
               
    }
};

give this with int main with proper header files

*/

/*
In DFS (Depth-First Search), the terms Preorder, Postorder, and Inorder refer to the specific sequence or timing in which you "visit" (process/print) a node relative to exploring its children.
While these terms are most famous for Binary Trees, they also apply broadly to Graphs and general DFS traversal algorithms.

1. Binary Tree Traversals (The Core Definition)
In a binary tree, every node has at most two children: a Left child and a Right child. The naming tells you exactly where the Current Node (Root) is processed:

🟩 Preorder Traversal (Root \(\rightarrow \) Left \(\rightarrow \) Right)
• The Rule: You visit the current node before you dive into its children.
• Order: Root \(\rightarrow \) Left Child \(\rightarrow \) Right Child
• Use Case: Used to duplicate/clone a tree structure or get a prefix math notation.

🟨 Inorder Traversal (Left \(\rightarrow \) Root \(\rightarrow \) Right)
• The Rule: You visit the current node in between visiting the left child and the right child.
• Order: Left Child \(\rightarrow \) Root \(\rightarrow \) Right Child
• Crucial Property: If you run an Inorder traversal on a Binary Search Tree (BST), the nodes will automatically print in sorted ascending order.

🟥 Postorder Traversal (Left \(\rightarrow \) Right \(\rightarrow \) Root)
• The Rule: You visit the current node after you have completely finished exploring all of its children.
• Order: Left Child \(\rightarrow \) Right Child \(\rightarrow \) Root
• Use Case: Used to delete a tree cleanly (you must delete the children before you delete the root) or to compute folder/file sizes on a hard drive (bottom-up accumulation).

struct Node {
    int data;
    Node* left;
    Node* right;
};

// 1. PREORDER: Process first
void preOrder(Node* root) {
    if (!root) return;
    std::cout << root->data << " "; // <-- VISIT FIRST
    preOrder(root->left);
    preOrder(root->right);
}

// 2. INORDER: Process in the middle
void inOrder(Node* root) {
    if (!root) return;
    inOrder(root->left);
    std::cout << root->data << " "; // <-- VISIT MIDDLE
    inOrder(root->right);
}

// 3. POSTORDER: Process at the very end
void postOrder(Node* root) {
    if (!root) return;
    postOrder(root->left);
    postOrder(root->right);
    std::cout << root->data << " "; // <-- VISIT LAST
}

*/


#include <iostream>
#include <vector>

// Structure for a binary tree node
struct Node
{
  int data;
  Node *left;
  Node *right;

  Node(int val)
  {
    data = val;
    left = nullptr;
    right = nullptr;
  }
};

// 🟩 PREORDER: Root -> Left -> Right
void getPreorder(Node *root, std::vector<int> &result)
{
  if (root == nullptr)
    return;

  result.push_back(root->data);     // Visit Root
  getPreorder(root->left, result);  // Traverse Left
  getPreorder(root->right, result); // Traverse Right
}

// 🟨 INORDER: Left -> Root -> Right
void getInorder(Node *root, std::vector<int> &result)
{
  if (root == nullptr)
    return;

  getInorder(root->left, result);  // Traverse Left
  result.push_back(root->data);    // Visit Root
  getInorder(root->right, result); // Traverse Right
}

// 🟥 POSTORDER: Left -> Right -> Root
void getPostorder(Node *root, std::vector<int> &result)
{
  if (root == nullptr)
    return;

  getPostorder(root->left, result);  // Traverse Left
  getPostorder(root->right, result); // Traverse Right
  result.push_back(root->data);      // Visit Root
}

// Helper function to cleanly print vectors
void printVector(const std::string &label, const std::vector<int> &vec)
{
  std::cout << label << ": [";
  for (size_t i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i];
    if (i < vec.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}

int main()
{
  // --- 1. BUILD THE EXACT TREE STRUCTURE ---
  //        1
  //      /   \
    //     2     3
  //    / \   /
  //   4   5 6
  Node *root = new Node(1);
  root->left = new Node(2);
  root->right = new Node(3);
  root->left->left = new Node(4);
  root->left->right = new Node(5);
  root->right->left = new Node(6);

  // --- 2. RUN TRAVERSALS AND CAPTURE ARRAYS ---
  std::vector<int> preorderResult;
  std::vector<int> inorderResult;
  std::vector<int> postorderResult;

  getPreorder(root, preorderResult);
  getInorder(root, inorderResult);
  getPostorder(root, postorderResult);

  // --- 3. PRINT OUTPUT ---
  printVector("🟩 Preorder ", preorderResult);
  printVector("🟨 Inorder  ", inorderResult);
  printVector("🟥 Postorder", postorderResult);

  // --- 4. CLEAN UP MEMORY ---
  delete root->right->left;
  delete root->left->right;
  delete root->left->left;
  delete root->right;
  delete root->left;
  delete root;

  return 0;
}

/*

Part 1: Visual Sample Tree & Output Comparison
Let's use this sample binary tree layout for our example:
text
       1
     /   \
    2     3
   / \   /
  4   5 6
Use code with caution.
If we track how a single DFS path walks completely around the tree, it hits each node at three distinct times: on the way down (Preorder), in the middle (Inorder), and on the way up (Postorder).
Here are the exact output arrays matching this tree structure:
Traversal Type	Core Formula	Step-by-Step Processing Path	Final Output Array

🟩 Preorder	Root ➔ Left ➔ Right	Hit 1 first, then move left to 2, then 4, come up to 5, then right to 3 and 6.	[1, 2, 4, 5, 3, 6]

🟨 Inorder	Left ➔ Root ➔ Right	Go to furthest left 4, step up to parent 2, drop to 5, step up to root 1, drop left to 6, up to 3.	[4, 2, 5, 1, 6, 3]

🟥 Postorder	Left ➔ Right ➔ Root	Left child 4 completes, right child 5 completes, parent 2 completes. Left child 6 completes, parent 3 completes. Root 1 finishes last.	[4, 5, 2, 6, 3, 1]

*/



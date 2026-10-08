#include <iostream>

// Your explicit struct definition
struct Node
{
  int data;
  Node *left;
  Node *right;
};

// Helper function to easily create new tree nodes
Node *createNode(int value)
{
  Node *newNode = new Node();
  newNode->data = value;
  newNode->left = nullptr;
  newNode->right = nullptr;
  return newNode;
}

// 1. PREORDER: Process first
void preOrder(Node *root)
{
  if (!root)
    return;
  std::cout << root->data << " "; // <-- VISIT FIRST
  preOrder(root->left);
  preOrder(root->right);
}

// 2. INORDER: Process in the middle
void inOrder(Node *root)
{
  if (!root)
    return;
  inOrder(root->left);
  std::cout << root->data << " "; // <-- VISIT MIDDLE
  inOrder(root->right);
}

// 3. POSTORDER: Process at the very end
void postOrder(Node *root)
{
  if (!root)
    return;
  postOrder(root->left);
  postOrder(root->right);
  std::cout << root->data << " "; // <-- VISIT LAST
}

// Helper function to safely clean up allocated heap memory
void freeTree(Node *root)
{
  if (!root)
    return;
  freeTree(root->left);
  freeTree(root->right);
  delete root;
}

int main()
{
  // --- 1. BUILD THE EXACT TARGET TREE ---
  //        1
  //      /   \
    //     2     3
  //    / \   /
  //   4   5 6
  Node *root = createNode(1);
  root->left = createNode(2);
  root->right = createNode(3);
  root->left->left = createNode(4);
  root->left->right = createNode(5);
  root->right->left = createNode(6);

  // --- 2. TRAVERSE AND PRINT OUTPUTS VIA YOUR LOGIC ---
  std::cout << "🟩 Preorder : [ ";
  preOrder(root);
  std::cout << "]\n";

  std::cout << "🟨 Inorder  : [ ";
  inOrder(root);
  std::cout << "]\n";

  std::cout << "🟥 Postorder: [ ";
  postOrder(root);
  std::cout << "]\n";

  // --- 3. CLEAN UP ALLOCATIONS ---
  freeTree(root);

  return 0;
}

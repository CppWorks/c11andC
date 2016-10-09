#ifndef NODE_H
#define NODE_H
struct Node
{
    int data;
    struct Node* left, *right;
};
#endif
// http://www.geeksforgeeks.org/check-whether-binary-tree-full-binary-tree-not/
/* Helper function that allocates a new node with the
given key and NULL left and right pointer. */
struct Node *newNode(char k)
{
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->data = (char)k;
    node->right = node->left = NULL;
    return node;
}


/* This function tests if a binary tree is a full binary tree. */
bool isFullTree(struct Node* root)
{
    // If empty tree
    if (root == NULL)
        return true;

    // If leaf node
    if (root->left == NULL && root->right == NULL)
        return true;

    // If both left and right are not NULL, and left & right subtrees
    // are full
    if ((root->left) && (root->right))
        return (isFullTree(root->left) && isFullTree(root->right));

    // We reach here when none of the above if conditions work
    return false;
}
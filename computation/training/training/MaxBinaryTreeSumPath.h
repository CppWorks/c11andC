/*For each node there can be four ways that the max path goes through the node :
1. Node only
2. Max path through Left Child + Node
3. Max path through Right Child + Node
4. Max path through Left Child + Node + Max path through Right Child*/
// path can be symmetric, so only consider path start from 'left side'
// C/C++ program to find maximum path sum in Binary Tree
/*The idea is to keep trace of four paths and pick up the max one in the end.
An important thing to note is, root of every subtree need to return maximum path sum such that at most one child of root is involved. 
This is needed for parent function call. In below code, this sum is stored in ‘max_single’ and returned by the recursive function.*/
#include <algorithm>    // std::min
using namespace std;

// A binary tree node
#ifndef NODE_H
#define NODE_H
struct Node
{
    int data;
    struct Node* left, *right;
};

// A utility function to allocate a new node
struct Node* newNode(int data)
{
    struct Node* newNode = new Node;
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return (newNode);
}
#endif
// This function returns overall maximum path sum in 'res'
// And returns max path sum going through root.
int findMaxUtil(Node* root, int &res)
{
    //Base Case
    if (root == NULL) // this is an end of leaf
        return 0;

    // l and r store maximum path sum going through left and
    // right child of root respectively
    int l = findMaxUtil(root->left, res); // go to the most left...
    int r = findMaxUtil(root->right, res); // go to the right child of current sub tree root.

    // Max path for parent call of root. This path must
    // include at-most one child of root
    int max_single = max(max(l, r) + root->data, root->data);

    // Max Top represents the sum when the Node under
    // consideration is the root of the maxsum path and no
    // ancestors of root are there in max sum path
    int max_top = max(max_single, l + r + root->data);

    res = max(res, max_top); // Store the Maximum Result.

    return max_single; // this is the key..we only return max single...one child to extend to the root
}

// Returns maximum path sum in tree with given root
int findMaxSum(Node *root)
{
    // Initialize result
    int res = INT_MIN;

    // Compute and return result
    findMaxUtil(root, res);
    return res;
}
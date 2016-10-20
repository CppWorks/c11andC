// PathToLeafBinaryTree.cpp : Defines the entry point for the console application.
//

// SumLeftLeaves.cpp : Defines the entry point for the console application.
//
// http://algorithms.tutorialhorizon.com/print-paths-from-root-to-all-leaf-nodes-in-a-binary-tree/
#include "stdafx.h"
#include <vector>
// C program for different tree traversals
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
/* A binary tree node has data, pointer to left child
and a pointer to right child */
struct node
{
    int data;
    struct node* left;
    struct node* right;
};


/* Helper function that allocates a new node with the
given data and NULL left and right pointers. */
struct node* newNode(int data)
{
    struct node* node = (struct node*)
        malloc(sizeof(struct node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return(node);
}

void PrintPath(vector<int>& path)
{
    for (size_t i = 0; i < path.size(); i++)
    {
        cout << path[i] << "\t";
    }
    cout << "\n";
}

void PrintPathLength(vector<int>& path, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        cout << path[i] << "\t";
    }
    cout << "\n";
}

/* Given a binary tree, print its nodes in preorder*/
// path is the path from root to current node.
void printPreorder(struct node* node, vector<int>& path)
{
    if (node == NULL)
        return;
    path.push_back(node->data);
    vector<int> oldPath = path;
    if (node->left == NULL && node->right == NULL)
    {
        PrintPath(path);
    }
    /* then recur on left sutree */
    printPreorder(node->left, path);
    path = oldPath;  // restore the path.
    /* now recur on right subtree */
    printPreorder(node->right, path);
    path = oldPath;
}

/* Given a binary tree, print its nodes in preorder*/
// path is the path from root to current node.
// This is a differnt way of printing path..use length.
void printPreorder(struct node* node, vector<int>& path, size_t length)
{
    if (node == NULL)
        return;
    if (length == path.size())
    {
        path.push_back(node->data);
    }
    else
    {
        path[length] = node->data;
    }
    length++;
    if (node->left == NULL && node->right == NULL)
    {
        PrintPathLength(path, length);
    }
    else
    {
        /* then recur on left sutree */
        printPreorder(node->left, path, length);
        /* now recur on right subtree */
        printPreorder(node->right, path, length);
    }

}
// another method is to pass path length...the path itself will overwrite the node at each length level by itself.

/* Driver program to test above functions*/
int main()
{
    //struct node *root = newNode(10);
    //root->left = newNode(5);
    //root->right = newNode(15);

    //root->left->left = newNode(10);
    //root->left->right = newNode(12);
    //root->left->right->left = newNode(6);

    //root->right->right = newNode(8);
    //root->right->left = newNode(5);

    struct node *root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);

    root->left->left = newNode(4);
    root->left->left->left = newNode(7);


    root->right->left = newNode(6);
    root->right->left->right = newNode(9);
    root->right->left->left = newNode(8);

    vector<int> path;
    printPreorder(root, path);

    printf("\n method 2\n");
    vector<int> path2;
    printPreorder(root, path2, 0);
    getchar();
    return 0;
}

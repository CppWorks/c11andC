// SumLeftLeaves.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

/* Given a binary tree, print its nodes according to the
"bottom-up" postorder traversal. */
void printPostorder(struct node* node)
{
    if (node == NULL)
        return;

    // first recur on left subtree
    printPostorder(node->left);

    // then recur on right subtree
    printPostorder(node->right);

    // now deal with the node
    printf("%d ", node->data);
}
int sum = 0;

/* Given a binary tree, print its nodes in inorder*/
void printInorder(struct node* node)
{
    if (node == NULL)
        return;

    /* first recur on left child */
    printInorder(node->left);

    /* then print the data of node */
    printf("%d ", node->data);
    // Check the child.
    // Another option is pass along the parent..to check if it is a left child
    if (node->left != NULL && node->left->left == NULL && node->left->right == NULL)
    {
        sum += node->left->data;
    }

    /* now recur on right child */
    printInorder(node->right);
}

/* Given a binary tree, print its nodes in preorder*/
void printPreorder(struct node* node)
{
    if (node == NULL)
        return;

    /* first print data of node */
    printf("%d ", node->data);

    /* then recur on left sutree */
    printPreorder(node->left);

    /* now recur on right subtree */
    printPreorder(node->right);
}

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
    root->left->right = newNode(5);
    root->left->left->left = newNode(8);
    root->left->left->right = newNode(9);

    root->right->left = newNode(6);
    root->right->right = newNode(7);
    root->right->right->left = newNode(10);
    root->right->right->right = newNode(11);

    printf("\nPreorder traversal of binary tree is \n");
    printPreorder(root);

    printf("\nInorder traversal of binary tree is \n");
    printInorder(root);

    printf("\nPostorder traversal of binary tree is \n");
    printPostorder(root);

 //   cout << "sum is left leaf " << sum << endl;
    getchar();
    return 0;
}

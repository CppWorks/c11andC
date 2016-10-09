// training.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "add-linked-list.h"
#include "minBinaryTreeHeight.h"
#include "MaxBinaryTreeSumPath.h"
#include "LinkedListInsertAtAlternate.h"
#include <iostream>
#include "RemoveCycleInList.h"
#include "FullBinaryTest.h"
#include "FindCycleInGraph.h"
// Driver program to test above functions

void AddLinkedList()
{
    node *head1 = NULL, *head2 = NULL, *result = NULL;
    int arr1[] = { 8, 9, 9 };
    int arr2[] = { 3, 8 };

    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    // Create first list as 9->9->9
    int i;
    for (i = size1 - 1; i >= 0; --i)
        push(&head1, arr1[i]);

    // Create second list as 1->8
    for (i = size2 - 1; i >= 0; --i)
        push(&head2, arr2[i]);

    addList(head1, head2, &result);

    printList(result);
}

void FindBinaryTreeHeight()
{
    // Let us construct the Tree shown in the above figure
    Node *root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    cout << "height is " << minDepth(root) << endl;;
}

void FindMaxSumValue()
{
    struct Node *root = newNode(10);
    root->left = newNode(2);
    root->right = newNode(10);
    root->left->left = newNode(20);
    root->left->right = newNode(1);
    root->right->right = newNode(-25);
    root->right->right->left = newNode(3);
    root->right->right->right = newNode(4);
    cout << "Max path sum is " << findMaxSum(root);
}

void GetLinkedListInsertAlternate()
{
    struct listNode *p = NULL, *q = NULL;
    push(&p, 3);
    push(&p, 2);
    push(&p, 1);
    printf("First Linked List:\n");
    printList(p);

    push(&q, 8);
    push(&q, 7);
    push(&q, 6);
    push(&q, 5);
    push(&q, 4);
    printf("Second Linked List:\n");
    printList(q);

    merge(p, &q);

    printf("Modified First Linked List:\n");
    printList(p);

    printf("Modified Second Linked List:\n");
    printList(q);
}

void RemoveLoopList()
{
    struct listNode *head = linkNode(50);
    head->next = linkNode(20);
    head->next->next = linkNode(15);
    head->next->next->next = linkNode(4);
    head->next->next->next->next = linkNode(10);

    /* Create a loop for testing */
    head->next->next->next->next->next = head->next->next;

    detectAndRemoveLoop(head);

    printf("Linked List after removing loop \n");
    printList(head);
}
// note for linked list, we use listnode
// for tree, we use Node
void TestBinaryFullTree()
{
    struct Node* root = NULL;
    root = newNode(10);
    root->left = newNode(20);
    root->right = newNode(30);

    root->left->right = newNode(40);
    root->left->left = newNode(50);
    root->right->left = newNode(60);
    root->right->right = newNode(70);

    root->left->left->left = newNode(80);
    root->left->left->right = newNode(90);
    root->left->right->left = newNode(80);
    root->left->right->right = newNode(90);
    root->right->left->left = newNode(80);
    root->right->left->right = newNode(90);
    root->right->right->left = newNode(80);
 //   root->right->right->right = newNode(90);

    if (isFullTree(root))
        printf("The Binary Tree is full\n");
    else
        printf("The Binary Tree is not full\n");
}

void FindCycleinGraph()
{
    /* Let us create following graph
    0
    |  \
    |    \
    1-----2 */
    int V = 3, E = 3;
    struct Graph* graph = createGraph(V, E);

    // add edge 0-1
    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;

    // add edge 1-2
    graph->edge[1].src = 1;
    graph->edge[1].dest = 2;

    // add edge 0-2
    graph->edge[2].src = 0;
    graph->edge[2].dest = 2;

    if (isCycle(graph))
        printf("graph contains cycle\n");
    else
        printf("graph doesn't contain cycle\n");

}
int main()
{

    FindCycleinGraph();

    return 0;
}
#include<stdio.h>
#include<stdlib.h>
/*
http://www.geeksforgeeks.org/detect-and-remove-loop-in-a-linked-list/
Write a function detectAndRemoveLoop() that checks whether a
given Linked List contains loop and if loop is present then removes the loop and returns true. 
And if the list doesn’t contain loop then returns false. Below diagram shows a linked list with
a loop. detectAndRemoveLoop() must change the below list to 1->2->3->4->5->NULL.*/
/* Link list node */
#ifndef LIST_NODE_H
#define LIST_NODE_H
struct listNode
{
    int data;
    struct listNode* next;
};
#endif
/* Function to remove loop. */
void removeLoop(struct listNode *, struct listNode *);

/* This function detects and removes loop in the list
If loop was there in the list then it returns 1,
otherwise returns 0 */
int detectAndRemoveLoop(struct listNode *list)
{
    struct listNode  *slow_p = list, *fast_p = list;

    while (slow_p && fast_p && fast_p->next)
    {
        slow_p = slow_p->next;
        fast_p = fast_p->next->next;

        /* If slow_p and fast_p meet at some point then there
        is a loop */
        if (slow_p == fast_p)
        {
            removeLoop(slow_p, list);

            /* Return 1 to indicate that loop is found */
            return 1;
        }
    }

    /* Return 0 to indeciate that ther is no loop*/
    return 0;
}

/* Function to remove loop.
loop_node --> Pointer to one of the loop nodes
head -->  Pointer to the start listNode of the linked list */
void removeLoop(struct listNode *loop_node, struct listNode *head)
{
    /*
    1 -> 2 -> 3
         ^     .
         5  <- 4-
    */
    struct listNode *ptr1 = loop_node;
    struct listNode *ptr2 = loop_node;

    // Count the number of nodes in loop
    unsigned int k = 1, i;
    while (ptr1->next != ptr2)
    {
        ptr1 = ptr1->next;
        k++;
    }

    // Fix one pointer to head
    ptr1 = head;

    // And the other pointer to k nodes after head
    // This is magic. coz pt2 is k steps ahead of pt1
    // whens it enters loop, it will take k steps to come
    // back to the loop start, at this moment, pt1 also arrives.
    ptr2 = head;
    for (i = 0; i < k; i++)
        ptr2 = ptr2->next;

    /*  Move both pointers at the same pace,
    they will meet at loop starting listNode */
    while (ptr2 != ptr1)
    {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    // Get pointer to the last listNode
    // Magic aye...
    ptr2 = ptr2->next;
    while (ptr2->next != ptr1)
        ptr2 = ptr2->next;

    /* Set the next listNode of the loop ending listNode
    to fix the loop */
    ptr2->next = NULL;
}

///* Function to print linked list */
//void printList(struct listNode *listNode)
//{
//    while (listNode != NULL)
//    {
//        printf("%d  ", listNode->data);
//        listNode = listNode->next;
//    }
//}

struct listNode *linkNode(int key)
{
    struct listNode *temp = new struct listNode;
    temp->data = key;
    temp->next = NULL;
    return temp;
}

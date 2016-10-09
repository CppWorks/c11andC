/*
Floyd’s Cycle-Finding Algorithm:
This is the fastest method. Traverse linked list using two pointers.  Move one pointer by one and other pointer by two. 
If these pointers meet at some listNode then there is a loop.  If pointers do not meet then linked list doesn’t have loop.
*/
// C program to detect loop in a linked list
#include<stdio.h>
#include<stdlib.h>

/* Link list listNode */
// A binary tree node
#ifndef LIST_NODE_H
#define LIST_NODE_H
struct listNode
{
    int data;
    struct listNode* next;
};
void push(struct listNode** head_ref, int new_data)
{
    /* allocate listNode */
    struct listNode* new_node =
        (struct listNode*) malloc(sizeof(struct listNode));

    /* put in the data  */
    new_node->data = new_data;

    /* link the old list off the new listNode */
    new_node->next = (*head_ref);

    /* move the head to point to the new listNode */
    (*head_ref) = new_node;
}
#endif

int detectloop(struct listNode *list)
{
    struct listNode  *slow_p = list, *fast_p = list;

    while (slow_p && fast_p && fast_p->next)
    {
        slow_p = slow_p->next;
        fast_p = fast_p->next->next;
        if (slow_p == fast_p)
        {
            printf("Found Loop");
            return 1;
        }
    }
    return 0;
}
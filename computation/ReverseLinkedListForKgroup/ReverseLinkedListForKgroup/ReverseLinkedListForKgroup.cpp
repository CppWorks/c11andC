// ReverseLinkedListForKgroup.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
// C program to reverse a linked list in groups of given size
#include<stdio.h>
#include<stdlib.h>
using namespace std;
/* Link list node */
struct node
{
    int data;
    struct node* next;
};

/* Reverses the linked list in groups of size k and returns the
pointer to the new head node. */
struct node *reverse(struct node *head, int k)
{
    struct node* current = head;
    struct node* next = NULL;
    struct node* prev = NULL;
    int count = 0;

    /*reverse first k nodes of the linked list */
    while (current != NULL && count < k)
    {
        std::cout << "current is " << current->data << std::endl;
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
        count++;
    }

    /* next is now a pointer to (k+1)th node
    Recursively call for the list starting from current.
    And make rest of the list as next of first node */
    // head->next is now 6..and head is at 1. so head->next now should be 6..which is the last node visited by the recrusive call.
    if (next != NULL)
    {
        cout << "head is " << head->data << endl;
        // next node is the new head in the reverse function
        // at this stage head is still at one, and we link with the return value (the last visited vale in k gruop)
        // 1 2 3
        // head = 1;
        // reverse(4) --> 6,5,4  (head = 4)
        // reverse (7) -> return 7
        // head = 4; 4 -> next = 7;          so we have 4 -> 7
        //  return 6 .head is 1..so we have 1->6
        head->next = reverse(next, k);
        if (head && head->next )
            cout << "head next is " << head->next->data;
    }

    /* prev is new head of the input list */
    std::cout << " pre is " << prev->data << std::endl;
    return prev;
}

/* UTILITY FUNCTIONS */
/* Function to push a node */
void push(struct node** head_ref, int new_data)
{
    /* allocate node */
    struct node* new_node =
        (struct node*) malloc(sizeof(struct node));

    /* put in the data  */
    new_node->data = new_data;

    /* link the old list off the new node */
    new_node->next = (*head_ref);

    /* move the head to point to the new node */
    (*head_ref) = new_node;
}

/* Function to print linked list */
void printList(struct node *node)
{
    while (node != NULL)
    {
        printf("%d  ", node->data);
        node = node->next;
    }
}

/* Drier program to test above function*/
int main(void)
{
    /* Start with the empty list */
    struct node* head = NULL;

    /* Created Linked list is 1->2->3->4->5->6->7->8->9 */
  //  push(&head, 9);
  //  push(&head, 8);
    push(&head, 7);
    push(&head, 6);
    push(&head, 5);
    push(&head, 4);
    push(&head, 3);
    push(&head, 2);
    push(&head, 1);

    printf("\nGiven linked list \n");
    printList(head);
    // new head is the kth node
    head = reverse(head, 3);

    printf("\nReversed Linked list \n");
    printList(head);

    return(0);
}

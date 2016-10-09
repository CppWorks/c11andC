/*
Method 1 (Check one by one)
We know that Floyd’s Cycle detection algorithm terminates when fast and slow pointers
meet at a common point. We also know that this common point is one of the loop nodes 
(2 or 3 or 4 or 5 in the above diagram). We store the address of this in a pointer 
variable say ptr2. Then we start from the head of the Linked List and check for nodes
one by one if they are reachable from ptr2. When we find a node that is reachable, we know that 
this node is the starting node of the loop in Linked 
List and we can get pointer to the previous of this node.*/

//Function to remove loop.Used by detectAndRemoveLoop() * /
void removeLoop(struct node *, struct node *);

/* This function detects and removes loop in the list
If loop was there in the list then it returns 1,
otherwise returns 0 */
int detectAndRemoveLoop(struct node *list)
{
    struct node  *slow_p = list, *fast_p = list;

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
head -->  Pointer to the start node of the linked list */
void removeLoop(struct node *loop_node, struct node *head)
{
    struct node *ptr1;
    struct node *ptr2;

    /* Set a pointer to the beging of the Linked List and
    move it one by one to find the first node which is
    part of the Linked List */
    ptr1 = head;
    while (1)
    {
        /* Now start a pointer from loop_node and check if it ever
        reaches ptr2 */
        ptr2 = loop_node;
        while (ptr2->next != loop_node && ptr2->next != ptr1)
            ptr2 = ptr2->next;

        /* If ptr2 reahced ptr1 then there is a loop. So break the
        loop */
        if (ptr2->next == ptr1)
            break;

        /* If ptr2 did't reach ptr1 then try the next node after ptr1 */
        ptr1 = ptr1->next;
    }

    /* After the end of loop ptr2 is the last node of the loop. So
    make next of ptr2 as NULL */
    ptr2->next = NULL;
}
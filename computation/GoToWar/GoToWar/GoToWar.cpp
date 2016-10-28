// GoToWar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define QUEUESIZE 1000
typedef struct {
    int q[QUEUESIZE + 1]; /* body of queue */
    int first; /* position of first element */
    int last; /* position of last element */
    int count; /* number of queue elements */
} queue;
void init_queue(queue *q)
{
    q->first = 0;
    q->last = QUEUESIZE - 1;
    q->count = 0;
}
void enqueue(queue *q, int x)
{
    if (q->count >= QUEUESIZE)
        printf("Warning: queue overflow enqueue x=%d\n", x);
    else {
        q->last = (q->last + 1) % QUEUESIZE;
        q->q[q->last] = x;
        q->count = q->count + 1;
    }
}
int dequeue(queue *q)
{
    int x;
    if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
    else {
        x = q->q[q->first];
        q->first = (q->first + 1) % QUEUESIZE;
        q->count = q->count - 1;
    }
    return(x);
}
int empty(queue *q)
{
    if (q->count <= 0) return (1);
    else return (0);
}

#define NCARDS 52 /* number of cards */
#define NSUITS 4 /* number of suits */
char values[] = "23456789TJQKA";
char suits[] = "cdhs";
#define  MAXSTEPS 1000


int rank_card(char value, char suit)
{
    int i, j; /* counters */
    // i is card number, j is suit
    // if i is 2, j is d, so j is 1..thren return 2 * 4 + d
    for (i = 0; i < (NCARDS / NSUITS); i++)
        if (values[i] == value)
            for (j = 0; j < NSUITS; j++)
                if (suits[j] == suit)
                    return(i*NSUITS + j);
    printf("Warning: bad input value=%d, suit=%d\n", value, suit);
}

char suit(int card)
{
    return(suits[card % NSUITS]);

}

char value(int card)
{
    return(values[card / NSUITS]);
}


void clear_queue(queue *a, queue *b)
{
    while (!empty(a)) enqueue(b, dequeue(a));
}

void war(queue *a, queue *b)
{
    int steps = 0; /* step counter */
    int x, y; /* top cards */
    queue c; /* cards involved in the war */
    bool inwar; /* are we involved in a war? */
    inwar = 0;
    init_queue(&c);
    while ((!empty(a)) && (!empty(b) && (steps < MAXSTEPS))) {
        steps = steps + 1;
        x = dequeue(a);
        y = dequeue(b);
        enqueue(&c, x);
        enqueue(&c, y);
        if (inwar) {
            inwar = 0;
        }
        else {
            if (value(x) > value(y))
                clear_queue(&c, a);
            else if (value(x) < value(y))
                clear_queue(&c, b);
            else if (value(y) == value(x))
                inwar = 1;
        }
    }

    if (!empty(a) && empty(b))
        printf("a wins in %d steps \n", steps);
    else if (empty(a) && !empty(b))
        printf("b wins in %d steps \n", steps);
    else if (!empty(a) && !empty(b))
        printf("game tied after %d steps, |a|=%d |b|=%d \n",
        steps, a->count, b->count);
    else
        printf("a and b tie in %d steps \n", steps);
}




int _tmain(int argc, _TCHAR* argv[])
{
    queue decks[2]; /* player’s decks */
    char value, suit, c; /* input characters */
    int i; /* deck counter */
    while (1) {
        for (i = 0; i <= 1; i++) {
            init_queue(&decks[i]);
            while ((c = getchar()) != '\n') {
                if (c == EOF) return 0;
                if (c != ' ') {
                    value = c;
                    suit = getchar();
                    enqueue(&decks[i], rank_card(value, suit));
                }
            }
        }
        war(&decks[0], &decks[1]);
    }

    return 0;
}


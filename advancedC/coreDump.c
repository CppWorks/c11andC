#include <stdio.h>
/* bad.c */

int bad(int *pt)
{
    int x = *pt;
    return x;
}

int main()
{
    int *ptr = 0;   /* null pointer */
    return bad(ptr);
}
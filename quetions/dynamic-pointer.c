#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>

#define DEBUG_VALUE(v) (printf(#v " is equal to %d\n", v))
#define FOO(x) (foo ## x)

void foo3(int a)
{
    printf("foooo\n");
}
// 14 Can the size of an array be declared at runtime?

// malloc : Allocate memory block
// Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
// char argv[], char* argv[].   They are different! a char is actually an array which can be called by its addess at argv[0]
//  when we want to define a string, we can do static char name[10] = "abcde"; or dynamically by char* pNmae = (char&)malloc(3); then strspy.
// if the length of static is shorter than abcde, compiler will complain.
/// iteresting....when we free new_argv[i] , argv[i] is also cleared..coz they are freeing pointers...so after deltoin, it points to null.
int main (int argc, char *argv[])
{
    char** new_argv;
    int i;
    /* Since argv[0] through argv[argc] are all valid, the
    program needs to allocate room for argc+1 pointers. */
    // ahh haaa...calloc give me an array so I can do new_argv[1] .....
    // if I use malloc....it wont work..
    new_argv = (char**) calloc(argc+1, sizeof (char*));
    /* or malloc((argc+1) * sizeof (char*)) */
    printf("allocated room for %d pointers starting at %p\n", argc+1, new_argv);
    /* now copy all the strings themselves
    (argv[0] through argv[argc-1]) */
    for (i = 0; i < argc; ++i) {
        /* make room for '\0' at end, too */
    new_argv[i] = (char*) malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i], argv[i]);
        printf("allocated %u bytes for new_argv[%d] at %p, ""copied \"%s\"\n",
                (unsigned int)strlen(argv[i]) + 1, i, new_argv[i], new_argv[i]);
    }
    new_argv[argc] = NULL;
    char** ite;
    printf("address at newarg[argc + 1] is %p\n", &new_argv[argc + 1]);
    for (ite = &new_argv[0]; ite < &new_argv[argc+1]; ite++)
    {
        printf("new_argv[%s] at %p\n", *ite, ite);
    }
    /* To deallocate everything, get rid of the strings (in any
    order), then the array of pointers. If you free the array
    of pointers first, you lose all reference to the copied
    strings. */
    for (i = 0; i < argc; ++i) 
    {
        free(new_argv[i]);
        printf("freed new_argv[%d] at %p\n", i, new_argv[i]);
        argv[i] = NULL;
    }
    free(new_argv);

    // so the pointer diff is again the index dif in the array....
    char** p0 = &argv[0];
    char** p2 = &argv[2];
    ptrdiff_t       diff = p2 - p0;
    //  printf("%s", p);
    printf("The difference of pointers is %ld\n",
        (long) diff);
    printf("p0 = %p\n",  (p0));
    printf("p2= %p\n",  (p2));
    printf("p2 - 2 = %p\n", (void*) (p2 - 2));
    printf("freed new_argv itself at %p\n", new_argv);
        printf("pf2 s= %s\n",  argv[2]);
    // casting to char* is magic...
 //  ptrdiff_t       addr_diff = (char*)p2 - (char*)p0;

  //  printf("The difference of addresses is %ld\n",
    //   (long) addr_diff);
    char * pInt = (char*) malloc( 5);
    strcpy(pInt, "ace");

    printf("char is %s\n", pInt);
    free(pInt);
    pInt = NULL;
    printf("int p is %p\n", pInt);
    printf("int is %s\n", pInt);


    int mm  = 3;
    DEBUG_VALUE(mm);
    FOO(3)(3);
    return 0; 
}

// 60 96
// 80 128
// a0
// gcc -Wno-implicit alarm.c -o hello -lpthread
// 1 kb = 1000 bytes
// 64 kb = 64 * 1000 bytes
// you can get memory from stack....or help...memerofy from stack can be deallocated automatically.. like int f = 3;//we have a memeroy for this variableon the stack..
// And it make sense the memory are deallocated in the fist in last out order.

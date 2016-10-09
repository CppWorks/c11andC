http://c.learncodethehardway.org/book/ex18.html
http://www.indiabix.com/technical/c/pointers/7

Exercise 18: Pointers To Functions
Functions in C are actually just pointers to a spot in the program where some code exists. Just like you've been creating pointers to structs, strings, and arrays, you can point a pointer at a function too. The main use for this is to pass "callbacks" to other functions, or to simulate classes and objects. In this exercise we'll do some callbacks, and in the next one we'll make a simple object system.

The format of a function pointer goes like this:

int (*POINTER_NAME)(int a, int b)

A way to remember how to write one is to do this:

Write a normal function declaration: int callme(int a, int b)
Wrap function name with pointer syntax: int (*callme)(int a, int b)
Change the name to the pointer name: int (*compare_cb)(int a, int b)
The key thing to remember is, when you're done with this, the variable name for the pointer is called compare_cb and then you use it just like it's a function. This is similar to how pointers to arrays can be used just like the arrays they point to. Pointers to functions can be used like the functions they point to but with a different name.


The next problem to solve with using function pointers is that it's hard to give them as parameters to a function, like when you want to pass the function callback to another function. The solution to this is to use typedef which is a C keyword for making new names for other more complex types. The only thing you need to do is put typedef before the same function pointer syntax, and then after that you can use the name like it's a type. I demonstrate this in the following exercise code:


call back

use typedef

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);

int sorted_order(int a, int b)
{
    return a - b;
}


int bubble_sort(int count, compare_cb cmp)

{
    int f = cmp(5,9);
    printf("oh value is %d\n", f);
    return f;
}

int main()
{
      bubble_sort(10,sorted_order);
      return 0;  
}

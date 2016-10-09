
http://thbecker.net/articles/rvalue_references/section_05.html

Move Semantics  - it means swap and destroy the resoueces liked to A by temp B destroy


 Suppose X is a class that holds a pointer or handle to some resource, say, m_pResource. 
 By a resource, I mean anything that takes considerable effort to construct, clone, or destruct. 
 A good example is std::vector, which holds a collection of objects that live in an array of allocated memory. 
 Then, logically, the copy assignment operator for X looks like this: // so does std::vector suppourt move? yes...try it...

X& X::operator=(X const & rhs)
{
    // [...]
    // Make a clone of what rhs.m_pResource refers to.
    // Destruct the resource that m_pResource refers to. 
    // Attach the clone to m_pResource.
    // [...]
}

// fora vector, we need to destory the memory allocated for the old array and make a clone of the new array..then redirect the pointer....

//Similar reasoning applies to the copy constructor. Now suppose X is used as follows:
X foo();
X x;
// perhaps use x in various ways
x = foo();

The last line above
    clones the resource from the temporary returned by foo,
    destructs the resource held by x and replaces it with the clone,
    destructs the temporary and thereby releases its resource. // note that temp resource is released...

Rather obviously, it would be ok, and much more efficient, 
to swap resource pointers (handles) between x and the temporary, 
and then let the temporary's destructor destruct x's original resource.  // so the temp resources dot need to be cloned and deleted....
In other words, in the special case where the right hand side of the assignment is an rvalue, we want the copy assignment operator to act like this:
// [...]
// swap m_pResource and rhs.m_pResource
// [...]  
This is called move semantics. With C++11, this conditional behavior can be achieved via an overload:
X& X::operator=(<mystery type> rhs)
{
    // [...]
    // swap this->m_pResource and rhs.m_pResource
    // [...]  
}

Rvalue References
If X is any type, then X&& is called an rvalue reference to X. For better distinction, the ordinary reference X& is now also called an lvalue reference.

An rvalue reference is a type that behaves much like the ordinary reference X&, with several exceptions. The most important one is that when it comes to function overload resolution, lvalues prefer old-style lvalue references, whereas rvalues prefer the new rvalue references:

void foo(X& x); // lvalue reference overload
void foo(X&& x); // rvalue reference overload
X x;
X foobar();

foo(x); // argument is lvalue: calls foo(X&)
foo(foobar()); // argument is rvalue: calls foo(X&&)
So the gist of it is:
Rvalue references allow a function to branch at compile time (via overload resolution) on the condition "Am I being called on an lvalue or an rvalue?"

    It is true that you can overload any function in this manner, as shown above. But in the overwhelming majority of cases,
    this kind of overload should occur only for copy constructors and assignment operators, for the purpose of achieving move semantics:
so it is mainly used by copy and assighment constructor overload for move Semantics...

 X& X::operator=(X const & rhs); // classical implementation
X& X::operator=(X&& rhs)
{
    // Move semantics: exchange content between this and rhs
    return *this;
}

Caveat: As it happens so often in C++, what looks just right at first glance is still a little shy of perfect. 
            It turns out that in some cases, the simple exchange of content between this and rhs in the implementation 
            of the copy assignment operator above is not quite good enough. We'll come back to this in Section 4, "Forcing Move Semantics" below.


       void foo(X const &);
        but not
            void foo(X&&);
        then again, the behavior is unchanged: foo can be called on l-values and r-values,
// This is a solution to take both lvalue and rvalue...why........

Forcing Move Semantics

this is about std::move

think about each object has an array by ponter...when we swap..we really just want to swap pointer addresed..not clone and redirect...

template<class T>
        void swap(T& a, T& b) 
        { 
            T tmp(a);
            a = b; 
            b = tmp; 
        } 

        X a, b;
        swap(a, b);
        There are no rvalues here. 
            Hence, all three lines in swap use non-move semantics. 
            But we know that move semantics would be fine: wherever a variable occurs as the source of a 
            copy construction or assignment, that variable is either not used again at all, or else it is used only as the target of an assignment.
C++11, there is an std library function called std::move that
comes to our rescue. It is a function that turns its argument into an 
rvalue without doing anything else. Therefore, in C++11, the std library function swap looks like this:

        template<class T> 
        void swap(T& a, T& b) 
        { 
            T tmp(std::move(a));
            a = std::move(b); 
            b = std::move(tmp);
        } 

        X a, b;
        swap(a, b);
        now T will be deleted with the old resources...nice...

Now all three lines in swap use move semantics. Note that for those types that do not implement move semantics (that is, do not overload their copy constructor 
and assignment operator with an rvalue reference version), the new swap behaves just like the old one.


Using std::move wherever we can, as shown in the swap function above, gives us the following important benefits:

        For those types that implement move semantics, many standard algorithms and operations will use move semantics and thus experience 
            a potentially significant performance gain. An important example is inplace sorting: inplace sorting algorithms do hardly 
            anything else but swap elements, and this swapping will now take advantage of move semantics for all types that provide it.

            The STL often requires copyability of certain types, e.g., types that can be used as container elements. Upon close inspection,
            it turns out that in many cases, moveability is enough. Therefore, we can now use types that are moveable but not copyable 
            (unique_pointer comes to mind) in many places where previously, they were not allowed. For example, these types can now be used as STL container elements.
            // unique_pionter what is that? now STL only need moveable?
            Now that we know about std::move, we are in a position to see why the implementation of the rvalue reference overload of 
            the copy assignment operator that I showed earlier is still a bit problematic. Consider a simple assignment between variables, like this:
        a = b; 
        What do you expect to happen here? You expect the object held by a to be replaced by a copy of b, and in the course of this replacement,
            you expect the object formerly held by a to be destructed. Now consider the line
            a = std::move(b); 
        If move semantics are implemented as a simple swap, then the effect of this 
            is that the objects held by a and b are being exchanged between a and b.
            Nothing is being destructed yet. The object formerly held by a will of
            course be destructed eventually, namely, when b goes out of scope. Unless, 
            of course, b becomes the target of a move, in which case the object formerly held
            by a gets passed on again. Therefore, as far as the implementer of the copy assignment
            operator is concerned, it is not known when the object formerly held by a will be destructed.


            So in a sense, we have drifted into the netherworld of non-deterministic destruction here: a variable has been assigned to, but the object formerly held by that variable is still out there somewhere. That's fine as long as the destruction of that object does not have any side effects that are visible to the outside world. But sometimes destructors do have such side effects. An example would be the release of a lock inside a destructor. Therefore, any part of an object's destruction that has side effects should be performed explicitly in the rvalue reference overload of the copy assignment operator:

        X& X::operator=(X&& rhs)
        {

            // Perform a cleanup that takes care of at least those parts of the
            // destructor that have side effects. Be sure to leave the object
            // in a destructible and assignable state.

            // Move semantics: exchange content between this and rhs

            return *this;
        }
        // what does this mean? not sure yet

        Is an Rvalue Reference an Rvalue?

            As before, let X be a class for which we have overloaded the copy constructor and copy assignment operator to implement move semantics. Now consider:
        void foo(X&& x)
        {
            X anotherX = x;
            // ...
        }

        now x becomes lvalue cos it has an address...
            Things that are declared as rvalue reference can be lvalues or rvalues. The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue.
so X anotherX = x; // calls X(X const & rhs)


        Here is an example of something that is declared as an rvalue reference and does not have a name, and is therefore an rvalue:
        X&& goo();
        X x = goo(); // calls X(X&& rhs) because the thing on
        // the right hand side has no name

        // interesting

        // the reason is that if we pass in a rvalue...and it is used by other copy constructory..if we treat it as rvalue..we will use the move..then x is still in scope.
        // so it is still used...but it is wrong now!

        would be dangerously confusing and error-prone because the thing from which we just moved, that is, the thing
            that we just pilfered, is still accessible on subsequent lines of code. But the whole point of move semantics was to apply it only
        where it "doesn't matter," in the sense that the thing from which we move dies and goes away right after the moving. Hence the rule, "If it has a name, then it's an lvalue."

        // but sometimes we want to force move semantics on lvalues at our discretion.


Pointers:
        Can the size of an array be declared at runtime?
        No. we cant do int char[i] and i is defined at run time (this means stack?). However we can declarer such an array with 
        a pointer by calling malloc or calloc to allocate the array from the heap.

        Note that int main (int argc, char *argv[]) argv[] can be replace by char** argv..  a pointer to a char* pointer

        const pointer means you cannot modify the reffed value

A "const pointer," or more correctly, a "pointer to const," is 
a pointer which points to data that is const (constant, or unchanging).
A pointer to const is declared by putting the word const at the beginning of the pointer declaration. 

Function
Why should I prototype a function?
A function prototype tells the compiler what kind of arguments a function is looking to receive and what kind of return value a function is going to give back. 
This approach helps the compiler ensure that calls to a function are made correctly and that no erroneous type conversions are taking place. For instance, consider the following prototype.

Using prototypes, the compiler can also ensure that the function definition, or body, is correct and correlates with the prototype.

what is prototype, it is declearion  without argument name.

How many parameters should a function have?
This is where tConfig comes to play. Use a struct to warp a lot of values, this could speed up
The only possible disadvantage to this approach is that by removing the parameters from the function definition, you are bypassing 
the compiler's capability to type-check each of the parameters for validity during the compile stage.

Generally, you should keep your functions small and focused, with as few parameters as possible to help with execution speed. 
If you find yourself writing lengthy functions with many parameters, maybe you should rethink your function design or consider using the structure-passing technique presented here.
Additionally, keeping your functions small and focused will help when you are trying to isolate and fix bugs in your programs.

Is using exit() the same as using return?
No. The exit() function is used to exit your program and return control to the operating system. 
The return statement is used to return from a function and return control to the calling function. 
If you issue a return from the main() function, you are essentially returning control to the calling function, 
which is the operating system. In this case, the return statement and exit() 
function are similar. Here is an example of a program that uses the exit() function and return statement:

            for ( p = a; p < & a[ MAX ]; ++p )
            {
                x = *p;
                /* do something with x */
            }

            THIS IS a tpical c way to look through array
What's different between these two versions? The initialization and 
increment in the loop are the same. The comparison is about the same; more on that in
a moment. The difference is between x=a[i] and x=*p. The first has to find the address of 
a[i]; to do that, it needs to multiply i by the size of an X and add it to the address of 
the first element of a. The second just has to go indirect on the p pointer. Indirection is fast; multiplication is relatively slow.

/* how the compiler implements version (b) */
X       *temp = & a[ MAX ];     /* optimization */
            for ( p = a; p < temp; ++p )
            {
                x = *p;
                /* do something with x */
            }

The takeaway is use pointer is faster then indexing if possible. But we call it micro optimising

    Can you assign a different address to an array tag?
 An array tag is not something you can put on the left side of an assignment operator. (It's not an "lvalue," let alone a "modifiable lvalue.") 

Prepocessors:


A macro is a preprocessor directive that provides a mechanism for token replacement in your source code. Macros are created by using the #define statement. Here is an example of a macro:

I just covered The C Language Basis, Variables and Data Storage, Functions, Preprocessors, Arrays, Pointers.

Files to go: bits and bytes, data files, standard library functions, strings.

What is cancelliation point?

Deferred cancellation can mean waiting on a condition variable, reading or writing FILE and other functions where the tread may be blocked for a sbstaintial period of time.

If we want to know when the tread has actually terminated, we must join with it by calling pthread_join.....

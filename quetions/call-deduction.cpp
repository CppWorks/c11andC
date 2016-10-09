#include <iostream>
#include <vector>
#include <initializer_list>

/*
Deduction from a function call
Template argument deduction attempts to determine template arguments (types for type template parameters Ti, templates for template template parameters TTi, and values for non-type template parameters Ii), which can be substituted into each parameter P to produce the type deduced A, which is the same as the type of the argument A, after adjustments listed below.
If there are multiple parameters, each P/A pair is deduced separately and the deduced template arguments are then combined. If deduction fails or is ambiguous for any P/A pair or if different pairs yield different deduced template arguments, or if any template argument remains neither deduced nor explicitly specified, compilation fails.
If removing references and cv-qualifiers from P gives std::initializer_list<P'> and A is a braced-init-list, then deduction is performed for every element of the initializer list, taking P' as the parameter and the list element A' as the argument:
template<class T> void f(std::initializer_list<T>);
f({1, 2, 3});  // P = std::initializer_list<T>, A = {1, 2, 3}
// P'1 = T, A'1 = 1: deduced T = int
// P'2 = T, A'2 = 2: deduced T = int
// P'3 = T, A'3 = 3: deduced T = int
// OK: deduced T = int
f({1, "abc"}); // P = std::initializer_list<T>, A = {1, "abc"}
// P'1 = T, A'1 = 1: deduced T = int
// P'2 = T, A'2 = "abc": deduced T = const char*
// error: deduction fails, T is ambiguous*/
using namespace std;

template<class T> void f(std::initializer_list<T>)
{
    cout<<"hey"<<endl;
};

void foo(int&&)
{

};

int main()
{
    f({1, 2, 3});
    int a = 3;
    foo(3);

    int b = 4;
    int tmp(std::move(a));
    a = std::move(b); // this givea a rvalue reference so rvalue assginment copy constructor can be used
    b = std::move(tmp);
    cout<<a<<"\t"<<b<<endl;
    return 0;
}
	
/*
template<class T1, class T2, int I>
class A {};            // primary template

template<class T, int I>
class A<T, T*, I> {};  // #1: partial specialization where T2 is a pointer to T1

template<class T, class T2, int I>
class A<T*, T2, I> {}; // #2: partial specialization where T1 is a pointer

template<class T>
class A<int, T*, 5> {}; // #3: partial specialization where T1 is int, I is 5,
//     and T2 is a pointer

template<class X, class T, int I>
class A<X, T*, I> {};   // #4: partial specialization where T2 is a pointer

// given the template A as defined above
A<int, int, 1> a1;   // no specializations match, uses primary template
A<int, int*, 1> a2;  // uses partial specialization #1 (T=int, I=1)
A<int, char*, 5> a3; // uses partial specialization #3, (T=char)
A<int, char*, 1> a4; // uses partial specialization #4, (X=int, T=char, I=1)
A<int*, int*, 2> a5; // error: matches #2 (T=int, T2=int*, I=2)
//        matches #4 (X=int*, T=int, I=2)
// neither one is more specialized than the other

*/
	
#include <iostream>
// This is about partial template specialization
// have a base template..add template we need for partial specialisation...they can be in any order.
// then they are specialied after the class name....for example is_same<T,T>
using namespace std;
class A {

};

class B {};

template<typename T, typename U>
struct is_same
{
    static const bool value = false;
};

template<typename T>
struct is_same <T,T>
{
    static const bool value = true;
};

template <class A, class B>
bool isSameClass()
{
    return is_same<A,B>::value;
}

template<int I, int J, int K> 
class C{
public:
    C() {
        i = I;
        j = J;
    }
int i;
int j;

};

template<int I> 
class C <I, I,2> {
public:
    C() {
        i = I;
        j = 2;
    }
    int i;
    int j;
};

template<int I> 
class C <I, 20,10> {
public:
    C() {
        i = I;
        j = 10;
    }
    int i;
    int j;
};

using namespace std;
int main()
{
    cout<<isSameClass<A,A>() << "\n";
    C<10,20,10> b1;
    cout<<"I is "<<b1.i<<"  J is "<< b1.j<<"\n";
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
	
#include <iostream>
#include <type_traits>
// This is about partial template specialization
// have a base template..add template we need for partial specialisation...they can be in any order.
// then they are specialied after the class name....for example is_same<T,T>
using namespace std;

//template<typename T> struct is_pointer { static const bool value = false; }; 
//
//template<typename T> struct is_pointer<T*> { static const bool value = true; };
//template<typename T> void func(const std::vector<T>& v) { std::cout << "is it a pointer? " << std::is_pointer<T>::value << std::endl; }


template<class T1, class T2, int I>
class A {
public:
    A() {
        
        cout << "primary \n";};
    bool isT2Pointer()
    {
        std::is_pointer<T2>::value;
    }
    T2 m_2;
};            // primary template

template<class T, int I>
class A<T, T*, I> {
public:
    A() {
        m_2 = new T;
        cout << "#1 \n";};
    bool isT2Pointer()
        {
           cout<<"is pointer"<< std::is_pointer<T*>::value << endl;
        }
public:
    T* m_2;
};  // #1: partial specialization where T2 is a pointer to T1

template<class T, class T2, int I>
class A<T*, T2, I> {
public:
    A() {cout << "#2 \n";};
}; // #2: partial specialization where T1 is a pointer

template<class T>
class A<double, T*, 5> {
public:
    A() {cout << "# 3 \n";};
}; // #3: partial specialization where T1 is int, I is 5,
//     and T2 is a pointer

template<class X, class T, int I>
class A<X, T*, I> {
public:
    A() {cout << "#4 \n";};
};   // #4: partial specialization where T2 is a pointer

//template<class T, int I>
//class A<int, T*, I> {
//public:
//    A() {cout << "#5 \n";};
//};   // #4: partial specialization where T2 is a pointer


int main()
{
    A<int, int*, 5> a5; 
    a5.isT2Pointer() ;
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
	
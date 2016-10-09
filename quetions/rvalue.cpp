#include <iostream>


using namespace std;

void foo(int&)
{
    cout<<"im in"<<endl;
}

// remember this is the way to pass in rvalue
void foo(int const & x)
{
    cout<<"const &"<<x<<endl;
}

void foo(int&&)
{
    cout<<"int &&"<<endl;
}
int foo2()
{
    return 2;
}
int main()
{
    foo(5); // this will fail coz we are passing rvalue to lvalue ref
    foo(foo2()); // this also fails as expected..

    // 1) Swaps the values a and b. This overload does not participate in overload resolution unless std::is_move_constructible_v<T> && std::is_move_assignable_v<T> is true. (since C++17)
    int a = 1;
    int b = 3;
    std::swap(a, b);
    cout<<"a is " << a <<"\t"<< " b is "<<b<<endl;

    // std::swap will do smart things if we have move implemented..
    // Type requirements
  //   -T must meet the requirements of MoveAssignable and MoveConstructible.
    //    -T2 must meet the requirements of Swappable.
    int& c = a;
    int& d = b;
    std::swap(c, d);
    cout<<"c is " << c <<"\t"<< " d is "<<d<<endl;

    int f = (c++, d++);
    cout<<"f is"<<f<<endl;

    int p[] = {1,2,3,4};
    int* p1 = &p[0];
    int* p2 = &p[2];
    cout<< sizeof(int)<<endl;
    return 0;
}
	
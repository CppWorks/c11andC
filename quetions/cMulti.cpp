#include <iostream>
#include <thread>

void foo() 
{
    std::cout << "foo()\n";
}

void bar()
{
    std::cout<<"bar()\n"<<std::endl;
}

int main()
{
    std::thread t([]{bar(); foo();});
    t.join();
    return 0;
}
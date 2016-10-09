#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>

using namespace std;

void doTask(unsigned id) 
{
    cout << "doTask()\n";
}
struct X
{
    void fa()
    {
        cout<<"fafafaf\n";
    }
};

void f()
{
    vector<thread> threads;

    for(unsigned i = 0; i < 10; ++i)
    {
        // spawn threads
        threads.push_back(thread(doTask, i));
    }

    // call join() on each thread in turn
    for_each(threads.begin(), threads.end(),
        std::mem_fn(&thread::join));

    vector<X> hi;
    hi.push_back(X());
    hi.push_back(X());
    for_each(hi.begin(), hi.end(),
        std::mem_fn(&X::fa));

}

int main()
{
    f();
    return 0;
}
#include <thread>
#include <iostream>
using namespace std;

void foo(int i) 
{
    cout << "foo(" << i << ")\n";
}

struct task_struct
{
    int& i;
    task_struct(int& ii):i(ii)
    {
        cout << "constructor for task_struct\n";
    }

    void operator()()
    {
        cout << "task_struct::operator()\n";
        for(unsigned j = 0; j < 10; ++j)
        {
            foo(i);
        }
    }
};


class MyThread
{
    thread t;
public:
    explicit MyThread(thread& th):t(move(th))
    {
        // Checking the thread is still joinalbe in the constructor 
        // and throw an exception if it's not.
        if(!t.joinable()) 
            throw std::logic_error( "Not joinable" );
        cout << "MyThread constructor\n";
    }

    ~MyThread()
    {
        // joins with the thread supplied to the constructor t.join();
        // do not need to joinable check. 
        // if(t.joinable()) t.join();
        t.join();
        cout << "MyThread destructor\n";
    }

    // copy constructor
  //  MyThread(MyThread const&) ;

    // copy-assignment operator
  //  MyThread& operator=(MyThread const&);
};

void current_thread_task()
{
    cout << "do something in current_thread_task()\n";
};

void f()
{	
    int state = 99; // local state variable

    task_struct task(state);
    thread t(task);

    MyThread mt(t);

    current_thread_task();
    // When the initial thread reaches this point, 
    // the object of MyThread is destroyed in ~MyThread()
}

int main()
{
    f();

    return 0;
}
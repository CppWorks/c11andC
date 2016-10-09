#include <iostream>
#include <vector>

using namespace std;


// 
struct D
{
	D(int x, int y)
	{
		a = x;
		b = y;
	
	}
	int a;
	int b;
};

int main()
{
	D my{1,2};
	cout<<my.a<<" and "<<my.b<<endl;
	return 0;
	
	
}

//oddo@oddo-VirtualBox:~/Desktop/c11$ g++ -std=c++11 range.cpp -o range

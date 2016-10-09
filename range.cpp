#include <iostream>
#include <vector>

using namespace std;

void f(vector<double>& v)
{
//	for (auto x : v) cout << x << "\n";
	for (auto& x : v) 
	{
		x = x+5;
		cout<<x<<"\n";
		
	}
};


template<class T>
class MyArrayWrapper {
public:
	T* data;
	int size;
public:
	int* begin() { 
		cout<<"begin called"<<endl;
		return size > 0 ? &data[0] : nullptr;}
	int* end() { 
		cout<<"end called"<<endl;
		return size > 0 ? &data[size-1] : nullptr;}
};



int main()
{
	vector<double> v;
	v.push_back(4.3);
	v.push_back(5.4);
	//f(v);
	
	MyArrayWrapper<int> arr;
	int array[] = {9,1,2};
	arr.data = &array[0];
	arr.size = 3;
	
	for (auto v : arr)
	{
		cout<<v<<endl;
	}
	
	
	}

//oddo@oddo-VirtualBox:~/Desktop/c11$ g++ -std=c++11 range.cpp -o range

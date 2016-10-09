#include <iostream>


using namespace std;

int binary()
{
    int m[] = {1,2,3,4,5,10};
    int* p = &m[0];
    int* q = &m[5];

    int f = 5;
    if (*p == f || *q == f)
    {
        cout<<"we found it at ends"<<endl;
        return 0;
    }
    // q-p = 5; /2 = 2; temp = p + 2; p = 3
    // q = 5; q-p = 2; 2/2  = 1; temp = 1
    int* temp;
    bool found = false;
    while (!found)
    {
        if (*p < f && *q > f)
            temp = p + (q-p)/2;
        cout<<"middle is "<<temp - p<< " with value "<<*temp<<endl;;
        if (*temp < f)
            p = temp;
        if (*temp > f)
            q = temp;
        cout<<"p is "<<*p <<"   q is "<<*q << endl;;
        if (*temp == f)
            found = true;

        if (q-p <= 1)
            break;

    }
    cout<<"do we find it " << (found ? "True" : "False") <<endl;
}

int main()
{
    return binary();


    return 0;
}
	
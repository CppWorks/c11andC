// computation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Cyclic.h"


int _tmain(int argc, _TCHAR* argv[])
{
    int n = 4;
    // 0-3 - 2 -1
    // [0,1,2,3]
    // [3,1,2,0]
    // [3,1,0,2]
    // [3,0,1,2]
    int f[4] = { 3, 0, 1, 2 };
    int tb[4] = { 0, 0, 0, 0 };
    //print_cycles(f, n, tb);
    make_inverse(f, n, tb);
    clear(tb, n);
    print_cycles(f, n, tb);
    return 0;
}


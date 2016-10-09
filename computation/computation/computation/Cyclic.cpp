#include "Cyclic.h"
#include "stdafx.h"
#include <iostream>
using namespace std;

// The key point is the leader needs to be met again....
void print_cycles(int *f, int n, int *tb /*=0*/) {
  for (int k = 0; k < n; ++k) {
    if (tb[k] == 1)
      continue; // already processed

    cout << "(";
    int i = k; // next in cycle
    const char *cm = "";
    do {
      cout << cm << i;
      cm = ", ";
      tb[i] = 1;
    } while ((i = f[i]) != k); // until we meet cycle leader again
    cout << ") ";
  }
}

bool is_cyclic(const int *f, int n)
// Return whether permutation is exactly one cycle.
{
  if (n <= 1)
    return true;
  int k = 0, e = 0;
  // start from 0..finish when we come back to 0..check the length
  do {
    e = f[e];
    ++k;
  } while (e != 0);
  return (k == n);
}

bool is_inverse(const int *f, const int *g, int n)
// Return whether f[] is the inverse of g[]
// f.g means f[g[k]]
// example: f = [0,1,2], g=[0,1,2]
// k = 0; f[g[0]] = f[0] = 0;
// k = 1; f[g[1]] = f[1] = 1;
// k = 2; f[g[2]] = f[2] = 2; // so its inverse...
{
  for (int k = 0; k < n; ++k)
    if (f[g[k]] != k)
      return false;
  return true;
}

// cycle is 0->3->2->1
// f[4] = { 3, 0, 1, 2 };
void make_inverse(int *f, int n, int *bp /*=0*/)
// Set (as permutation) f to its own inverse.
{
  int *tp = bp;
  // if (0 == bp) tp = new bitarray(n); // tags
  // tp->clear_all();

  for (int k = 0; k < n; ++k) {
    if (tp[k] == 1)
      continue; // already processed
    tp[k] = 1;

    // invert a cycle:
    int i = k;
    int g = f[i]; // next index
    while (0 == (tp[g] == 1)) {
      int t = f[g];
      f[g] = i;
      i = g;
      g = t;
    }
    f[g] = i;
  }

  if (0 == bp)
    delete tp;
}

void clear(int* bp, int n)
{
    for (int i = 0; i < n; i++)
    {
        bp[i] = 0;
    }
}

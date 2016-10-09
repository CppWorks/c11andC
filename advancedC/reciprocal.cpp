#include <cassert>
#include "reciprocal.hpp"
double reciprocal (int i) {
    // I should be non-zero.
    assert (i != 0);
    return 1.0/i;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread


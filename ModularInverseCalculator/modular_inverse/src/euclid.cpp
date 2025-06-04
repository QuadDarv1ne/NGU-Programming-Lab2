#include "euclid.h"

std::tuple<int, int, int> extendEuclid(int c, int m) {
    if (m == 0) {
        return std::make_tuple(c, 1, 0);
    }
    
    auto [gcd, u1, v1] = extendEuclid(m, c % m);
    int u = v1;
    int v = u1 - (c / m) * v1;
    
    return std::make_tuple(gcd, u, v);
}
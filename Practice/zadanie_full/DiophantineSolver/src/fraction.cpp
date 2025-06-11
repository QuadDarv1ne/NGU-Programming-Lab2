#include "fraction.h"
#include <vector>
#include <utility>

std::vector<int> continued_fraction(int a, int b) {
    std::vector<int> cf;
    while (b != 0) {
        cf.push_back(a / b);
        int temp = a % b;
        a = b;
        b = temp;
    }
    return cf;
}

std::vector<std::pair<int, int>> convergents(const std::vector<int>& cf) {
    std::vector<std::pair<int, int>> conv;
    if (cf.empty()) return conv;
    
    int p0 = 1, q0 = 0;
    int p1 = cf[0], q1 = 1;
    
    conv.push_back({p1, q1});
    
    for (size_t i = 1; i < cf.size(); ++i) {
        int p2 = cf[i] * p1 + p0;
        int q2 = cf[i] * q1 + q0;
        
        conv.push_back({p2, q2});
        
        p0 = p1;
        q0 = q1;
        p1 = p2;
        q1 = q2;
    }
    return conv;
}
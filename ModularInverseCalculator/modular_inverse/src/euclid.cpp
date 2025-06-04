#include "euclid.h"
#include <stdexcept>

std::tuple<int, int, int> extendEuclid(int v, int c) {
    if (c == 0) {
        return std::make_tuple(v, 1, 0);
    }
    
    auto [gcd, u1, v1] = extendEuclid(c, v % c);
    int u = v1;
    int v_coeff = u1 - (v / c) * v1;
    
    return std::make_tuple(gcd, u, v_coeff);
}

int modInverseEuclid(int v, int c) {
    if (c <= 0) {
        throw std::invalid_argument("Модуль должен быть положительным");
    }
    
    v %= c;
    if (v < 0) v += c;
    if (v == 0) {
        throw std::runtime_error("Обратный элемент не существует");
    }
    
    auto [gcd, u, v_coeff] = extendEuclid(v, c);
    
    if (gcd != 1) {
        throw std::runtime_error("Обратный элемент не существует");
    }
   
    u %= c;
    if (u < 0) u += c;
    
    return u;
}
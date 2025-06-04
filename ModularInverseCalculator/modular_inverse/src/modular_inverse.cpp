#include "modular_inverse.h"
#include "euclid.h"
#include <stdexcept>

int modInverse(int c, int m) {
    if (m <= 0) {
        throw std::invalid_argument("Модуль должен быть положительным");
    }
    
    // Нормализация c
    c %= m;
    if (c < 0) c += m;
    if (c == 0) {
        throw std::runtime_error("Обратный элемент не существует");
    }
    
    auto [gcd, u, v] = extendEuclid(c, m);
    
    if (gcd != 1) {
        throw std::runtime_error("Обратный элемент не существует");
    }
   
    // Нормализация результата
    u %= m;
    if (u < 0) u += m;
    
    return u;
}
#include "modular_inverse.h"
#include <stdexcept>

int modularInverse(int v, int c) {
    if (c <= 0) {
        throw std::invalid_argument("Модуль должен быть положительным");
    }
    
    // Нормализация v
    v %= c;
    if (v < 0) v += c;
    
    // Перебор всех возможных значений
    for (int d = 1; d < c; d++) {
        if ((static_cast<long long>(v) * d) % c == 1) {
            return d;
        }
    }
    
    throw std::runtime_error("Обратный элемент не существует");
}
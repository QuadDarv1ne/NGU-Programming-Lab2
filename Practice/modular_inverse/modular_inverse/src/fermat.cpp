#include "fermat.h"
#include <cmath>
#include <stdexcept>
#include <cstdint> // Для int64_t

/**
 * Проверяет, является ли число простым.
 * @param n Число для проверки.
 * @return true, если число простое, иначе false.
 */
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    int limit = static_cast<int>(std::sqrt(n)) + 1;
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

/**
 * Вычисляет base^exponent mod mod.
 * @param base Основание.
 * @param exponent Показатель степени.
 * @param mod Модуль.
 * @return Результат возведения в степень по модулю.
 */
int64_t mod_exp(int64_t base, int64_t exponent, int64_t mod) {
    if (mod == 1) return 0;
    
    int64_t result = 1;
    base = base % mod;
    
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        
        exponent = exponent >> 1;
        base = (base * base) % mod;
    }
    
    return result;
}

/**
 * Находит обратный элемент по модулю с использованием теоремы Ферма.
 * @param a Число, для которого ищется обратный элемент.
 * @param p Простой модуль.
 * @return Обратный элемент.
 * @throws invalid_argument Если модуль не является простым числом или число кратно модулю.
 */
int64_t modInverseFermat(int64_t a, int64_t p) {
    if (!is_prime(p)) {
        throw std::invalid_argument("Модуль должен быть простым числом");
    }
    
    if (a % p == 0) {
        throw std::invalid_argument("Число не должно быть кратно модулю");
    }
    
    return mod_exp(a, p - 2, p);
}

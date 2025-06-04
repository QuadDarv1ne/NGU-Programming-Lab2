#include "fermat.h"
#include <cmath>
#include <stdexcept>

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

long long mod_exp(long long base, long long exponent, long long mod) {
    if (mod == 1) return 0;
    
    long long result = 1;
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

long long modInverseFermat(long long a, long long p) {
    if (!is_prime(p)) {
        throw std::invalid_argument("Модуль должен быть простым числом");
    }
    
    if (a % p == 0) {
        throw std::invalid_argument("Число не должно быть кратно модулю");
    }
    
    return mod_exp(a, p - 2, p);
}
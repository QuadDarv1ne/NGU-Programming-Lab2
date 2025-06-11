#include "fermat_exponentiation.h"

int modExp(int a, int x, int p) {
    a %= p;
    if (a < 0) a += p;  // Нормализация отрицательных чисел
    
    int result = 1;
    while (x > 0) {
        if (x & 1) {
            result = (static_cast<long long>(result) * a) % p;
        }
        a = (static_cast<long long>(a) * a) % p;
        x >>= 1;
    }
    return result;
}

bool verifyFermat(int a, int p) {
    if (a % p == 0) return true; // 0 ≡ 0 (mod p)
    return modExp(a, p - 1, p) == 1;
}
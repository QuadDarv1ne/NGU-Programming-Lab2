#ifndef FERMAT_H
#define FERMAT_H

// Проверка на простоту
bool is_prime(int n);

// Быстрое возведение в степень по модулю
long long mod_exp(long long base, long long exponent, long long mod);

// Вычисление обратного элемента по теореме Ферма
long long modInverseFermat(long long a, long long p);

#endif
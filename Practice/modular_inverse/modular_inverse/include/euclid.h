#ifndef EUCLID_H
#define EUCLID_H

#include <tuple>

// Функция для вычисления НОД
int gcd(int a, int b);

// Расширенный алгоритм Евклида
std::tuple<int, int, int> extendEuclid(int a, int b);

// Функция для нахождения обратного элемента по модулю
int modInverseEuclid(int a, int m);

// Расширенный алгоритм Евклида, возвращающий только коэффициенты
std::tuple<int, int> extended_gcd(int a, int b);

#endif // EUCLID_H

#ifndef EUCLID_H
#define EUCLID_H

#include <tuple>

// Рекурсивный расширенный алгоритм Евклида
std::tuple<int, int, int> extendEuclid(int v, int c);

// Вычисление обратного элемента по модулю
int modInverseEuclid(int v, int c);

#endif
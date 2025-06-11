#ifndef FRACTION_H
#define FRACTION_H

#include <vector>

// Представление числа в виде цепной дроби
std::vector<int> continued_fraction(int numerator, int denominator);

// Вычисление подходящих дробей
std::vector<std::pair<int, int>> convergents(const std::vector<int>& cf);

#endif
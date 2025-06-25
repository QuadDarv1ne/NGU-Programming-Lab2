#ifndef CONTINUED_FRACTION_H
#define CONTINUED_FRACTION_H

#include <vector>
#include <tuple>
#include <utility>
#include <stdexcept>
#include <cstdint> // Для int64_t

// Основные функции для работы с цепными дробями (int64_t версия)
std::vector<int64_t> computeContinuedFraction(int64_t a, int64_t b);
void computeConvergents(const std::vector<int64_t>& coefficients, 
                       std::vector<std::pair<int64_t, int64_t>>& convergents);
void printContinuedFraction(int64_t a, int64_t b);

// Функции, требуемые интерфейсом (int версия)
std::vector<int> continued_fraction(int a, int b);
std::vector<std::pair<int, int>> convergents(const std::vector<int>& coefficients);
std::tuple<int, int> solve_diophantine(int a, int b, int c);

#endif // CONTINUED_FRACTION_H
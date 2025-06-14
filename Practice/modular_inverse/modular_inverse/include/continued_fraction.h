#ifndef CONTINUED_FRACTION_H
#define CONTINUED_FRACTION_H

#include <vector>
#include <tuple>
#include <utility>
#include <optional>

/**
 * Вычисляет цепную дробь для чисел a и b.
 * @param a Первое число.
 * @param b Второе число.
 * @return Вектор коэффициентов цепной дроби.
 * @throws std::runtime_error Если b равно нулю.
 */
std::vector<int> continued_fraction(int a, int b);

/**
 * Вычисляет подходящие дроби для цепной дроби.
 * @param coefficients Вектор коэффициентов цепной дроби.
 * @return Вектор пар (числитель, знаменатель) подходящих дробей.
 */
std::vector<std::pair<int, int>> convergents(const std::vector<int>& coefficients);

/**
 * Решает диофантово уравнение a*x + b*y = c.
 * @param a Коэффициент при x.
 * @param b Коэффициент при y.
 * @param c Свободный член.
 * @return Кортеж (x, y) - решение уравнения.
 * @throws std::runtime_error Если уравнение не имеет целочисленных решений.
 */
std::tuple<int, int> solve_diophantine(int a, int b, int c);

#endif // CONTINUED_FRACTION_H

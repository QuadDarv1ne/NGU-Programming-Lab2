#include "continued_fraction.h"
#include "euclid.h"
#include <vector>
#include <tuple>
#include <cmath>
#include <stdexcept>

using namespace std;

/**
 * Вычисляет цепную дробь для чисел a и b.
 * @param a Первое число.
 * @param b Второе число.
 * @return Вектор коэффициентов цепной дроби.
 * @throws runtime_error Если b равно нулю.
 */
vector<int> continued_fraction(int a, int b) {
    if (b == 0) {
        throw runtime_error("Деление на ноль");
    }

    vector<int> coefficients;
    a = abs(a);
    b = abs(b);

    while (b != 0) {
        coefficients.push_back(a / b);
        int temp = b;
        b = a % b;
        a = temp;
    }
    return coefficients;
}

/**
 * Вычисляет подходящие дроби для цепной дроби.
 * @param coefficients Вектор коэффициентов цепной дроби.
 * @return Вектор пар (числитель, знаменатель) подходящих дробей.
 */
vector<pair<int, int>> convergents(const vector<int>& coefficients) {
    vector<pair<int, int>> fractions;
    if (coefficients.empty()) return fractions;

    int p0 = 1, q0 = 0;
    int p1 = coefficients[0], q1 = 1;

    fractions.push_back({p1, q1});

    for (size_t i = 1; i < coefficients.size(); ++i) {
        int p2 = coefficients[i] * p1 + p0;
        int q2 = coefficients[i] * q1 + q0;

        fractions.push_back({p2, q2});

        p0 = p1;
        q0 = q1;
        p1 = p2;
        q1 = q2;
    }

    return fractions;
}

/**
 * Решает диофантово уравнение a*x + b*y = c.
 * @param a Коэффициент при x.
 * @param b Коэффициент при y.
 * @param c Свободный член.
 * @return Кортеж (x, y) - решение уравнения.
 * @throws runtime_error Если уравнение не имеет целочисленных решений.
 */
tuple<int, int> solve_diophantine(int a, int b, int c) {
    int g = gcd(a, b);
    if (c % g != 0) {
        throw runtime_error("Уравнение не имеет целочисленных решений");
    }

    a /= g;
    b /= g;
    c /= g;

    int x0, y0;
    tie(x0, y0) = extended_gcd(a, b);

    int x = x0 * c;
    int y = y0 * c;

    return make_tuple(x, y);
}

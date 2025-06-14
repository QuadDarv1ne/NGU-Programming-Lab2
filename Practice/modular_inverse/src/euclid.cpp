#include "euclid.h"
#include <stdexcept>
#include <tuple>

using namespace std;

/**
 * Нормализует число по модулю.
 * @param a Число для нормализации.
 * @param m Модуль.
 * @return Нормализованное число.
 */
int normalize(int a, int m) {
    a %= m;
    if (a < 0) a += m;
    return a;
}

/**
 * Вычисляет наибольший общий делитель (НОД) двух чисел.
 * @param a Первое число.
 * @param b Второе число.
 * @return НОД чисел a и b.
 */
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/**
 * Расширенный алгоритм Евклида для нахождения НОД и коэффициентов.
 * @param a Первое число.
 * @param b Второе число.
 * @return Кортеж (НОД, x, y), где x и y - коэффициенты.
 */
tuple<int, int, int> extendEuclid(int a, int b) {
    if (b == 0) {
        return make_tuple(a, 1, 0);
    }

    auto [gcd, x1, y1] = extendEuclid(b, a % b);
    int x = y1;
    int y = x1 - (a / b) * y1;

    return make_tuple(gcd, x, y);
}

/**
 * Расширенный алгоритм Евклида, возвращающий только коэффициенты.
 * @param a Первое число.
 * @param b Второе число.
 * @return Кортеж (x, y), где x и y - коэффициенты.
 */
tuple<int, int> extended_gcd(int a, int b) {
    if (b == 0) {
        return make_tuple(1, 0);
    } else {
        auto [x, y] = extended_gcd(b, a % b);
        return make_tuple(y, x - (a / b) * y);
    }
}

/**
 * Находит обратный элемент по модулю с использованием расширенного алгоритма Евклида.
 * @param a Число, для которого ищется обратный элемент.
 * @param m Модуль.
 * @return Обратный элемент.
 * @throws invalid_argument Если модуль не положительный.
 * @throws runtime_error Если обратный элемент не существует.
 */
int modInverseEuclid(int a, int m) {
    if (m <= 0) {
        throw invalid_argument("Модуль должен быть положительным");
    }

    a = normalize(a, m);
    if (a == 0) {
        throw runtime_error("Обратный элемент не существует");
    }

    auto [gcd, x, y] = extendEuclid(a, m);

    if (gcd != 1) {
        throw runtime_error("Обратный элемент не существует");
    }

    x = normalize(x, m);
    return x;
}

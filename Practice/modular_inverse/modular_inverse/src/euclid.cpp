#include "euclid.h"
#include <stdexcept>
#include <tuple>

using namespace std;

// Функция для вычисления НОД
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Расширенный алгоритм Евклида
tuple<int, int, int> extendEuclid(int a, int b) {
    if (b == 0) {
        return make_tuple(a, 1, 0);
    }

    auto [gcd, x1, y1] = extendEuclid(b, a % b);
    int x = y1;
    int y = x1 - (a / b) * y1;

    return make_tuple(gcd, x, y);
}

// Расширенный алгоритм Евклида, возвращающий только коэффициенты
tuple<int, int> extended_gcd(int a, int b) {
    if (b == 0) {
        return make_tuple(1, 0);
    } else {
        auto [x, y] = extended_gcd(b, a % b);
        return make_tuple(y, x - (a / b) * y);
    }
}

// Функция для нахождения обратного элемента с использованием расширенного алгоритма Евклида
int modInverseEuclid(int a, int m) {
    if (m <= 0) {
        throw invalid_argument("Модуль должен быть положительным");
    }

    a %= m;
    if (a < 0) a += m;
    if (a == 0) {
        throw runtime_error("Обратный элемент не существует");
    }

    auto [gcd, x, y] = extendEuclid(a, m);

    if (gcd != 1) {
        throw runtime_error("Обратный элемент не существует");
    }

    x %= m;
    if (x < 0) x += m;

    return x;
}

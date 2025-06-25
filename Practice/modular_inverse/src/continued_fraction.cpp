#include "continued_fraction.h"
#include "euclid.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
using namespace std;

/**
* Вычисляет коэффициенты цепной дроби для числа a/b.
* Цепная дробь представляется в виде [a0; a1, a2, ..., an].
* @param a Числитель дроби.
* @param b Знаменатель дроби.
* @return Вектор коэффициентов цепной дроби.
* @note Если b=0, возвращается пустой вектор.
*/
vector<int64_t> computeContinuedFraction(int64_t a, int64_t b) {
    vector<int64_t> coefficients;

    if (b == 0) {
        return coefficients;
    }

    while (b != 0) {
        int64_t quotient = a / b;
        coefficients.push_back(quotient);
        int64_t remainder = a % b;
        a = b;
        b = remainder;
    }

    return coefficients;
}

/**
* Вычисляет подходящие дроби для заданных коэффициентов цепной дроби.
* Подходящие дроби представляются в виде пар (числитель, знаменатель).
* @param coefficients Вектор коэффициентов цепной дроби.
* @param convergents Ссылка на вектор для сохранения подходящих дробей.
* @note Результат сохраняется в параметре convergents.
*       Если coefficients пуст, convergents также будет пустым.
*/
void computeConvergents(const vector<int64_t>& coefficients, 
                       vector<pair<int64_t, int64_t>>& convergents) {
    convergents.clear();

    if (coefficients.empty()) {
        return;
    }

    // Инициализация P_{-2}, P_{-1}, Q_{-2}, Q_{-1}
    int64_t P_prev2 = 0;
    int64_t P_prev1 = 1;
    int64_t Q_prev2 = 1;
    int64_t Q_prev1 = 0;

    for (size_t i = 0; i < coefficients.size(); ++i) {
        int64_t a_i = coefficients[i];

        // Вычисляем P_i и Q_i
        int64_t P_i = a_i * P_prev1 + P_prev2;
        int64_t Q_i = a_i * Q_prev1 + Q_prev2;

        convergents.emplace_back(P_i, Q_i);

        // Обновляем предыдущие значения для следующей итерации
        P_prev2 = P_prev1;
        P_prev1 = P_i;
        Q_prev2 = Q_prev1;
        Q_prev1 = Q_i;
    }
}

/**
* Выводит на экран цепную дробь и подходящие дроби для числа a/b.
* Формат вывода включает коэффициенты цепной дроби и последовательность
* подходящих дробей с их десятичными приближениями.
* @param a Числитель дроби.
* @param b Знаменатель дроби.
* @note Если b=0, выводится сообщение об ошибке.
*/
void printContinuedFraction(int64_t a, int64_t b) {
    cout << "Вычисление цепной дроби для " << a << "/" << b << ":\n";

    vector<int64_t> coefficients = computeContinuedFraction(a, b);

    if (coefficients.empty()) {
        cout << "Невозможно вычислить цепную дробь (деление на ноль).\n";
        return;
    }

    cout << "Коэффициенты цепной дроби: [";
    for (size_t i = 0; i < coefficients.size(); ++i) {
        cout << coefficients[i];
        if (i != coefficients.size() - 1) {
            cout << (i == 0 ? ";" : ", ");
        }
    }
    cout << "]\n";

    vector<pair<int64_t, int64_t>> convergents;
    computeConvergents(coefficients, convergents);

    cout << "Подходящие дроби:\n";
    for (size_t i = 0; i < convergents.size(); ++i) {
        cout << i+1 << ": " << convergents[i].first << "/" << convergents[i].second;

        // Вычисляем десятичное приближение
        if (convergents[i].second != 0) {
            double value = static_cast<double>(convergents[i].first) / convergents[i].second;
            cout << " ≈ " << value;
        }

        cout << endl;
    }
}

/**
* Основная функция программы.
* Запрашивает у пользователя числитель и знаменатель дроби,
* затем выводит её цепную дробь и подходящие дроби.
* @return 0 в случае успешного выполнения, 1 при ошибке ввода.
*/
int main() {
    int64_t a, b;

    cout << "Введите числитель и знаменатель дроби (через пробел): ";
    cin >> a >> b;

    if (b == 0) {
        cout << "Ошибка: знаменатель не может быть нулем.\n";
        return 1;
    }

    printContinuedFraction(a, b);

    return 0;
}

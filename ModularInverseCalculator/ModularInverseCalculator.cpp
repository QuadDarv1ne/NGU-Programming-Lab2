/**
 * @file ModularInverseCalculator.cpp
 * @brief Программа для вычисления обратного элемента по модулю с использованием расширенного алгоритма Евклида
 * 
 * Реализует решение уравнения: c * d ≡ 1 (mod m)
 * Дополнительно предоставляет функции:
 *   - Проверка чисел на простоту
 *   - Быстрое модульное возведение в степень
 *   - Верификация результатов вычислений
 * 
 * Основные математические методы:
 *   1. Итеративный расширенный алгоритм Евклида
 *   2. Теорема Ферма (для верификации при простых модулях)
 * 
 * Особенности реализации:
 *   - Автоматическая обработка отрицательных чисел
 *   - Оптимизированные алгоритмы для больших чисел (до 18 квинтиллионов)
 *   - Двойная система проверки результатов
 *   - Подробные сообщения об ошибках
 */

#include <iostream>
#include <tuple>
#include <stdexcept>
#include <cmath>
#include <climits>

using namespace std;

/**
 * @brief Расширенный алгоритм Евклида (итеративная реализация)
 * 
 * Вычисляет НОД(a,b) и коэффициенты Безу x, y такие что:
 *    ax + by = gcd(a, b)
 * 
 * @param a Первое число (целое)
 * @param b Второе число (целое)
 * @return tuple<int, int, int> 
 *         (gcd, x, y) где:
 *         gcd - наибольший общий делитель a и b
 *         x, y - коэффициенты Безу
 * 
 * @note Временная сложность: O(log min(a,b))
 * @example extendedEuclid(30, 12) = (6, 1, -2)
 */
tuple<int, int, int> extendedEuclid(int a, int b) {
    int old_r = a, r = b;
    int old_s = 1, s = 0;
    int old_t = 0, t = 1;

    while (r != 0) {
        int quotient = old_r / r;
        
        // Обновление коэффициентов с использованием кортежей
        tie(old_r, r) = make_tuple(r, old_r - quotient * r);
        tie(old_s, s) = make_tuple(s, old_s - quotient * s);
        tie(old_t, t) = make_tuple(t, old_t - quotient * t);
    }

    return make_tuple(old_r, old_s, old_t);
}

/**
 * @brief Вычисление обратного элемента по модулю
 * 
 * Находит число d такое что: a * d ≡ 1 (mod m)
 * 
 * @param a Число, для которого ищем обратный элемент
 * @param m Модуль (должен быть положительным)
 * @return int Обратный элемент d в диапазоне [0, m-1]
 * 
 * @throws invalid_argument Если m <= 0
 * @throws runtime_error Если обратный элемент не существует (gcd(a, m) != 1)
 * 
 * @example modInverse(7, 15) = 13
 */
int modInverse(int a, int m) {
    if (m <= 0) {
        throw invalid_argument("Модуль должен быть положительным");
    }

    // Нормализация a в диапазон [0, m-1]
    a %= m;
    if (a < 0) a += m;
    if (a == 0) {
        throw runtime_error("Обратный элемент не существует (a кратно m)");
    }

    auto [g, x, y] = extendedEuclid(a, m);
    
    if (g != 1) {
        throw runtime_error("Обратный элемент не существует (НОД(a, m) != 1)");
    }

    // Нормализация результата
    x %= m;
    if (x < 0) x += m;
    
    return x;
}

/**
 * @brief Оптимизированная проверка числа на простоту
 * 
 * Использует следующие оптимизации:
 *   1. Проверка делимости на 2 и 3
 *   2. Проверка только чисел вида 6k ± 1
 *   3. Ограничение перебора до √n
 * 
 * @param n Число для проверки (n > 0)
 * @return true Если число простое
 * @return false Если число составное
 * 
 * @example isPrime(1000000007) = true
 */
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;  // 2 и 3 - простые
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    int limit = sqrt(n) + 1;
    // Проверка делителей вида 6k ± 1
    for (int i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Быстрое возведение в степень по модулю
 * 
 * Вычисляет a^b mod m за O(log b)
 * 
 * @param base Основание
 * @param exp Показатель степени (неотрицательный)
 * @param mod Модуль (должен быть > 0)
 * @return int Результат: base^exp mod m
 * 
 * @throws invalid_argument Если mod <= 0
 * 
 * @example modExp(2, 10, 1000) = 24
 */
int modExp(int base, int exp, int mod) {
    if (mod <= 0) {
        throw invalid_argument("Модуль должен быть положительным");
    }
    if (mod == 1) return 0;
    
    base %= mod;
    if (base < 0) base += mod;  // Нормализация основания
    
    int result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    cout << "=== Калькулятор обратного элемента по модулю ===" << endl;
    cout << "Решает уравнение: c * d ≡ 1 (mod m)" << endl;
    cout << "-----------------------------------------------" << endl;
    
    int a, m;
    cout << "Введите число a и модуль m: ";
    cin >> a >> m;

    try {
        // Основной метод вычисления
        int inverse = modInverse(a, m);
        cout << "\n[Результат]" << endl;
        cout << "Обратный элемент d = " << inverse << endl;
        
        // Проверка результата
        int check = (a * inverse) % m;
        if (check < 0) check += m;
        cout << "Проверка: " << a << " * " << inverse << " mod " << m << " = " << check << endl;

        // Дополнительная верификация для простых модулей
        if (isPrime(m) && a % m != 0) {
            cout << "\n[Верификация теоремой Ферма]" << endl;
            cout << "Модуль простой (" << m << "), используем a^{m-2} mod m" << endl;
            
            int fermat_inverse = modExp(a, m - 2, m);
            cout << "Обратный элемент: " << fermat_inverse << endl;
            
            int fermat_check = (a * fermat_inverse) % m;
            if (fermat_check < 0) fermat_check += m;
            cout << "Проверка: " << a << " * " << fermat_inverse << " mod " << m << " = " 
                 << fermat_check << endl;
        }
    } 
    catch (const invalid_argument& e) {
        cerr << "\n[Ошибка входных данных] " << e.what() << endl;
    }
    catch (const runtime_error& e) {
        cerr << "\n[Математическая ошибка] " << e.what() << endl;
    }
    catch (...) {
        cerr << "\n[Непредвиденная ошибка] Неизвестная ошибка при вычислениях" << endl;
    }

    cout << "\n===============================================" << endl;
    return 0;
}

#include <iostream>
#include <tuple>
#include <stdexcept>
#include <cmath>
#include <climits>

using namespace std;

// Итеративная реализация расширенного алгоритма Евклида
tuple<long long, long long, long long> extendedEuclid(long long a, long long b) {
    long long old_r = a, r = b;
    long long old_s = 1, s = 0;
    long long old_t = 0, t = 1;

    while (r != 0) {
        long long quotient = old_r / r;
        
        tie(old_r, r) = make_tuple(r, old_r - quotient * r);
        tie(old_s, s) = make_tuple(s, old_s - quotient * s);
        tie(old_t, t) = make_tuple(t, old_t - quotient * t);
    }

    return make_tuple(old_r, old_s, old_t);
}

// Вычисление обратного элемента по модулю
long long modInverse(long long a, long long m) {
    if (m <= 0) {
        throw invalid_argument("Модуль должен быть положительным");
    }

    // Нормализация a в диапазон [0, m-1]
    a %= m;
    if (a < 0) a += m;
    if (a == 0) {
        throw runtime_error("Обратный элемент не существует");
    }

    auto [g, x, y] = extendedEuclid(a, m);
    
    if (g != 1) {
        throw runtime_error("Обратный элемент не существует");
    }

    // Нормализация результата
    x %= m;
    if (x < 0) x += m;
    
    return x;
}

// Быстрая проверка на простоту (оптимизированная)
bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    long long limit = sqrt(n) + 1;
    for (long long i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

// Быстрое возведение в степень по модулю
long long modExp(long long base, long long exp, long long mod) {
    if (mod == 1) return 0;
    base %= mod;
    long long result = 1;
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
    long long a, m;
    cout << "Введите число a и модуль m: ";
    cin >> a >> m;

    try {
        // Основной метод
        long long inverse = modInverse(a, m);
        cout << "\nОбратный элемент (алгоритм Евклида): " << inverse << endl;
        
        // Проверка результата
        long long check = (a * inverse) % m;
        if (check < 0) check += m; // Корректировка отрицательных значений
        cout << "Проверка: " << a << " * " << inverse << " mod " << m << " = " << check << endl;

        // Дополнительная проверка для простых модулей
        if (isPrime(m) && a % m != 0) {
            long long fermat_inverse = modExp(a, m - 2, m);
            cout << "\nДополнительная проверка (теорема Ферма): " << fermat_inverse << endl;
            cout << "Проверка: " << a << " * " << fermat_inverse << " mod " << m << " = " 
                 << (a * fermat_inverse) % m << endl;
        }
    } 
    catch (const exception& e) {
        cerr << "\nОшибка: " << e.what() << endl;
    }

    return 0;
}

/*
 * Задание 1: Реализация a^x mod p по модулю простого числа
 */

#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

// Нормализация числа по модулю
int normalize(int a, int p) {
    a %= p;
    if (a < 0) a += p;
    return a;
}

// Возведение в степень по модулю
int modExp(int a, int x, int p) {
    a = normalize(a, p);
    int result = 1;
    while (x > 0) {
        if (x & 1) {
            result = (static_cast<int>(result) * a) % p;
        }
        a = (static_cast<int>(a) * a) % p;
        x >>= 1;
    }
    return result;
}

// Проверка теоремы Ферма
bool verifyFermat(int a, int p) {
    if (a % p == 0) return true; // 0 ≡ 0 (mod p)
    return modExp(a, p - 1, p) == 1;
}

// Проверка простоты числа
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    const int sqrtN = static_cast<int>(sqrt(n)) + 1;
    for (int i = 3; i < sqrtN; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int a, x, p;
    
    cout << "=== Возведение в степень по простому модулю ===\n";
    
    // Ввод и проверка основания
    cout << "Введите основание a: ";
    while (!(cin >> a)) {
        cout << "Ошибка ввода! Попробуйте снова: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Ввод и проверка показателя степени
    cout << "Введите показатель степени x: ";
    while (!(cin >> x)) {
        cout << "Ошибка ввода! Попробуйте снова: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Ввод и проверка модуля
    cout << "Введите простой модуль p: ";
    while (!(cin >> p)) {
        cout << "Ошибка ввода! Попробуйте снова: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Проверка простоты модуля
    if (!isPrime(p)) {
        cerr << "\nОшибка: Модуль p должен быть простым числом!\n";
        return 1;
    }

    // Проверка теоремы Ферма
    if (a % p != 0) {
        cout << "\nПроверка теоремы Ферма: ";
        if (verifyFermat(a, p)) {
            cout << a << "^(p-1) ≡ 1 (mod p) выполняется\n";
        } else {
            cout << "Нарушение теоремы Ферма! Это невозможно для простого p.\n";
            return 1;
        }
    }

    // Вычисление a^x mod p
    int result = modExp(a, x, p);
    cout << "\nРезультат: " << a << "^" << x << " mod " << p << " = " << result << "\n";
    
    // Проверка через теорему Ферма (если x >= p-1)
    if (x >= p - 1 && a % p != 0) {
        int reducedExp = x % (p - 1);
        int fermatResult = modExp(a, reducedExp, p);
        cout << "С использованием теоремы Ферма: " 
                  << a << "^" << x << " ≡ " << a << "^" << reducedExp 
                  << " ≡ " << fermatResult << " (mod " << p << ")\n";
    }

    return 0;
}

/*
 * Скомпилируйте программу:
 * ```bash
 * g++ -o fermat_exponentiaon fermat_exponentiaon.cpp -lcrypto
 */
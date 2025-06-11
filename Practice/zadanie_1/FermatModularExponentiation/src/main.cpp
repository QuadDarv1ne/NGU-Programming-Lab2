#include <iostream>
#include "fermat_exponentiation.h"
#include "prime_check.h"

int main() {
    int a, x, p;
    
    std::cout << "=== Возведение в степень по простому модулю ===\n";
    std::cout << "Введите основание a: ";
    std::cin >> a;
    std::cout << "Введите показатель степени x: ";
    std::cin >> x;
    std::cout << "Введите простой модуль p: ";
    std::cin >> p;

    // Проверка простоты модуля
    if (!isPrime(p)) {
        std::cerr << "\nОшибка: Модуль p должен быть простым числом!\n";
        return 1;
    }

    // Проверка теоремы Ферма
    if (a % p != 0) {
        std::cout << "\nПроверка теоремы Ферма: ";
        if (verifyFermat(a, p)) {
            std::cout << a << "^(p-1) ≡ 1 (mod p) выполняется\n";
        } else {
            std::cout << "Нарушение теоремы Ферма! Это невозможно для простого p.\n";
            return 1;
        }
    }

    // Вычисление a^x mod p
    int result = modExp(a, x, p);
    std::cout << "\nРезультат: " << a << "^" << x << " mod " << p << " = " << result << "\n";
    
    // Проверка через теорему Ферма (если x >= p-1)
    if (x >= p - 1 && a % p != 0) {
        int reducedExp = x % (p - 1);
        int fermatResult = modExp(a, reducedExp, p);
        std::cout << "С использованием теоремы Ферма: " 
                  << a << "^" << x << " ≡ " << a << "^" << reducedExp 
                  << " ≡ " << fermatResult << " (mod " << p << ")\n";
    }

    return 0;
}
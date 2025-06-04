#include "menu.h"
#include "euclid.h"
#include "fermat.h"
#include "rsa.h"
#include <iostream>
#include <stdexcept>

void display_menu() {
    std::cout << "\n=== Модулярная арифметика и криптография ===" << std::endl;
    std::cout << "1. Вычисление обратного элемента через алгоритм Евклида" << std::endl;
    std::cout << "2. Вычисление обратного элемента через теорему Ферма" << std::endl;
    std::cout << "3. Демонстрация RSA шифрования" << std::endl;
    std::cout << "4. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
}

void run_main_menu() {
    int choice;
    
    do {
        display_menu();
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: {
                    int v, c;
                    std::cout << "Введите v и c: ";
                    std::cin >> v >> c;
                    
                    int d = modInverseEuclid(v, c);
                    std::cout << "Обратный элемент d = " << d << std::endl;
                    
                    long long check = static_cast<long long>(v) * d % c;
                    if (check < 0) check += c;
                    std::cout << "Проверка: " << v << " * " << d << " mod " << c << " = " << check << std::endl;
                    break;
                }
                case 2: {
                    long long a, p;
                    std::cout << "Введите число a: ";
                    std::cin >> a;
                    std::cout << "Введите простой модуль p: ";
                    std::cin >> p;
                    
                    long long inverse = modInverseFermat(a, p);
                    std::cout << "Обратный элемент: " << a << "^{-1} mod " << p << " = " << inverse << std::endl;
                    std::cout << "Проверка: (" << a << " * " << inverse << ") mod " << p << " = " 
                              << (a * inverse) % p << std::endl;
                    
                    if (a % p != 0) {
                        long long fermat = mod_exp(a, p - 1, p);
                        std::cout << "Проверка малой теоремы Ферма: " << a << "^{" << p-1 << "} mod " << p 
                                  << " = " << fermat << std::endl;
                    }
                    break;
                }
                case 3: {
                    run_rsa_demo();
                    break;
                }
                case 4:
                    std::cout << "Выход из программы..." << std::endl;
                    break;
                default:
                    std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
            }
        } 
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
        
    } while (choice != 4);
}
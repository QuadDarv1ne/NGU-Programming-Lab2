#include "menu.h"
#include "euclid.h"
#include "fermat.h"
#include "modular_inverse.h"
#include "continued_fraction.h"
#include "rsa_crypto.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>  // Для abs()
#include <limits>
#include <ios>
#include <cstdint> // Для int64_t
#include <clocale>
#include "set_encoding.h"

using namespace std;

// Функция для безопасного ввода целого числа
int safe_input_int(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Ошибка ввода! Попробуйте снова: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

// Функция для безопасного ввода int64_t
int64_t safe_input_int64(const string& prompt) {
    int64_t value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Ошибка ввода! Попробуйте снова: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

void display_menu() {
    cout << "\n=== Модулярная арифметика и криптография ===" << endl;
    cout << "1. Базовое вычисление обратного элемента (перебор)" << endl;
    cout << "2. Вычисление обратного элемента через алгоритм Евклида" << endl;
    cout << "3. Вычисление обратного элемента через теорему Ферма" << endl;
    cout << "4. Демонстрация RSA шифрования (openssl)" << endl;
    cout << "5. Вычисление цепной дроби и решение диофантова уравнения" << endl;
    cout << "6. RSA Шифрование/Дешифрование (полный функционал)" << endl;
    cout << "7. Выход "<< endl;
    cout << "Выберите опцию: ";
}

void run_main_menu() {
    setConsoleEncoding();

    int choice;
    
    do {
        display_menu();
        choice = safe_input_int("");
        
        try {
            switch (choice) {
                case 1: {
                    int v = safe_input_int("Введите v: ");
                    int c = safe_input_int("Введите c: ");
                    
                    int d = modularInverse(v, c);
                    cout << "Обратный элемент d = " << d << endl;
                    
                    int64_t check = static_cast<int64_t>(v) * d % c;
                    if (check < 0) check += c;
                    cout << "Проверка: " << v << " * " << d << " mod " << c << " = " << check << endl;
                    break;
                }
                case 2: {
                    int v = safe_input_int("Введите v: ");
                    int c = safe_input_int("Введите c: ");
                    
                    int d = modInverseEuclid(v, c);
                    cout << "Обратный элемент d = " << d << endl;
                    
                    int64_t check = static_cast<int64_t>(v) * d % c;
                    if (check < 0) check += c;
                    cout << "Проверка: " << v << " * " << d << " mod " << c << " = " << check << endl;
                    break;
                }
                case 3: {
                    int64_t a = safe_input_int64("Введите число a: ");
                    int64_t p = safe_input_int64("Введите простой модуль p: ");
                    
                    int64_t inverse = modInverseFermat(a, p);
                    cout << "Обратный элемент: " << a << "^{-1} mod " << p << " = " << inverse << endl;
                    cout << "Проверка: (" << a << " * " << inverse << ") mod " << p << " = " 
                         << (a * inverse) % p << endl;
                    
                    if (a % p != 0) {
                        int64_t fermat = mod_exp(a, p - 1, p);
                        cout << "Проверка малой теоремы Ферма: " << a << "^{" << p-1 << "} mod " << p 
                             << " = " << fermat << endl;
                    }
                    break;
                }
                case 4: {
                    run_rsa_crypto();
                    break;
                }
                case 5: {
                    int a = safe_input_int("Введите коэффициент a: ");
                    int b = safe_input_int("Введите коэффициент b: ");
                    int c = safe_input_int("Введите свободный член c: ");
                    
                    try {
                        auto [x, y] = solve_diophantine(a, b, c);
                        cout << "Решено: " << a << "*(" << x << ") + " 
                             << b << "*(" << y << ") = " << c << "\n";
                        cout << "Проверка: " << a*x + b*y << " = " << c << "\n\n";
                        
                        int a_abs = abs(a);
                        int b_abs = abs(b);
                        if (b_abs == 0) {
                            cerr << "Ошибка: знаменатель не может быть нулем" << endl;
                        } else {
                            vector<int> cf = continued_fraction(a_abs, b_abs);
                            cout << "Цепная дробь для " << a_abs << "/" << b_abs << ":\n[";
                            for (size_t i = 0; i < cf.size(); ++i) {
                                cout << cf[i];
                                if (i < cf.size()-1) cout << ", ";
                            }
                            cout << "]\n";
                            
                            vector<pair<int, int>> conv = convergents(cf);
                            cout << "\nПодходящие дроби:\n";
                            for (const auto& f : conv) {
                                cout << f.first << "/" << f.second << "\n";
                            }
                        }
                    } catch (const exception& e) {
                        cerr << "Ошибка: " << e.what() << endl;
                    }
                    break;
                }
                case 6: {
                    run_rsa_crypto();
                    break;
                }
                case 7: {
                    cout << "Выход из программы..." << endl;
                    return;
                }
                default:
                    cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
    } while (true); // Бесконечный цикл с выходом через return
}

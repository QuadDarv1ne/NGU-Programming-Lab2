#include "menu.h"
#include "euclid.h"
#include "fermat.h"
#include "modular_inverse.h"
#include "continued_fraction.h"
#include "rsa.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>  // Для abs()
#include <limits>
#include <ios>

using namespace std;

void display_menu() {
    cout << "\n=== Модулярная арифметика и криптография ===" << endl;
    cout << "1. Базовое вычисление обратного элемента (перебор)" << endl;
    cout << "2. Вычисление обратного элемента через алгоритм Евклида" << endl;
    cout << "3. Вычисление обратного элемента через теорему Ферма" << endl;
    cout << "4. Демонстрация RSA шифрования" << endl;
    cout << "5. Вычисление цепной дроби и решение диофантова уравнения" << endl;
    cout << "6. Выход "<< endl;
    cout << "Выберите опцию: ";
}

void run_main_menu() {
    int choice;
    
    do {
        display_menu();
        cin >> choice;
        
        try {
            switch (choice) {
                case 1: {  // Базовый метод (перебор)
                    int v, c;
                    cout << "Введите v и c: ";
                    cin >> v >> c;
                    
                    int d = modularInverse(v, c);
                    cout << "Обратный элемент d = " << d << endl;
                    
                    long long check = static_cast<long long>(v) * d % c;
                    if (check < 0) check += c;
                    cout << "Проверка: " << v << " * " << d << " mod " << c << " = " << check << endl;
                    break;
                }
                case 2: {  // Алгоритм Евклида
                    int v, c;
                    cout << "Введите v и c: ";
                    cin >> v >> c;
                    
                    int d = modInverseEuclid(v, c);
                    cout << "Обратный элемент d = " << d << endl;
                    
                    long long check = static_cast<long long>(v) * d % c;
                    if (check < 0) check += c;
                    cout << "Проверка: " << v << " * " << d << " mod " << c << " = " << check << endl;
                    break;
                }
                case 3: {  // Теорема Ферма
                    long long a, p;
                    cout << "Введите число a: ";
                    cin >> a;
                    cout << "Введите простой модуль p: ";
                    cin >> p;
                    
                    long long inverse = modInverseFermat(a, p);
                    cout << "Обратный элемент: " << a << "^{-1} mod " << p << " = " << inverse << endl;
                    cout << "Проверка: (" << a << " * " << inverse << ") mod " << p << " = " 
                              << (a * inverse) % p << endl;
                    
                    if (a % p != 0) {
                        long long fermat = mod_exp(a, p - 1, p);
                        cout << "Проверка малой теоремы Ферма: " << a << "^{" << p-1 << "} mod " << p 
                                  << " = " << fermat << endl;
                    }
                    break;
                }
                case 4: {  // RSA
                    run_rsa_demo();
                    break;
                }
                case 5: {  // Цепная дробь и диофантово уравнение
                    int a, b, c;
                    cout << "Введите коэффициенты a, b и c для уравнения a*x + b*y = c: ";
                    cin >> a >> b >> c;
                    
                    try {
                        // Решение уравнения
                        auto [x, y] = solve_diophantine(a, b, c);
                        
                        // Вывод результатов
                        cout << "Решено: " << a << "*(" << x << ") + " 
                                << b << "*(" << y << ") = " << c << "\n";
                        cout << "Проверка: " << a*x + b*y << " = " << c << "\n\n";
                        
                        // Вывод цепной дроби (используем абсолютные значения)
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
                            
                            // Вывод подходящих дробей
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
                case 6: {  // Выход
                    cout << "Выход из программы..." << endl;
                    return;  // Немедленный выход из функции
                }
                default:
                    cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            // Сбрасываем флаги ошибок ввода
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
    } while (true);  // Бесконечный цикл с выходом через return
}
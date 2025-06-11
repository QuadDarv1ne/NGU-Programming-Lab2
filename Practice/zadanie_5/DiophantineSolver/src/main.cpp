#include <iostream>
#include "diophantine.h"
#include "fraction.h"

int main() {
    const int a = 237;
    const int b = 44;
    const int c = 1;
    
    try {
        // Решение уравнения
        auto [x, y] = solve_diophantine(a, b, c);
        
        // Вывод результатов
        std::cout << "Решено: " << a << "*(" << x << ") + " 
                  << b << "*(" << y << ") = " << c << "\n";
        std::cout << "Проверка: " << a*x + b*y << " = " << c << "\n\n";
        
        // Вывод цепной дроби
        std::vector<int> cf = continued_fraction(a, b);
        std::cout << "Цепная дробь для " << a << "/" << b << ":\n[";
        for (size_t i = 0; i < cf.size(); ++i) {
            std::cout << cf[i];
            if (i < cf.size()-1) std::cout << ", ";
        }
        std::cout << "]\n";
        
        // Вывод подходящих дробей
        std::vector<std::pair<int, int>> conv = convergents(cf);
        std::cout << "\nПодходящие дроби:\n";
        for (const auto& f : conv) {
            std::cout << f.first << "/" << f.second << "\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
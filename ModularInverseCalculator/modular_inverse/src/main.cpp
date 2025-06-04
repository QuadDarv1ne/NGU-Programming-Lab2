#include <iostream>
#include "modular_inverse.h"

int main() {
    int c, m;
    std::cout << "Введите c и m: ";
    std::cin >> c >> m;
    
    try {
        int d = modInverse(c, m);
        
        std::cout << "Обратный элемент d = " << d << std::endl;
        
        // Проверка с использованием long long для избежания переполнения
        long long check = static_cast<long long>(c) * d % m;
        if (check < 0) check += m;
        std::cout << "Проверка: " << c << " * " << d << " mod " << m << " = " << check << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    
    return 0;
}
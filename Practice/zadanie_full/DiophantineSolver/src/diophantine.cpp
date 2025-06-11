#include "diophantine.h"
#include "fraction.h"
#include <stdexcept>
#include <cmath>

std::pair<int, int> solve_diophantine(int a, int b, int c) {
    // Проверка входных данных
    if (a == 0 && b == 0) {
        if (c == 0) return {0, 0};
        throw std::runtime_error("Нет решения: оба коэффициента нулевые");
    }
    
    // Вычисляем цепную дробь для a/b
    std::vector<int> cf = continued_fraction(std::abs(a), std::abs(b));
    
    // Находим подходящие дроби
    std::vector<std::pair<int, int>> conv = convergents(cf);
    
    // Предпоследняя подходящая дробь
    if (conv.size() < 2) {
        throw std::runtime_error("Недостаточно подходящих дробей");
    }
    
    size_t n = conv.size() - 1;
    int pn = conv[n-1].first;
    int qn = conv[n-1].second;
    
    // Учет знаков
    if (a < 0) pn = -pn;
    if (b < 0) qn = -qn;
    
    // Проверка НОД
    int gcd = pn * std::abs(a) + qn * std::abs(b);
    if (c % gcd != 0) {
        throw std::runtime_error("Уравнение неразрешимо: c не делится на НОД(a,b)");
    }
    
    // Масштабирование решения
    int k = c / gcd;
    return {k * pn, k * qn};
}
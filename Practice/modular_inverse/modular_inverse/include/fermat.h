#ifndef FERMAT_H
#define FERMAT_H

#include <cstdint> // Для int64_t

/**
 * Проверяет, является ли число простым.
 * @param n Число для проверки.
 * @return true, если число простое, иначе false.
 */
bool is_prime(int n);

/**
 * Вычисляет base^exponent mod mod.
 * @param base Основание.
 * @param exponent Показатель степени.
 * @param mod Модуль.
 * @return Результат возведения в степень по модулю.
 */
int64_t mod_exp(int64_t base, int64_t exponent, int64_t mod);

/**
 * Находит обратный элемент по модулю с использованием теоремы Ферма.
 * @param a Число, для которого ищется обратный элемент.
 * @param p Простой модуль.
 * @return Обратный элемент.
 * @throws std::invalid_argument Если модуль не является простым числом или число кратно модулю.
 */
int64_t modInverseFermat(int64_t a, int64_t p);

#endif

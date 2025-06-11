#ifndef FERMAT_EXPONENTIATION_H
#define FERMAT_EXPONENTIATION_H

/// Вычисляет a^x mod p с использованием бинарного возведения в степень
/// @param a Основание
/// @param x Показатель степени
/// @param p Простой модуль
/// @return Результат вычисления a^x mod p
int modExp(int a, int x, int p);

/// Проверяет выполнение малой теоремы Ферма
/// @param a Основание
/// @param p Простой модуль
/// @return true, если теорема выполняется, иначе false
bool verifyFermat(int a, int p);

#endif
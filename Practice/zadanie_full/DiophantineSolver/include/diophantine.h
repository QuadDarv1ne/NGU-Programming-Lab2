#ifndef DIOPHANTINE_H
#define DIOPHANTINE_H

#include <utility>

// Решение диофантова уравнения ax + by = c
std::pair<int, int> solve_diophantine(int a, int b, int c);

#endif
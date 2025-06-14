#ifndef CONTINUED_FRACTION_H
#define CONTINUED_FRACTION_H

#include <vector>
#include <tuple>
#include <utility>

std::vector<int> continued_fraction(int a, int b);
std::vector<std::pair<int, int>> convergents(const std::vector<int>& coefficients);
std::tuple<int, int> solve_diophantine(int a, int b, int c);

#endif // CONTINUED_FRACTION_H
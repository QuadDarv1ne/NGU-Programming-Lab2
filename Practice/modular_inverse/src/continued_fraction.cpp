#include "continued_fraction.h"
#include "euclid.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <stdexcept>

using namespace std;

vector<int64_t> computeContinuedFraction(int64_t a, int64_t b) {
    vector<int64_t> coefficients;

    if (b == 0) {
        throw runtime_error("Знаменатель не может быть нулем");
    }

    a = abs(a);
    b = abs(b);

    while (b != 0) {
        int64_t quotient = a / b;
        coefficients.push_back(quotient);
        int64_t remainder = a % b;
        a = b;
        b = remainder;
    }

    return coefficients;
}

void computeConvergents(const vector<int64_t>& coefficients, 
                       vector<pair<int64_t, int64_t>>& convergents) {
    convergents.clear();

    if (coefficients.empty()) {
        return;
    }

    int64_t P_prev2 = 0;
    int64_t P_prev1 = 1;
    int64_t Q_prev2 = 1;
    int64_t Q_prev1 = 0;

    for (size_t i = 0; i < coefficients.size(); ++i) {
        int64_t a_i = coefficients[i];
        int64_t P_i = a_i * P_prev1 + P_prev2;
        int64_t Q_i = a_i * Q_prev1 + Q_prev2;

        convergents.emplace_back(P_i, Q_i);

        P_prev2 = P_prev1;
        P_prev1 = P_i;
        Q_prev2 = Q_prev1;
        Q_prev1 = Q_i;
    }
}

// Реализация функций для интерфейса (int версия)
vector<int> continued_fraction(int a, int b) {
    auto result = computeContinuedFraction(static_cast<int64_t>(a), static_cast<int64_t>(b));
    return vector<int>(result.begin(), result.end());
}

vector<pair<int, int>> convergents(const vector<int>& coefficients) {
    vector<pair<int64_t, int64_t>> temp;
    vector<pair<int, int>> result;
    
    vector<int64_t> coeffs(coefficients.begin(), coefficients.end());
    computeConvergents(coeffs, temp);
    
    for (const auto& p : temp) {
        result.emplace_back(static_cast<int>(p.first), static_cast<int>(p.second));
    }
    
    return result;
}

tuple<int, int> solve_diophantine(int a, int b, int c) {
    // Реализация решения диофантова уравнения
    auto [gcd, x, y] = extendEuclid(abs(a), abs(b));
    
    if (c % gcd != 0) {
        throw runtime_error("Уравнение не имеет решения");
    }

    int multiplier = c / gcd;
    x *= multiplier;
    y *= multiplier;

    if (a < 0) x = -x;
    if (b < 0) y = -y;

    return make_tuple(x, y);
}

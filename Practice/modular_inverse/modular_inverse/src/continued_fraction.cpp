#include "continued_fraction.h"
#include "euclid.h"
#include <vector>
#include <tuple>
#include <cmath>
#include <stdexcept>

using namespace std;

vector<int> continued_fraction(int a, int b) {
    if (b == 0) {
        throw runtime_error("Division by zero");
    }

    vector<int> coefficients;
    a = abs(a);
    b = abs(b);

    while (b != 0) {
        coefficients.push_back(a / b);
        int temp = b;
        b = a % b;
        a = temp;
    }
    return coefficients;
}

vector<pair<int, int>> convergents(const vector<int>& coefficients) {
    vector<pair<int, int>> fractions;
    if (coefficients.empty()) return fractions;

    int p0 = 1, q0 = 0;
    int p1 = coefficients[0], q1 = 1;

    fractions.push_back({p1, q1});

    for (size_t i = 1; i < coefficients.size(); ++i) {
        int p2 = coefficients[i] * p1 + p0;
        int q2 = coefficients[i] * q1 + q0;

        fractions.push_back({p2, q2});

        p0 = p1;
        q0 = q1;
        p1 = p2;
        q1 = q2;
    }

    return fractions;
}

tuple<int, int> solve_diophantine(int a, int b, int c) {
    int g = gcd(a, b);
    if (c % g != 0) {
        throw runtime_error("No integer solutions exist for the given equation");
    }

    a /= g;
    b /= g;
    c /= g;

    int x0, y0;
    tie(x0, y0) = extended_gcd(a, b);

    int x = x0 * c;
    int y = y0 * c;

    return make_tuple(x, y);
}

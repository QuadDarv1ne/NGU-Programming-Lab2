#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Функция уравнения: 2ln(x) - 0.5x + 1
double f(double x) {
    return 2 * log(x) - 0.5 * x + 1;
}

// Производная функции: 2/x - 0.5
double df(double x) {
    return 2 / x - 0.5;
}

// Метод половинного деления
void bisection(double a, double b, double eps) {
    int n = 0;
    cout << "Метод половинного деления:\n";
    cout << "N\t a\t\t b\t\t b - a\n";
    while (fabs(b - a) > eps) {
        double c = (a + b) / 2;
        cout << n << "\t " << fixed << setprecision(6) << a << "\t " << b << "\t " << b - a << endl;
        if (f(c) * f(a) < 0) {
            b = c;
        } else {
            a = c;
        }
        n++;
    }
    cout << "Корень: " << (a + b) / 2 << endl;
}

// Метод Ньютона
void newton(double x0, double eps) {
    int n = 0;
    double x = x0;
    cout << "Метод Ньютона:\n";
    cout << "N\t Xn\t\t Xn+1\t\t Xn+1 - Xn\n";
    while (true) {
        double fx = f(x);
        double dx = df(x);
        double x_next = x - fx / dx;
        cout << n << "\t " << fixed << setprecision(6) << x << "\t " << x_next << "\t " << fabs(x_next - x) << endl;
        if (fabs(x_next - x) < eps) break;
        x = x_next;
        n++;
    }
    cout << "Корень: " << x << endl;
}

// Метод простых итераций
void iteration(double x0, double eps) {
    int n = 0;
    double x = x0;
    cout << "Метод простых итераций:\n";
    cout << "N\t Xn\t\t Xn+1\t\t Xn+1 - Xn\n";
    while (true) {
        double x_next = (2 * log(x) + 1) / 0.5;
        cout << n << "\t " << fixed << setprecision(6) << x << "\t " << x_next << "\t " << fabs(x_next - x) << endl;
        if (fabs(x_next - x) < eps) break;
        x = x_next;
        n++;
    }
    cout << "Корень: " << x << endl;
}

int main() {
    double eps = 1e-4;
    double a = 0.5, b = 1.0; // Интервал для метода половинного деления
    double x0_newton = 0.75;  // Начальное приближение для Ньютона
    double x0_iter = 0.75;    // Начальное приближение для итераций

    bisection(a, b, eps);
    newton(x0_newton, eps);
    iteration(x0_iter, eps);

    return 0;
}

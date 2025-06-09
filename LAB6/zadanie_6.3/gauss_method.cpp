/**
 * @file gauss_method.cpp
 * @brief Решение СЛАУ методом Гаусса с выбором главного элемента
 * 
 * @details
 * 1. Прямой ход с выбором главного элемента
 * 2. Обратный ход для нахождения решения
 * 3. Вывод промежуточных результатов
 */

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void gaussElimination(vector<vector<double>>& A, vector<double>& b) {
    int n = A.size();
    
    // Прямой ход
    for (int k = 0; k < n; ++k) {
        // Выбор главного элемента
        int maxRow = k;
        for (int i = k+1; i < n; ++i)
            if (abs(A[i][k]) > abs(A[maxRow][k]))
                maxRow = i;
        
        swap(A[k], A[maxRow]);
        swap(b[k], b[maxRow]);
        
        // Исключение
        for (int i = k+1; i < n; ++i) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; ++j)
                A[i][j] -= factor * A[k][j];
            b[i] -= factor * b[k];
        }
    }
    
    // Обратный ход
    vector<double> x(n);
    for (int i = n-1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i+1; j < n; ++j)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
    
    // Вывод решения
    cout << "Решение:\n";
    for (int i = 0; i < n; ++i)
        cout << "x" << i+1 << " = " << x[i] << "\n";
}

int main() {
    vector<vector<double>> A = {
        {0.12, -0.34, 0.56},
        {0.89, 0.45, -0.23},
        {0.67, -0.91, 0.34}
    };
    vector<double> b = {0.89, -1.15, 0.56};
    
    gaussElimination(A, b);
    return 0;
}

/**
 * @file task2.cpp
 * @brief Генерация матрицы по шаблону
 * 
 * Программа создает квадратную матрицу по формуле:
 * a[i][j] = 100 + 5*(i + j)
 * 
 * @version 1.0
 * @author Student Name
 */

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

int main() {
    int N;
    cout << "Введите количество строк матрицы: ";
    cin >> N;

    // Создание матрицы по формуле
    vector<vector<int>> matrix(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = 100 + 5 * (i + j);
        }
    }

    // Вывод матрицы
    cout << "\nСгенерированная матрица:\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << setw(4) << val;
        }
        cout << endl;
    }
    
    return 0;
}

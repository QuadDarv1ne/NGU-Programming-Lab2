/**
 * @file task1.cpp
 * @brief Обработка матрицы: реверс строк, поиск экстремумов
 * 
 * Программа выполняет:
 * 1. Генерацию квадратной матрицы N×N случайными числами в диапазоне [-50, 50]
 * 2. Реверс строк матрицы без дополнительного массива
 * 3. Поиск второго минимального и максимального элементов
 * 
 * @version 1.0
 * @author Student Name
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;

int main() {
    srand(time(nullptr));
    int N;
    cout << "Введите размер матрицы N: ";
    cin >> N;

    // Инициализация матрицы случайными числами
    vector<vector<double>> matrix(N, vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (rand() % 10001) / 100.0 - 50.0;
        }
    }

    // Вывод исходной матрицы
    cout << "\nИсходная матрица:\n";
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << fixed << setprecision(2) << setw(8) << val;
        }
        cout << endl;
    }

    // Реверс строк
    reverse(matrix.begin(), matrix.end());
    cout << "\nМатрица после реверса строк:\n";
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << fixed << setprecision(2) << setw(8) << val;
        }
        cout << endl;
    }

    // Поиск экстремумов
    double max_val = numeric_limits<double>::lowest();
    double second_max = numeric_limits<double>::lowest();
    double min_val = numeric_limits<double>::max();
    double second_min = numeric_limits<double>::max();

    for (const auto& row : matrix) {
        for (double val : row) {
            // Обновление максимумов
            if (val > max_val) {
                second_max = max_val;
                max_val = val;
            } else if (val > second_max && val < max_val) {
                second_max = val;
            }
            
            // Обновление минимумов
            if (val < min_val) {
                second_min = min_val;
                min_val = val;
            } else if (val < second_min && val > min_val) {
                second_min = val;
            }
        }
    }

    // Вывод результатов
    cout << "\nРезультаты анализа:\n";
    if (second_max == numeric_limits<double>::lowest()) {
        cout << "Второй максимум не найден\n";
    } else {
        cout << "Второй максимум: " << fixed << setprecision(2) << second_max << endl;
    }
    
    if (second_min == numeric_limits<double>::max()) {
        cout << "Второй минимум не найден\n";
    } else {
        cout << "Второй минимум: " << fixed << setprecision(2) << second_min << endl;
    }

    return 0;
}

/**
 * @file tile_path_width.cpp
 * @brief Вычисление максимальной ширины дорожки по краю площади
 * 
 * @details
 * 1. Вычисляет площадь всей плитки и площадь клумбы
 * 2. Находит максимальную ширину дорожки через бинарный поиск
 * 3. Проверяет возможность выкладки для каждой ширины
 */

#include <iostream>
#include <algorithm>

using namespace std;

long long calculateMaxWidth(long long n, long long m, long long t) {
    long long low = 0, high = min(n, m)/2;
    long long best = 0;
    
    while (low <= high) {
        long long mid = (low + high) / 2;
        long long innerArea = (n - 2*mid) * (m - 2*mid);
        
        if (innerArea >= 0 && n*m - innerArea <= t) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

int main() {
    long long n, m, t;
    cin >> n >> m >> t;
    cout << calculateMaxWidth(n, m, t);
    return 0;
}

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/**
 * @brief Находит минимальное количество переворотов пар монет для преобразования доски в шахматный узор
 * 
 * @param board Двумерный вектор, представляющий доску (0 - решка, 1 - орел)
 * @return int Минимальное количество переворотов или -1, если решение невозможно
 * 
 * @details
 * Функция пытается преобразовать доску в один из двух возможных шахматных узоров:
 * 1. Начинающийся с 0 в левом верхнем углу
 * 2. Начинающийся с 1 в левом верхнем углу
 * 
 * Для каждого узора:
 * 1. Создается временная копия доски
 * 2. Проверяются все ячейки:
 *    - Если значение не соответствует узору, ищется соседняя монета для совместного переворота
 * 3. Если все несоответствия исправлены, запоминается количество переворотов
 * 
 * Возвращается минимальное количество переворотов из двух вариантов.
 */
int minFlipsToChessboard(vector<vector<int>>& board) {
    int rows = board.size();
    int cols = board[0].size();
    int minFlips = INT_MAX;
    
    // Направления для поиска соседей (вверх, вниз, влево, вправо)
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    // Пробуем оба возможных шахматных узора
    for (int pattern = 0; pattern < 2; ++pattern) {
        vector<vector<int>> temp = board; // Рабочая копия доски
        int flips = 0;                    // Счетчик переворотов
        bool possible = true;             // Флаг возможности решения
        
        // Проверяем все ячейки доски
        for (int i = 0; i < rows && possible; ++i) {
            for (int j = 0; j < cols && possible; ++j) {
                // Ожидаемое значение для текущей позиции в шахматном узоре
                int expected = (i + j + pattern) % 2;
                
                // Если текущее значение не соответствует ожидаемому
                if (temp[i][j] != expected) {
                    bool flipped = false; // Флаг успешного переворота
                    
                    // Проверяем всех соседей
                    for (int d = 0; d < 4; ++d) {
                        int ni = i + dx[d]; // Координата соседа по строке
                        int nj = j + dy[d]; // Координата соседа по столбцу
                        
                        // Проверяем, что сосед в пределах доски
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                            // Если сосед тоже не соответствует и их совместный переворот исправит ситуацию
                            if (temp[ni][nj] != expected && 
                                (ni + nj + pattern) % 2 != temp[ni][nj]) {
                                // Переворачиваем обе монеты
                                temp[i][j] ^= 1;
                                temp[ni][nj] ^= 1;
                                flips++;
                                flipped = true;
                                break;
                            }
                        }
                    }
                    // Если не нашли подходящего соседа - решение невозможно
                    if (!flipped) possible = false;
                }
            }
        }
        
        // Если решение возможно и количество переворотов минимально
        if (possible && flips < minFlips) {
            minFlips = flips;
        }
    }
    
    // Возвращаем результат или -1, если решение невозможно
    return (minFlips == INT_MAX) ? -1 : minFlips;
}

int main() {
    /**
     * @brief Основная функция демонстрации работы алгоритма
     * 
     * Функция выполняет:
     * 1. Ввод размеров доски
     * 2. Ввод конфигурации монет
     * 3. Вычисление минимального количества переворотов
     * 4. Вывод результата
     */
    int rows, cols;
    cout << "Введите размеры доски (строки столбцы): ";
    cin >> rows >> cols;
    
    cout << "Введите конфигурацию доски (0 - решка, 1 - орел):\n";
    vector<vector<int>> board(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> board[i][j];
        }
    }
    
    int result = minFlipsToChessboard(board);
    if (result == -1) {
        cout << "Невозможно получить шахматный узор\n";
    } else {
        cout << "Минимальное количество переворотов: " << result << endl;
    }
    
    return 0;
}

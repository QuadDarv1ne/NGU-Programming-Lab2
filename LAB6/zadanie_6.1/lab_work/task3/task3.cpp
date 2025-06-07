/**
 * @file task3.cpp
 * @brief Реализация клеточного автомата Конвея
 * 
 * Программа реализует:
 * 1. Классические правила "Жизни" (выживание 2-3, рождение 3)
 * 2. Пользовательские правила (выживание 2 или 4, рождение 4)
 * 3. Визуализацию в консоли с анимацией
 * 4. Поддержку устойчивых фигур: Block, Beehive, Loaf, Boat
 * 5. Генерацию случайных начальных состояний
 * 
 * @version 1.0
 * @author Student Name
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <string>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

/**
 * @brief Кросс-платформенная очистка экрана
 */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @class CellularAutomaton
 * @brief Класс для реализации клеточного автомата
 * 
 * Реализует функционал клеточного автомата Конвея с поддержкой:
 * - Тороидальных границ
 * - Двух наборов правил (классические и пользовательские)
 * - Визуализации состояния
 */
class CellularAutomaton {
    int rows, cols;
    vector<vector<bool>> grid;
    
    /**
     * @brief Подсчет соседей с тороидальными границами
     * @param x Координата X клетки
     * @param y Координата Y клетки
     * @return Количество живых соседей
     */
    int countNeighbors(int x, int y) const {
        int count = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = (x + dx + rows) % rows;
                int ny = (y + dy + cols) % cols;
                if (grid[nx][ny]) count++;
            }
        }
        return count;
    }

public:
    /**
     * @brief Конструктор класса
     * @param r Количество строк
     * @param c Количество столбцов
     */
    CellularAutomaton(int r, int c) : rows(r), cols(c), grid(r, vector<bool>(c, false)) {}
    
    /**
     * @brief Инициализация предопределенной фигуры
     * @param name Название фигуры (block, beehive, loaf, boat)
     */
    void initFigure(const string& name) {
        int cx = rows/2, cy = cols/2;
        
        if (name == "block") {
            grid[cx][cy] = true;
            grid[cx][cy+1] = true;
            grid[cx+1][cy] = true;
            grid[cx+1][cy+1] = true;
        } 
        else if (name == "beehive") {
            grid[cx-1][cy] = true;
            grid[cx-1][cy+1] = true;
            grid[cx][cy-1] = true;
            grid[cx][cy+2] = true;
            grid[cx+1][cy] = true;
            grid[cx+1][cy+1] = true;
        } 
        else if (name == "loaf") {
            grid[cx-1][cy] = true;
            grid[cx-1][cy+1] = true;
            grid[cx][cy-1] = true;
            grid[cx][cy+2] = true;
            grid[cx+1][cy] = true;
            grid[cx+1][cy+1] = true;
            grid[cx+2][cy+1] = true;
        } 
        else if (name == "boat") {
            grid[cx-1][cy-1] = true;
            grid[cx-1][cy] = true;
            grid[cx][cy-1] = true;
            grid[cx][cy+1] = true;
            grid[cx+1][cy] = true;
        }
    }
    
    /**
     * @brief Случайная инициализация сетки
     * @param probability Вероятность появления живой клетки
     */
    void randomInit(double probability) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = (rand() % 100) < (probability * 100);
            }
        }
    }
    
    /**
     * @brief Расчет следующего поколения
     * @param useCustomRules Флаг использования пользовательских правил
     */
    void nextGeneration(bool useCustomRules) {
        vector<vector<bool>> newGrid = grid;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int neighbors = countNeighbors(i, j);
                
                if (grid[i][j]) {
                    // Правила выживания
                    if (useCustomRules) {
                        newGrid[i][j] = (neighbors == 2 || neighbors == 4);
                    } else {
                        newGrid[i][j] = (neighbors == 2 || neighbors == 3);
                    }
                } else {
                    // Правила рождения
                    if (useCustomRules) {
                        newGrid[i][j] = (neighbors == 4);
                    } else {
                        newGrid[i][j] = (neighbors == 3);
                    }
                }
            }
        }
        
        grid = newGrid;
    }
    
    /**
     * @brief Вывод состояния в консоль
     * @param title Заголовок для отображения
     */
    void display(const string& title) const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Используем Unicode символ для живой клетки
                cout << (grid[i][j] ? "\u25A0" : " ");
            }
            cout << endl;
        }
        cout << "\n" << title << endl;
    }
};

int main() {
    // Поддержка UTF-8 для Windows
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    // Настройка локали
    setlocale(LC_ALL, "en_US.UTF-8");
    
    srand(time(nullptr));
    const int ROWS = 20;
    const int COLS = 40;
    
    CellularAutomaton automaton(ROWS, COLS);
    vector<string> figures = {"block", "beehive", "loaf", "boat"};
    
    // Демонстрация устойчивых фигур с классическими правилами
    for (const auto& figure : figures) {
        automaton = CellularAutomaton(ROWS, COLS);
        automaton.initFigure(figure);
        
        clearScreen();
        automaton.display(figure + " - Исходное состояние (классические правила)");
        this_thread::sleep_for(chrono::seconds(2));
        
        for (int gen = 1; gen <= 5; gen++) {
            automaton.nextGeneration(false);
            clearScreen();
            automaton.display(figure + " - Поколение " + to_string(gen));
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    // Демонстрация со случайным начальным состоянием и пользовательскими правилами
    automaton = CellularAutomaton(ROWS, COLS);
    automaton.randomInit(0.3);
    
    for (int gen = 0; gen < 100; gen++) {
        automaton.nextGeneration(true);
        clearScreen();
        automaton.display("Пользовательские правила - Поколение " + to_string(gen));
        this_thread::sleep_for(chrono::milliseconds(250));
    }
    
    return 0;
}

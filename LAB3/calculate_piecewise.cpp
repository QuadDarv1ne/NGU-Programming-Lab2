/**
 * @file piecewise_function.cpp
 * @brief Вычисление значений кусочно-заданной функции и вывод таблицы.
 * 
 * Программа вычисляет значения функции, заданной графически, на интервале [-4, 8] 
 * с шагом 0.5. Функция имеет следующие ветви:
 * - Не определена при x < -3 и x > 7.
 * - Линейные участки с коэффициентом 1 (45°).
 * - Постоянные значения на отдельных интервалах.
 * 
 * Формат вывода: таблица с колонками x и y. Значения округляются до 0.1.
 * Для неопределенных значений выводится "не определено".
 */

/**
 * doxygen -g
 * doxygen Doxyfile
 */
 
 #include <iostream>
 #include <iomanip>
 #include <string>
 #include <cmath>
 
 using namespace std;
 
 int main() {
     // Параметры интервала и шага
     const double x_start = -4.0; ///< Начало интервала
     const double x_end = 8.0;    ///< Конец интервала
     const double dx = 0.5;       ///< Шаг вычислений
     double x = x_start;          ///< Текущее значение x
 
     // Заголовок таблицы
     cout << "x\t| y" << endl;
     cout << "----------------" << endl;
 
     // Основной цикл вычислений
     while (x <= x_end + 1e-6) { // Учет погрешности для чисел с плавающей точкой
         string y_str; ///< Строковое представление y
 
         // Ветви кусочно-заданной функции
         if (x < -3.0) {
             y_str = "не определено";
         } else if (-3.0 <= x && x < -1.0) {
             double y = x + 3;
             y_str = to_string(round(y * 10) / 10).substr(0, 3); 
         } else if (-1.0 <= x && x < 0.0) {
             y_str = "2";
         } else if (0.0 <= x && x < 1.0) {
             double y = x + 2;
             y_str = to_string(round(y * 10) / 10).substr(0, 3);
         } else if (1.0 <= x && x < 3.0) {
             double y = x + 2;
             y_str = to_string(round(y * 10) / 10).substr(0, 3);
         } else if (3.0 <= x && x < 5.0) {
             y_str = "5";
         } else if (5.0 <= x && x <= 7.0) {
             double y = x;
             y_str = to_string(round(y * 10) / 10).substr(0, 3);
         } else {
             y_str = "не определено";
         }
 
         // Форматированный вывод строки таблицы
         cout << fixed << setprecision(1) << x << "\t| " << y_str << endl;
         x = round((x + dx) * 10) / 10; // Корректное увеличение x
     }
 
     return 0;
 }

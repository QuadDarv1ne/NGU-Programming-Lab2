/**
 * @file task2.go
 * @brief Генерация матрицы по шаблону
 * 
 * Программа создает квадратную матрицу по формуле:
 * a[i][j] = 100 + 5*(i + j)
 * 
 * @version 1.0
 * @author Student Name
 */

package main

import "fmt"

func main() {
	var N int
	fmt.Print("Введите количество строк матрицы: ")
	fmt.Scan(&N)

	// Создание матрицы
	matrix := make([][]int, N)
	for i := range matrix {
		matrix[i] = make([]int, N)
		for j := range matrix[i] {
			matrix[i][j] = 100 + 5*(i+j)
		}
	}

	// Вывод матрицы
	fmt.Println("\nСгенерированная матрица:")
	for _, row := range matrix {
		for _, val := range row {
			fmt.Printf("%4d", val)
		}
		fmt.Println()
	}
}

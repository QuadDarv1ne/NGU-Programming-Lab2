/**
 * @file task1.go
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

package main

import (
	"fmt"
	"math/rand"
	"os/exec"
	"runtime"
	"time"
)

func main() {
	// Поддержка UTF-8 для Windows
	if runtime.GOOS == "windows" {
		exec.Command("cmd", "/c", "chcp 65001").Run()
	}

	rand.Seed(time.Now().UnixNano())
	var N int
	fmt.Print("Введите размер матрицы N: ")
	fmt.Scan(&N)

	// Инициализация матрицы
	matrix := make([][]float64, N)
	for i := range matrix {
		matrix[i] = make([]float64, N)
		for j := range matrix[i] {
			matrix[i][j] = rand.Float64()*100 - 50
		}
	}

	// Вывод исходной матрицы
	fmt.Println("\nИсходная матрица:")
	printMatrix(matrix)

	// Реверс строк
	for i := 0; i < len(matrix)/2; i++ {
		matrix[i], matrix[len(matrix)-1-i] = matrix[len(matrix)-1-i], matrix[i]
	}
	fmt.Println("\nМатрица после реверса строк:")
	printMatrix(matrix)

	// Поиск экстремумов
	const minFloat = -1 << 63
	const maxFloat = 1 << 63 - 1
	maxVal, secondMax := minFloat, minFloat
	minVal, secondMin := maxFloat, maxFloat

	for _, row := range matrix {
		for _, val := range row {
			// Обновление максимумов
			if val > maxVal {
				secondMax = maxVal
				maxVal = val
			} else if val > secondMax && val < maxVal {
				secondMax = val
			}

			// Обновление минимумов
			if val < minVal {
				secondMin = minVal
				minVal = val
			} else if val < secondMin && val > minVal {
				secondMin = val
			}
		}
	}

	// Вывод результатов
	fmt.Println("\nРезультаты анализа:")
	if secondMax == minFloat {
		fmt.Println("Второй максимум не найден")
	} else {
		fmt.Printf("Второй максимум: %.2f\n", secondMax)
	}
	if secondMin == maxFloat {
		fmt.Println("Второй минимум не найден")
	} else {
		fmt.Printf("Второй минимум: %.2f\n", secondMin)
	}
}

// Вспомогательная функция для вывода матрицы
func printMatrix(matrix [][]float64) {
	for _, row := range matrix {
		for _, val := range row {
			fmt.Printf("%8.2f", val)
		}
		fmt.Println()
	}
}

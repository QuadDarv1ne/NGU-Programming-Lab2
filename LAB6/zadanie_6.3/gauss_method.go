	/**
 * Решение СЛАУ методом Гаусса с выбором главного элемента
 *
 * Алгоритм:
 * 1. Прямой ход с выбором главного элемента
 * 2. Обратный ход для нахождения решения
 * 3. Вывод промежуточных результатов
 */
package main

import (
	"fmt"
	"math"
)

func gaussElimination(A [][]float64, b []float64) []float64 {
	n := len(A)
	
	// Прямой ход с выбором главного элемента
	for k := 0; k < n; k++ {
		// Поиск строки с максимальным элементом
		maxRow := k
		for i := k + 1; i < n; i++ {
			if math.Abs(A[i][k]) > math.Abs(A[maxRow][k]) {
				maxRow = i
			}
		}
		
		// Перестановка строк
		A[k], A[maxRow] = A[maxRow], A[k]
		b[k], b[maxRow] = b[maxRow], b[k]
		
		// Исключение
		for i := k + 1; i < n; i++ {
			factor := A[i][k] / A[k][k]
			for j := k; j < n; j++ {
				A[i][j] -= factor * A[k][j]
			}
			b[i] -= factor * b[k]
		}
	}
	
	// Обратный ход
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		x[i] = b[i]
		for j := i + 1; j < n; j++ {
			x[i] -= A[i][j] * x[j]
		}
		x[i] /= A[i][i]
	}
	
	return x
}

func main() {
	A := [][]float64{
		{0.12, -0.34, 0.56},
		{0.89, 0.45, -0.23},
		{0.67, -0.91, 0.34},
	}
	b := []float64{0.89, -1.15, 0.56}
	
	x := gaussElimination(A, b)
	
	fmt.Println("Решение:")
	for i, val := range x {
		fmt.Printf("x%d = %.4f\n", i+1, val)
	}
}

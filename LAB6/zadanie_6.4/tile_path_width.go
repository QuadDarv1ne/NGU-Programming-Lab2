/**
 * Вычисление максимальной ширины дорожки по краю площади
 *
 * Алгоритм:
 * 1. Вычисляет площадь всей плитки и площадь клумбы
 * 2. Находит максимальную ширину дорожки через бинарный поиск
 * 3. Проверяет возможность выкладки для каждой ширины
 */
package main

import (
	"fmt"
)

func calculateMaxWidth(n, m, t int64) int64 {
	var low, high int64 = 0, n
	if m < n {
		high = m
	}
	high /= 2
	
	var best int64 = 0
	
	for low <= high {
		mid := (low + high) / 2
		innerArea := (n - 2*mid) * (m - 2*mid)
		
		if innerArea >= 0 && n*m - innerArea <= t {
			best = mid
			low = mid + 1
		} else {
			high = mid - 1
		}
	}
	return best
}

func main() {
	var n, m, t int64
	fmt.Scan(&n, &m, &t)
	fmt.Println(calculateMaxWidth(n, m, t))
}

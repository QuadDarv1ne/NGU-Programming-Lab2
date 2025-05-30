/**
 * @file task3.go
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

package main

import (
	"fmt"
	"math/rand"
	"os"
	"os/exec"
	"runtime"
	"strconv"
	"time"
)

/**
 * @brief Кросс-платформенная очистка экрана
 */
func clearScreen() {
	var cmd *exec.Cmd
	if runtime.GOOS == "windows" {
		cmd = exec.Command("cmd", "/c", "cls")
	} else {
		cmd = exec.Command("clear")
	}
	cmd.Stdout = os.Stdout
	cmd.Run()
}

/**
 * @struct CellularAutomaton
 * @brief Структура для реализации клеточного автомата
 * 
 * Содержит функционал для:
 * - Тороидальных границ
 * - Двух наборов правил
 * - Визуализации состояния
 */
type CellularAutomaton struct {
	rows, cols int
	grid       [][]bool
}

/**
 * @brief Конструктор нового автомата
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @return Указатель на созданный автомат
 */
func NewAutomaton(rows, cols int) *CellularAutomaton {
	grid := make([][]bool, rows)
	for i := range grid {
		grid[i] = make([]bool, cols)
	}
	return &CellularAutomaton{rows: rows, cols: cols, grid: grid}
}

/**
 * @brief Подсчет соседей с тороидальными границами
 * @param x Координата X клетки
 * @param y Координата Y клетки
 * @return Количество живых соседей
 */
func (ca *CellularAutomaton) countNeighbors(x, y int) int {
	count := 0
	for dx := -1; dx <= 1; dx++ {
		for dy := -1; dy <= 1; dy++ {
			if dx == 0 && dy == 0 {
				continue
			}
			nx := (x + dx + ca.rows) % ca.rows
			ny := (y + dy + ca.cols) % ca.cols
			if ca.grid[nx][ny] {
				count++
			}
		}
	}
	return count
}

/**
 * @brief Инициализация предопределенной фигуры
 * @param name Название фигуры (block, beehive, loaf, boat)
 */
func (ca *CellularAutomaton) initFigure(name string) {
	cx, cy := ca.rows/2, ca.cols/2
	switch name {
	case "block":
		ca.grid[cx][cy] = true
		ca.grid[cx][cy+1] = true
		ca.grid[cx+1][cy] = true
		ca.grid[cx+1][cy+1] = true
	case "beehive":
		ca.grid[cx-1][cy] = true
		ca.grid[cx-1][cy+1] = true
		ca.grid[cx][cy-1] = true
		ca.grid[cx][cy+2] = true
		ca.grid[cx+1][cy] = true
		ca.grid[cx+1][cy+1] = true
	case "loaf":
		ca.grid[cx-1][cy] = true
		ca.grid[cx-1][cy+1] = true
		ca.grid[cx][cy-1] = true
		ca.grid[cx][cy+2] = true
		ca.grid[cx+1][cy] = true
		ca.grid[cx+1][cy+1] = true
		ca.grid[cx+2][cy+1] = true
	case "boat":
		ca.grid[cx-1][cy-1] = true
		ca.grid[cx-1][cy] = true
		ca.grid[cx][cy-1] = true
		ca.grid[cx][cy+1] = true
		ca.grid[cx+1][cy] = true
	}
}

/**
 * @brief Случайная инициализация сетки
 * @param probability Вероятность появления живой клетки
 */
func (ca *CellularAutomaton) randomInit(probability float64) {
	for i := range ca.grid {
		for j := range ca.grid[i] {
			ca.grid[i][j] = rand.Float64() < probability
		}
	}
}

/**
 * @brief Расчет следующего поколения
 * @param customRules Флаг использования пользовательских правил
 */
func (ca *CellularAutomaton) nextGeneration(customRules bool) {
	newGrid := make([][]bool, ca.rows)
	for i := range newGrid {
		newGrid[i] = make([]bool, ca.cols)
		copy(newGrid[i], ca.grid[i])
	}

	for i := 0; i < ca.rows; i++ {
		for j := 0; j < ca.cols; j++ {
			neighbors := ca.countNeighbors(i, j)
			alive := ca.grid[i][j]

			if alive {
				if customRules {
					newGrid[i][j] = neighbors == 2 || neighbors == 4
				} else {
					newGrid[i][j] = neighbors == 2 || neighbors == 3
				}
			} else {
				if customRules {
					newGrid[i][j] = neighbors == 4
				} else {
					newGrid[i][j] = neighbors == 3
				}
			}
		}
	}
	ca.grid = newGrid
}

/**
 * @brief Вывод состояния в консоль
 * @param title Заголовок для отображения
 */
func (ca *CellularAutomaton) display(title string) {
	for _, row := range ca.grid {
		for _, cell := range row {
			if cell {
				fmt.Print("■")
			} else {
				fmt.Print(" ")
			}
		}
		fmt.Println()
	}
	fmt.Printf("\n%s\n", title)
}

func main() {
	// Поддержка UTF-8 для Windows
    if runtime.GOOS == "windows" {
        exec.Command("cmd", "/c", "chcp 65001").Run()
    }
	
	rand.Seed(time.Now().UnixNano())
	const ROWS = 20
	const COLS = 40

	figures := []string{"block", "beehive", "loaf", "boat"}

	// Демонстрация устойчивых фигур
	for _, figure := range figures {
		automaton := NewAutomaton(ROWS, COLS)
		automaton.initFigure(figure)

		clearScreen()
		automaton.display(figure + " - Исходное состояние (классические правила)")
		time.Sleep(2 * time.Second)

		for gen := 1; gen <= 5; gen++ {
			automaton.nextGeneration(false)
			clearScreen()
			automaton.display(figure + " - Поколение " + strconv.Itoa(gen))
			time.Sleep(500 * time.Millisecond)
		}
		time.Sleep(time.Second)
	}

	// Демонстрация случайной колонии с пользовательскими правилами
	automaton := NewAutomaton(ROWS, COLS)
	automaton.randomInit(0.3)

	for gen := 0; gen < 100; gen++ {
		automaton.nextGeneration(true)
		clearScreen()
		automaton.display("Пользовательские правила - Поколение " + strconv.Itoa(gen))
		time.Sleep(250 * time.Millisecond)
	}
}

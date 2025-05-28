/**
 * Программа "Электронная очередь в поликлинике"
 *
 * Обрабатывает команды:
 *   ENQUEUE <время> - добавить посетителя в очередь
 *   DISTRIBUTE      - распределить очередь по окнам
 *
 * Алгоритм:
 *   1. Посетители сортируются по убыванию времени обслуживания
 *   2. Каждый посетитель направляется в наименее загруженное окно
 *   3. Внутри окна посетители сортируются по порядку добавления
 *
 * Формат талона: TXXX (XXX - номер с ведущими нулями для номеров <1000)
 */

package main

import (
	"fmt"
	"sort"
	"strconv"
)

type Visitor struct {
	ticket string
	time   int
	order  int
}

type ByTimeDesc []Visitor

func (a ByTimeDesc) Len() int      { return len(a) }
func (a ByTimeDesc) Swap(i, j int) { a[i], a[j] = a[j], a[i] }
func (a ByTimeDesc) Less(i, j int) bool {
	if a[i].time == a[j].time {
		return a[i].order < a[j].order
	}
	return a[i].time > a[j].time
}

type ByOrder []Visitor

func (a ByOrder) Len() int           { return len(a) }
func (a ByOrder) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a ByOrder) Less(i, j int) bool { return a[i].order < a[j].order }

func main() {
	var windowsCount int
	fmt.Println(">>> Введите кол-во окон")
	fmt.Scan(&windowsCount)

	visitors := []Visitor{}
	orderCounter := 0

	for {
		var command string
		fmt.Scan(&command)
		if command == "ENQUEUE" {
			var time int
			fmt.Scan(&time)
			ticket := "T"
			if orderCounter < 1000 {
				ticket += fmt.Sprintf("%03d", orderCounter)
			} else {
				ticket += strconv.Itoa(orderCounter)
			}
			visitors = append(visitors, Visitor{ticket, time, orderCounter})
			orderCounter++
			fmt.Println(">>>", ticket)
		} else if command == "DISTRIBUTE" {
			break
		}
	}

	windows := make([][]Visitor, windowsCount)
	windowTime := make([]int, windowsCount)

	sort.Sort(ByTimeDesc(visitors))

	for _, v := range visitors {
		minIndex := 0
		minTime := windowTime[0]
		for i := 1; i < windowsCount; i++ {
			if windowTime[i] < minTime {
				minTime = windowTime[i]
				minIndex = i
			}
		}
		windows[minIndex] = append(windows[minIndex], v)
		windowTime[minIndex] += v.time
	}

	for i := 0; i < windowsCount; i++ {
		sort.Sort(ByOrder(windows[i]))
	}

	fmt.Print(">>> ")
	for i := 0; i < windowsCount; i++ {
		fmt.Printf("Окно %d (%d минут): ", i+1, windowTime[i])
		for j, v := range windows[i] {
			fmt.Print(v.ticket)
			if j < len(windows[i])-1 {
				fmt.Print(", ")
			}
		}
		if i < windowsCount-1 {
			fmt.Print(" >>> ")
		}
	}
	fmt.Println()
}

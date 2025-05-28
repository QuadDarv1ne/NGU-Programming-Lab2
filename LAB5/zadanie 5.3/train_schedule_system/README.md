# 🚂 Train Schedule System

## 📝 Описание проекта

**Программа для управления графиком движения поездов с возможностью:**

- Создания и обновления маршрутов
- Поиска поездов по городам
- Просмотра городов в маршруте поезда
- Вывода полного расписания движения

## 🛠️ Сборка проекта

### Требования

- Компилятор `C++` с поддержкой `C++11`
- Система сборки `Make` **(опционально)**

### Инструкция по сборке

```bash
# Создать директорию для объектных файлов
mkdir -p bin

# Компиляция компонентов
g++ -c src/commands.cpp -Iinclude -o bin/commands.o
g++ src/main.cpp bin/commands.o -Iinclude -o bin/train_schedule

# Запуск программы
./bin/train_schedule
```

## ⌨️ Доступные команды

| Команда | Формат | Описание |
|---------|--------|----------|
| `CREATE_TRAIN` | `CREATE_TRAIN <поезд> <город1> <город2> ...` | Создает новый маршрут поезда |
| `TRAINS_FOR_TOWN` | `TRAINS_FOR_TOWN <город>` | Показывает все поезда, проходящие через город |
| `TOWNS_FOR_TRAIN` | `TOWNS_FOR_TRAIN <поезд>` | Показывает все города маршрута с другими поездами |
| `TRAINS` | `TRAINS` | Выводит полное расписание всех поездов |

## 💻 Пример использования

```bash
CREATE_TRAIN Express Moscow St.Petersburg Novgorod
CREATE_TRAIN Sapsan St.Petersburg Moscow
TRAINS_FOR_TOWN Moscow
TOWNS_FOR_TRAIN Express
TRAINS
```

### Пример вывода

```bash
Express Sapsan
Stop Moscow: Sapsan
Stop St.Petersburg: Sapsan
Stop Novgorod: 0
TRAIN Express: Moscow St.Petersburg Novgorod
TRAIN Sapsan: St.Petersburg Moscow
```

## 📂 Структура проекта

```textline
train_schedule_system/
├── bin/                   # Скомпилированные файлы
├── include/               # Заголовочные файлы
│   └── commands.h         # Объявления функций и структур
├── src/                   # Исходный код
│   ├── commands.cpp       # Реализация логики команд
│   └── main.cpp           # Основная программа
└── README.md              # Документация
```  

## ⚙️ Технические особенности

- Использование `STL-контейнеров` (`map`, `vector`, `set`)
- Динамическое обновление маршрутов
- Обработка ошибок ввода
- Эффективные алгоритмы поиска
- Многофайловая архитектура

---

- `Преподаватель`: **Дуплей Максим Игоревич**
- `Telegram`: [@quadd4rv1n7](https://t.me/quadd4rv1n7)
- `Дата разработки`: 28.05.2025

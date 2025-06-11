cmake_minimum_required(VERSION 3.10)
project(DiophantineSolver)

# Настройка выходной директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Подключение заголовочных файлов
include_directories(include)

# Создание исполняемого файла
add_executable(solve_diophantine
    src/diophantine.cpp
    src/fraction.cpp
    src/main.cpp
)

# Для Windows: настройка консольного приложения
if(WIN32)
    set_target_properties(solve_diophantine PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
cmake_minimum_required(VERSION 3.10)
project(ModularInverseCalculator)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем поддиректории
include_directories(include)

# Сборка исполняемого файла
add_executable(modular_inverse
    src/euclid.cpp
    src/modular_inverse.cpp
    src/main.cpp
)
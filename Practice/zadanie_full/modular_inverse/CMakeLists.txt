cmake_minimum_required(VERSION 3.10)
project(ModularInverseCalculator)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем поддиректории
include_directories(include)

# Поиск OpenSSL для RSA
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Сборка исполняемого файла
add_executable(modular_inverse
    src/euclid.cpp
    src/fermat.cpp
    src/modular_inverse.cpp
    src/rsa.cpp
    src/menu.cpp
    src/main.cpp
)

# Линковка с OpenSSL
target_link_libraries(modular_inverse ${OPENSSL_LIBRARIES})
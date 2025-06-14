cmake_minimum_required(VERSION 3.10)
project(ModularInverseCalculator)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск OpenSSL для RSA
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Подключение заголовочных файлов
include_directories(include)

# Настройка выходной директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Сборка основного исполняемого файла
add_executable(modular_inverse
    src/main.cpp
    src/menu.cpp
    src/euclid.cpp
    src/fermat.cpp
    src/modular_inverse.cpp
    src/rsa.cpp
    src/continued_fraction.cpp
)

# Линковка с OpenSSL
target_link_libraries(modular_inverse ${OPENSSL_LIBRARIES})

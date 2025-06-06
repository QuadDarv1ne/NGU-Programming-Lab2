cmake_minimum_required(VERSION 3.12)
project(AES_CFB_Demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Добавляем исполняемый файл
add_executable(aes_cfb_demo
    src/main.cpp
    src/aes_cfb.cpp
)

# Подключаем OpenSSL
target_link_libraries(aes_cfb_demo ${OPENSSL_LIBRARIES})

# Устанавлием выходной каталог
set_target_properties(aes_cfb_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Установка для тестирования
include(CTest)
add_test(NAME AES_CFB_Demo COMMAND bin/aes_cfb_demo)

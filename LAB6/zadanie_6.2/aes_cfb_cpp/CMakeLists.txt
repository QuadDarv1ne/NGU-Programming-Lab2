cmake_minimum_required(VERSION 3.12)
project(aes_cfb_demo LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройка политик CMake
if(POLICY CMP0075)
    cmake_policy(SET CMP0075 NEW)
endif()

# Поиск OpenSSL
find_package(OpenSSL REQUIRED)

# Настройка выходных директорий
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Добавление исполняемого файла
add_executable(aes_cfb_demo
    src/aes_cfb.cpp
    src/main.cpp
)

# Настройка включения заголовков
target_include_directories(aes_cfb_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(aes_cfb_demo PRIVATE ${OPENSSL_INCLUDE_DIR})

# Линковка с OpenSSL
target_link_libraries(aes_cfb_demo PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Установка для пользователя
install(TARGETS aes_cfb_demo
    RUNTIME DESTINATION bin
)

# Опции сборки
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_test(NAME aes_cfb_demo_test COMMAND aes_cfb_demo)
endif()

# Информационное сообщение
message(STATUS "Конфигурация проекта завершена успешно")
message(STATUS "Исполняемый файл будет создан в: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

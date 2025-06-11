cmake_minimum_required(VERSION 3.10)
project(FermatModularExponentiation)

# Настройка выходной директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Подключение заголовочных файлов
include_directories(include)

# Создание исполняемого файла
add_executable(mod_exp
    src/fermat_exponentiation.cpp
    src/prime_check.cpp
    src/main.cpp
)

# Для Windows: настройка консольного приложения
if(WIN32)
    set_target_properties(mod_exp PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
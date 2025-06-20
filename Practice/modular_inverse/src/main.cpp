#include "menu.h"
#include "set_encoding.h"
#include <clocale>

int main() {
    setConsoleEncoding();
    
    // Установка локали для поддержки русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Настройка кодировки консоли Windows
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    run_main_menu();
    return 0;
}
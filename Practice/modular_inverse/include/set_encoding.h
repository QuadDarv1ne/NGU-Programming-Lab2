#ifndef SET_ENCODING_H
#define SET_ENCODING_H

#include <clocale>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

static void setConsoleEncoding() {
    #ifdef _WIN32
    // Настройки для Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #else
    // Настройки для Linux/WSL/MacOS
    try {
        // Попробуем разные варианты локалей
        const char* locales[] = {
            "C.UTF-8", 
            "en_US.UTF-8", 
            "ru_RU.UTF-8",
            ""
        };
        
        for (const char* loc : locales) {
            if (std::setlocale(LC_ALL, loc) != nullptr) {
                break;
            }
        }
    } catch (...) {
        // Игнорируем ошибки, если не удалось установить локаль
    }
    #endif
    
    // Настройки потоков
    std::ios_base::sync_with_stdio(false);
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::wcin.imbue(std::locale());
}

#endif // SET_ENCODING_H
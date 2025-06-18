#ifndef SET_ENCODING_H
#define SET_ENCODING_H

#include <clocale>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

static void setConsoleEncoding() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Перенаправление потоков для корректной работы UTF-8
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    std::wcin.imbue(std::locale("ru_RU.UTF-8"));
}

#endif
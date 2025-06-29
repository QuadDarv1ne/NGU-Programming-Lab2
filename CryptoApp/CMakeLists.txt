cmake_minimum_required(VERSION 3.10)
project(CryptoApp VERSION 1.0 LANGUAGES CXX)

# Принудительная проверка платформы
if(NOT WIN32 AND NOT UNIX)
    message(FATAL_ERROR "Поддерживаются только Windows и Linux платформы")
endif()

# Общие настройки компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Для инструментов анализа кода

# Настройки для Windows
if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Настройки для Linux
if(UNIX)
    add_definitions(-DLINUX_PLATFORM)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Общие настройки выходных директорий
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Создание динамических библиотек
add_library(Cast128 SHARED src/cast128/cast128.cpp)
add_library(RSA_SHARED SHARED src/rsa/rsa.cpp)
add_library(LUC_SHARED SHARED src/luc/luc.cpp)

# Установка версий для библиотек
set_target_properties(Cast128 PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(RSA_SHARED PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(LUC_SHARED PROPERTIES VERSION ${PROJECT_VERSION})

# Основное приложение
add_executable(CryptoApp src/main.cpp)
target_link_libraries(CryptoApp Cast128 RSA_SHARED LUC_SHARED)

# Установка
install(TARGETS CryptoApp 
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

install(TARGETS Cast128 RSA_SHARED LUC_SHARED
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Настройка CPack
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "Криптографическое приложение с поддержкой CAST-128, RSA и LUC")
set(CPACK_PACKAGE_VENDOR "YourCompany")
set(CPACK_PACKAGE_CONTACT "support@yourcompany.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# Для Linux (DEB и RPM)
if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.28), libstdc++6 (>= 9.0)")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_RPM_PACKAGE_REQUIRES "libstdc++ >= 9.0, glibc >= 2.28")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
endif()

# Для Windows
if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MENU_LINKS "bin/CryptoApp" "CryptoApp")
endif()

# Цель uninstall
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/uninstall.cmake"
    @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/uninstall.cmake
    COMMENT "Uninstalling CryptoApp"
)

include(CPack)

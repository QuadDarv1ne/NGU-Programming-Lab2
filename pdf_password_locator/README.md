vcpkg — это менеджер пакетов для C++, который упрощает установку библиотек.

1. Предварительные требования
a) Git
b) Visual Studio (с компонентами "Разработка настольных приложений на C++")
c) CMake (версия 3.15+)


Установка vcpkg
Откройте командную строку (cmd) или PowerShell

Клонируйте репозиторий vcpkg:

cmd
git clone https://github.com/microsoft/vcpkg
Перейдите в папку vcpkg:

cmd
cd vcpkg
Запустите скрипт начальной настройки:

Для PowerShell:

ps1
.\bootstrap-vcpkg.bat
Для CMD:

cmd
bootstrap-vcpkg.bat


```bash
# Установка зависимостей для Poppler
.\vcpkg install freetype zlib libpng libjpeg-turbo tiff openjpeg --triplet x64-windows

# Установка Poppler с поддержкой GLIB (опционально)
.\vcpkg install poppler[glib] --triplet x64-windows
```

Полезные команды vcpkg
Команда	Описание
vcpkg search poppler	Поиск пакета
vcpkg list	Установленные пакеты
vcpkg export poppler --zip	Экспорт библиотеки
vcpkg remove poppler	Удаление пакета

git pull
.\bootstrap-vcpkg.bat
.\vcpkg update
.\vcpkg upgrade --no-dry-run


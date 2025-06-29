# CryptoApp

`CryptoApp` — это приложение для шифрования и дешифрования данных с использованием различных алгоритмов, таких как `CAST-128`, `RSA` и `LUC`

## Структура проекта

```textline
CryptoApp/
├── CMakeLists.txt
├── LICENSE.txt
├── README.md
├── docs/
│   └── BUILD_INSTRUCTIONS.md
└── src/
    ├── main.cpp
    ├── cast128/
    │   ├── cast128.cpp
    │   └── cast128.h
    ├── rsa/
    │   ├── rsa.cpp
    │   └── rsa.h
    └── luc/
        ├── luc.cpp
        └── luc.h
```

## Сборка

Инструкции по сборке проекта можно найти в [документации](docs/BUILD_INSTRUCTIONS.md).

## Использование

**После успешной сборки вы можете запустить приложение:**

```bash
./CryptoApp
```

## Установка

```bash
sudo make install
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
CryptoApp
```

Или для `Windows` с использованием `CMake` и установщика `NSIS`/`WiX`

## Создание пакета (DEB/NSIS)

```bash
cpack -G DEB  # Для создания .deb пакета
# ИЛИ
cpack -G RPM  # Для создания .rpm пакета
```

```bash
sudo dpkg -i CryptoApp-1.0.0-Linux.deb
```

## Удаление

```bash
sudo rm -f /usr/local/bin/CryptoApp
sudo rm -f /usr/local/lib/libCast128.so
sudo rm -f /usr/local/lib/libRSA_SHARED.so
sudo rm -f /usr/local/lib/libLUC_SHARED.so
sudo ldconfig
```

## Лицензия

Этот проект лицензирован в соответствии с условиями `MIT License`

***Инструкция по сборке:**

```bash
# Для Ubuntu/Debian
sudo apt-get install build-essential cmake libssl-dev

# Для Windows (используя vcpkg)
vcpkg install openssl

# Создаем директорию для сборки
mkdir build
cd build

# Генерируем Makefile
cmake ..

# Собираем проект
make

# Запускаем программу
./modular_inverse
```

```bash
g++ -Iinclude -std=c++17 -o modular_inverse src/euclid.cpp src/modular_inverse.cpp src/main.cpp

# Запускаем программу
.\modular_inverse
```

#!/bin/bash

echo "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄"
echo "█        Сборка проекта AES-128 CFB        █"
echo "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀"
echo

# Сборка C++ версии
echo "Сборка C++ версии..."
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
if [ $? -eq 0 ]; then
    echo "C++ версия успешно собрана: build/bin/aes_cfb_demo"
else
    echo "Ошибка сборки C++ версии!"
    exit 1
fi
cd ..

# Сборка Go версии
echo -e "\nСборка Go версии..."
go build -o aes_cfb_go main.go
if [ $? -eq 0 ]; then
    echo "Go версия успешно собрана: aes_cfb_go"
else
    echo "Ошибка сборки Go версии!"
    exit 1
fi

echo -e "\nСборка завершена успешно!"

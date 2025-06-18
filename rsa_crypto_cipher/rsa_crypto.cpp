/**
 * Программа для шифрования/дешифрования данных с использованием алгоритма RSA.
 * 
 * Реализует:
 * 1. Генерацию ключей RSA (открытый/закрытый)
 * 2. Шифрование и дешифрование текстовых сообщений
 * 3. Шифрование и дешифрование бинарных файлов
 * 
 * Основные компоненты:
 * - Тест Миллера-Рабина для проверки простых чисел
 * - Модульные операции для работы с большими числами
 * - Генерация простых чисел в заданном диапазоне
 * - Вычисление модульной инверсии (расширенный алгоритм Евклида)
 * 
 * Функционал:
 * 1. mulmod() - Безопасное умножение по модулю
 * 2. powmod() - Возведение в степень по модулю
 * 3. isPrime() - Проверка числа на простоту
 * 4. generatePrime() - Генерация простого числа
 * 5. generateRSAKeys() - Создание ключей RSA
 * 6. encryptMessage()/decryptMessage() - Работа с текстом
 * 7. encryptFile()/decryptFile() - Работа с файлами
 * 
 * Особенности:
 * - Использование длинной арифметики для предотвращения переполнений
 * - Побайтовое шифрование данных
 * - Автоматическая генерация ключей
 * - Консольный интерфейс с меню выбора операций
 * 
 * Ограничения:
 * - Работает с числами до 20 разрядов (int64_t)
 * - Шифрование файлов выполняется побайтово (не для больших файлов)
 * 
 * Использование:
 * Программа предлагает интерактивное меню для выбора операций:
 * 1. Шифрование текста
 * 2. Дешифрование текста
 * 3. Шифрование файла
 * 4. Дешифрование файла
 * 5. Генерация новых ключей
 * 
 * Компиляция с помощью g++:
 * g++ -std=c++17 rsa_crypto.cpp -o rsa_crypto
 * 
 * Для оптимизации и предупреждений можно использовать:
 * g++ -std=c++11 -Wall -Wextra -Wpedantic -O2 -o rsa_crypto rsa_crypto.cpp
 * 
 * Пример вызова:
 * ./rsa_crypto
 * 
 * Требования:
 * - Компилятор C++ с поддержкой C++11, C++14 или C++17
 * - Стандартные библиотеки: iostream, fstream, vector, string, random, cstdint, cctype
 */

 /**
  * make       # Компиляция
  * make run   # Компиляция и запуск
  * make clean # Удаление исполняемого файла
  */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>

using namespace std;

// Функция для модульного умножения (a * b) % mod, избегая переполнения
int64_t mulmod(int64_t a, int64_t b, int64_t mod) {
    int64_t res = 0;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            res = (res + a) % mod;
        a = (a * 2) % mod;
        b = b / 2;
    }
    return res % mod;
}

// Функция для возведения в степень по модулю (a^b) % mod
int64_t powmod(int64_t a, int64_t b, int64_t mod) {
    int64_t res = 1;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            res = mulmod(res, a, mod);
        a = mulmod(a, a, mod);
        b = b >> 1;
    }
    return res;
}

// Тест Миллера-Рабина для проверки простоты
bool millerTest(int64_t d, int64_t n) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int64_t> dist(2, n - 2);
    
    int64_t a = dist(gen);
    int64_t x = powmod(a, d, n);
    
    if (x == 1 || x == n - 1)
        return true;
    
    while (d != n - 1) {
        x = mulmod(x, x, n);
        d *= 2;
        
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    
    return false;
}

// Функция для проверки, является ли число простым (используя тест Миллера-Рабина)
bool isPrime(int64_t n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;
    
    int64_t d = n - 1;
    while (d % 2 == 0)
        d /= 2;
    
    for (int i = 0; i < k; i++)
        if (!millerTest(d, n))
            return false;
    
    return true;
}

// Функция для генерации простого числа в заданном диапазоне
int64_t generatePrime(int64_t min, int64_t max) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int64_t> dist(min, max);
    
    int64_t num;
    do {
        num = dist(gen);
        if (num % 2 == 0 && num != 2) 
            num++;
    } while (!isPrime(num));
    
    return num;
}

// Функция для вычисления НОД (алгоритм Евклида)
int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Функция для вычисления обратного модуля (расширенный алгоритм Евклида)
int64_t modInverse(int64_t a, int64_t m) {
    int64_t m0 = m;
    int64_t y = 0, x = 1;
    
    if (m == 1)
        return 0;
    
    while (a > 1) {
        int64_t q = a / m;
        int64_t t = m;
        
        m = a % m;
        a = t;
        t = y;
        
        y = x - q * y;
        x = t;
    }
    
    if (x < 0)
        x += m0;
    
    return x;
}

// Структура для хранения ключей
struct RSAKeys {
    int64_t publicKey;
    int64_t privateKey;
    int64_t n;
};

// Генерация ключей RSA
RSAKeys generateRSAKeys() {
    RSAKeys keys;
    
    // Генерируем два простых числа
    int64_t p = generatePrime(1000, 10000);
    int64_t q = generatePrime(1000, 10000);
    
    // Убедимся, что p и q разные
    while (p == q) {
        q = generatePrime(1000, 10000);
    }
    
    // Вычисляем модуль
    keys.n = p * q;
    
    // Вычисляем функцию Эйлера
    int64_t phi = (p - 1) * (q - 1);
    
    // Выбираем открытую экспоненту (обычно 65537)
    int64_t e = 65537;
    while (gcd(e, phi) != 1) {
        e++;
    }
    keys.publicKey = e;
    
    // Вычисляем секретную экспоненту
    keys.privateKey = modInverse(e, phi);
    
    return keys;
}

// Шифрование сообщения
vector<int64_t> encryptMessage(const string& message, int64_t e, int64_t n) {
    vector<int64_t> encrypted;
    
    for (char c : message) {
        int64_t m = static_cast<int64_t>(static_cast<unsigned char>(c));
        encrypted.push_back(powmod(m, e, n));
    }
    
    return encrypted;
}

// Дешифрование сообщения
string decryptMessage(const vector<int64_t>& encrypted, int64_t d, int64_t n) {
    string decrypted;
    
    for (int64_t num : encrypted) {
        int64_t m = powmod(num, d, n);
        decrypted += static_cast<char>(m);
    }
    
    return decrypted;
}

// Шифрование файла
void encryptFile(const string& inputFile, const string& outputFile, int64_t e, int64_t n) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        throw runtime_error("Не удалось открыть входной файл: " + inputFile);
    }
    
    ofstream out(outputFile);
    if (!out) {
        throw runtime_error("Не удалось создать выходной файл: " + outputFile);
    }
    
    char c;
    while (in.get(c)) {
        int64_t m = static_cast<int64_t>(static_cast<unsigned char>(c));
        int64_t encrypted = powmod(m, e, n);
        out << encrypted << " ";
    }
    
    in.close();
    out.close();
}

// Дешифрование файла
void decryptFile(const string& inputFile, const string& outputFile, int64_t d, int64_t n) {
    ifstream in(inputFile);
    if (!in) {
        throw runtime_error("Не удалось открыть входной файл: " + inputFile);
    }
    
    ofstream out(outputFile, ios::binary);
    if (!out) {
        throw runtime_error("Не удалось создать выходной файл: " + outputFile);
    }
    
    int64_t num;
    while (in >> num) {
        int64_t decrypted = powmod(num, d, n);
        out << static_cast<char>(decrypted);
    }
    
    in.close();
    out.close();
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "RSA Шифрование/Дешифрование\n";
    cout << "1. Шифровать сообщение\n";
    cout << "2. Дешифровать сообщение\n";
    cout << "3. Шифровать файл\n";
    cout << "4. Дешифровать файл\n";
    cout << "5. Сгенерировать новые ключи\n";
    cout << "Выберите действие: ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Очистка буфера
    
    RSAKeys keys = generateRSAKeys();
    
    if (choice == 5) {
        cout << "Сгенерированы новые ключи:\n";
        cout << "Открытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
        cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n";
        return 0;
    }
    
    // Для других действий автоматически используем сгенерированные ключи
    cout << "\nИспользуются автоматически сгенерированные ключи:\n";
    cout << "Открытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
    cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n\n";
    
    try {
        switch (choice) {
            case 1: {
                cout << "Введите сообщение для шифрования: ";
                string message;
                getline(cin, message);
                
                vector<int64_t> encrypted = encryptMessage(message, keys.publicKey, keys.n);
                cout << "Зашифрованное сообщение: ";
                for (int64_t num : encrypted) {
                    cout << num << " ";
                }
                cout << endl;
                break;
            }
            case 2: {
                cout << "Введите зашифрованное сообщение (числа через пробел): ";
                string encryptedStr;
                getline(cin, encryptedStr);
                
                vector<int64_t> encrypted;
                size_t pos = 0;
                while (pos < encryptedStr.length()) {
                    size_t spacePos = encryptedStr.find(' ', pos);
                    if (spacePos == string::npos) spacePos = encryptedStr.length();
                    
                    string numStr = encryptedStr.substr(pos, spacePos - pos);
                    if (!numStr.empty()) {
                        encrypted.push_back(stoll(numStr));
                    }
                    pos = spacePos + 1;
                }
                
                string decrypted = decryptMessage(encrypted, keys.privateKey, keys.n);
                cout << "Расшифрованное сообщение: " << decrypted << endl;
                break;
            }
            case 3: {
                cout << "Введите имя файла для шифрования: ";
                string inputFile;
                getline(cin, inputFile);
                
                cout << "Введите имя выходного файла: ";
                string outputFile;
                getline(cin, outputFile);
                
                encryptFile(inputFile, outputFile, keys.publicKey, keys.n);
                cout << "Файл успешно зашифрован." << endl;
                break;
            }
            case 4: {
                cout << "Введите имя файла для дешифрования: ";
                string inputFile;
                getline(cin, inputFile);
                
                cout << "Введите имя выходного файла: ";
                string outputFile;
                getline(cin, outputFile);
                
                decryptFile(inputFile, outputFile, keys.privateKey, keys.n);
                cout << "Файл успешно расшифрован." << endl;
                break;
            }
            default:
                cout << "Неверный выбор." << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}

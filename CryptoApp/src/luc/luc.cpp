#include "luc.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <tuple>

using namespace std;

// Вспомогательные функции

int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int64_t lcm(int64_t a, int64_t b) {
    return (a / gcd(a, b)) * b;
}

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

int64_t powmod(int64_t a, int64_t b, int64_t mod) {
    int64_t res = 1;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            res = (res * a) % mod;
        a = (a * a) % mod;
        b = b >> 1;
    }
    return res;
}

int legendreSymbol(int64_t a, int64_t p) {
    int64_t ls = powmod(a, (p - 1) / 2, p);
    return ls == p - 1 ? -1 : ls;
}

bool isPrime(int64_t n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    int64_t d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int64_t> dist(2, n - 2);

    for (int i = 0; i < k; i++) {
        int64_t a = dist(gen);
        int64_t x = powmod(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;

            if (x == 1) return false;
            if (x == n - 1) break;
        }

        if (x != n - 1) return false;
    }

    return true;
}

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

// Основные функции LUC

int64_t lucasSequence(int64_t k, int64_t P, int64_t Q, int64_t n) {
    if (n == 1) return 0;

    int64_t V_prev = 2 % n;  // V_0
    int64_t V_curr = P % n;  // V_1

    for (int64_t i = 2; i <= k; ++i) {
        int64_t V_next = (P * V_curr - Q * V_prev) % n;
        if (V_next < 0) V_next += n;

        V_prev = V_curr;
        V_curr = V_next;
    }

    return V_curr;
}

LUCKeys generateLUCKeys() {
    LUCKeys keys;

    // Генерация простых чисел
    int64_t p = generatePrime(1000, 10000);
    int64_t q = generatePrime(1000, 10000);

    while (p == q) {
        q = generatePrime(1000, 10000);
    }

    // Вычисление модуля
    keys.n = p * q;

    // Вычисление функции Кармайкла
    int64_t lambda = lcm(p - legendreSymbol(p, 5), q - legendreSymbol(q, 5));

    // Выбор открытой экспоненты
    int64_t e = 65537;
    while (gcd(e, lambda) != 1) {
        e++;
    }
    keys.publicKey = e;

    // Вычисление секретной экспоненты
    keys.privateKey = modInverse(e, lambda);

    return keys;
}

vector<int64_t> encryptMessageLUC(const string& message, int64_t e, int64_t n) {
    vector<int64_t> encrypted;

    for (char c : message) {
        int64_t m = static_cast<int64_t>(static_cast<unsigned char>(c));
        encrypted.push_back(lucasSequence(e, m, 1, n));
    }

    return encrypted;
}

string decryptMessageLUC(const vector<int64_t>& encrypted, int64_t d, int64_t n) {
    string decrypted;

    for (int64_t num : encrypted) {
        int64_t m = lucasSequence(d, num, 1, n);
        decrypted += static_cast<char>(m);
    }

    return decrypted;
}

void encryptFileLUC(const string& inputFile, const string& outputFile, int64_t e, int64_t n) {
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
        int64_t encrypted = lucasSequence(e, m, 1, n);
        out << encrypted << " ";
    }

    in.close();
    out.close();
}

void decryptFileLUC(const string& inputFile, const string& outputFile, int64_t d, int64_t n) {
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
        int64_t decrypted = lucasSequence(d, num, 1, n);
        out << static_cast<char>(decrypted);
    }

    in.close();
    out.close();
}

void run_luc_crypto() {
    setlocale(LC_ALL, "Russian");

    cout << "LUC Шифрование/Дешифрование\n";
    cout << "1. Шифровать сообщение\n";
    cout << "2. Дешифровать сообщение\n";
    cout << "3. Шифровать файл\n";
    cout << "4. Дешифровать файл\n";
    cout << "5. Сгенерировать новые ключи\n";
    cout << "Выберите действие: ";

    int choice;
    cin >> choice;
    cin.ignore();

    LUCKeys keys = generateLUCKeys();

    if (choice == 5) {
        cout << "Сгенерированы новые ключи:\n";
        cout << "Открытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
        cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n";
        return;
    }

    cout << "\nИспользуются автоматически сгенерированные ключи:\n";
    cout << "Открытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
    cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n\n";

    try {
        switch (choice) {
            case 1: {
                cout << "Введите сообщение для шифрования: ";
                string message;
                getline(cin, message);

                vector<int64_t> encrypted = encryptMessageLUC(message, keys.publicKey, keys.n);
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

                string decrypted = decryptMessageLUC(encrypted, keys.privateKey, keys.n);
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

                encryptFileLUC(inputFile, outputFile, keys.publicKey, keys.n);
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

                decryptFileLUC(inputFile, outputFile, keys.privateKey, keys.n);
                cout << "Файл успешно расшифрован." << endl;
                break;
            }
            default:
                cout << "Неверный выбор." << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "cast128/cast128.h"
#include "rsa/rsa.h"
#include "luc/luc.h"
#include <iomanip>

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void handleCast128() {
    string message, password;
    
    cout << "\n=== CAST-128 Шифрование/Дешифрование ===\n";
    cout << "Введите сообщение: ";
    getline(cin, message);
    
    cout << "Введите пароль: ";
    getline(cin, password);

    try {
        CastKey key = generateCastKey(password);
        vector<uint8_t> encrypted = encryptMessage(message, key);

        cout << "\nРезультат шифрования (hex): ";
        for (uint8_t byte : encrypted) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(byte);
        }
        cout << dec << endl;

        string decrypted = decryptMessage(encrypted, key);
        cout << "Результат дешифрования: " << decrypted << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void handleRSA() {
    string message;
    
    cout << "\n=== RSA Шифрование/Дешифрование ===\n";
    cout << "Введите сообщение: ";
    getline(cin, message);

    try {
        RSAKeys keys = generateRSAKeys();
        vector<int64_t> encrypted = encryptMessageRSA(message, keys.publicKey, keys.n);

        cout << "\nОткрытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
        cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n";
        cout << "Результат шифрования: ";
        for (int64_t num : encrypted) {
            cout << num << " ";
        }
        cout << endl;

        string decrypted = decryptMessageRSA(encrypted, keys.privateKey, keys.n);
        cout << "Результат дешифрования: " << decrypted << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void handleLUC() {
    string message;
    
    cout << "\n=== LUC Шифрование/Дешифрование ===\n";
    cout << "Введите сообщение: ";
    getline(cin, message);

    try {
        LUCKeys keys = generateLUCKeys();
        vector<int64_t> encrypted = encryptMessageLUC(message, keys.publicKey, keys.n);

        cout << "\nОткрытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
        cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n";
        cout << "Результат шифрования: ";
        for (int64_t num : encrypted) {
            cout << num << " ";
        }
        cout << endl;

        string decrypted = decryptMessageLUC(encrypted, keys.privateKey, keys.n);
        cout << "Результат дешифрования: " << decrypted << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void showMainMenu() {
    cout << "\n=== Криптографическое Приложение ===" << endl;
    cout << "1. CAST-128" << endl;
    cout << "2. RSA" << endl;
    cout << "3. LUC" << endl;
    cout << "4. Выход" << endl;
    cout << "Выберите алгоритм: ";
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    int choice;
    do {
        showMainMenu();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: handleCast128(); break;
            case 2: handleRSA(); break;
            case 3: handleLUC(); break;
            case 4: cout << "Завершение работы программы." << endl; break;
            default: cout << "Неверный выбор. Пожалуйста, введите число от 1 до 4." << endl;
        }
    } while (choice != 4);

    return 0;
}

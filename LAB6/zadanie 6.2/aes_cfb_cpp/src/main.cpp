/**
 * @file main.cpp
 * @brief Основной файл демонстрации работы AES-128 в режиме CFB
 * 
 * Содержит точку входа в программу, демонстрирует процесс:
 * 1. Генерации ключа и вектора инициализации
 * 2. Шифрования введенного пользователем текста
 * 3. Дешифрования и проверки результата
 */

#include "aes_cfb.h"
#include <iostream>
#include <fstream>
#include <string>

/**
 * @brief Добавляет PKCS7 padding к данным
 * @param data Исходные данные
 * @return Данные с добавленным padding
 */
std::string add_padding(const std::string& data) {
    int pad_len = AES_BLOCK_SIZE - (data.size() % AES_BLOCK_SIZE);
    if (pad_len == 0) pad_len = AES_BLOCK_SIZE;
    return data + std::string(pad_len, static_cast<char>(pad_len));
}

/**
 * @brief Удаляет PKCS7 padding из данных
 * @param data Данные с padding
 * @return Данные без padding
 */
std::string remove_padding(const std::string& data) {
    if (data.empty()) return data;
    int pad_len = static_cast<unsigned char>(data.back());
    if (pad_len > AES_BLOCK_SIZE) return data;
    return data.substr(0, data.size() - pad_len);
}

int main() {
    try {
        // Генерация ключа и IV
        unsigned char key[AES_BLOCK_SIZE];
        unsigned char iv[AES_BLOCK_SIZE];
        
        generate_random_bytes(key, AES_BLOCK_SIZE);
        generate_random_bytes(iv, AES_BLOCK_SIZE);
        
        // Вывод сгенерированных ключей
        print_state(key, "Ключ");
        print_state(iv, "Вектор инициализации (IV)");
        
        // Запись ключа в файл
        std::ofstream key_file("aes_key.bin", std::ios::binary);
        if (!key_file) {
            throw std::runtime_error("Failed to open key file for writing");
        }
        key_file.write(reinterpret_cast<const char*>(key), AES_BLOCK_SIZE);
        key_file.close();
        
        // Ввод текста для шифрования
        std::string plaintext;
        std::cout << "Введите текст для шифрования: ";
        std::getline(std::cin, plaintext);
        
        // Добавление padding
        std::string padded_text = add_padding(plaintext);
        
        // Шифрование
        auto ciphertext = aes_cfb_encrypt(padded_text, key, iv);
        print_state(ciphertext.data(), "Зашифрованный текст");
        
        // Расшифрование
        std::string decrypted = aes_cfb_decrypt(ciphertext, key, iv);
        decrypted = remove_padding(decrypted);
        
        std::cout << "Расшифрованный текст: " << decrypted << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

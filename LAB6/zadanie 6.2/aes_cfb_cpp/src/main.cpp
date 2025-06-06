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

int main() {
    try {
        AES_CFB aes;
        
        std::cout << "Key: " << AES_CFB::bytes_to_hex(aes.get_key().data(), AES_CFB::BLOCK_SIZE) << "\n";
        std::cout << "IV: " << AES_CFB::bytes_to_hex(aes.get_iv().data(), AES_CFB::BLOCK_SIZE) << "\n";
        
        aes.save_key_to_file("aes_key.bin");
        
        std::string plaintext;
        std::cout << "Enter text to encrypt: ";
        std::getline(std::cin, plaintext);
        
        auto ciphertext = aes.encrypt(plaintext);
        std::cout << "Ciphertext: " << AES_CFB::bytes_to_hex(ciphertext.data(), ciphertext.size()) << "\n";
        
        auto decrypted = aes.decrypt(ciphertext);
        std::cout << "Decrypted: " << decrypted << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

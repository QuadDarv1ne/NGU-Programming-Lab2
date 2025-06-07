#include "../include/aes_cfb.h"
#include <iostream>
#include <limits>

/**
 * @brief Очищает буфер ввода
 */
void clear_input_buffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    try {
        std::cout << "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n";
        std::cout << "█ Демонстрация AES-128 CFB (Cipher Feedback) █\n";
        std::cout << "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n";
        
        // Инициализация AES с автоматической генерацией ключей
        AES_CFB aes;
        
        // Вывод сгенерированных ключей
        std::cout << "Сгенерированный ключ: "
                  << AES_CFB::bytes_to_hex(aes.key().data(), AES_CFB::BLOCK_SIZE) << "\n";
        std::cout << "Вектор инициализации: "
                  << AES_CFB::bytes_to_hex(aes.iv().data(), AES_CFB::BLOCK_SIZE) << "\n\n";
        
        // Сохранение ключа
        aes.save_key("aes_key.bin");
        std::cout << "Ключ сохранен в файл: aes_key.bin\n\n";
        
        // Ввод текста для шифрования
        std::string plaintext;
        std::cout << "Введите текст для шифрования: ";
        std::getline(std::cin, plaintext);
        
        // Шифрование
        auto ciphertext = aes.encrypt(plaintext);
        std::cout << "\nШифрование успешно завершено!\n";
        std::cout << "Зашифрованный текст (HEX): " 
                  << AES_CFB::bytes_to_hex(ciphertext.data(), ciphertext.size()) << "\n\n";
        
        // Дешифрование
        auto decrypted = aes.decrypt(ciphertext);
        std::cout << "Дешифрование успешно завершено!\n";
        std::cout << "Дешифрованный текст: " << decrypted << "\n";
        
        std::cout << "\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n";
        std::cout << "█          Программа успешно завершена          █\n";
        std::cout << "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n";
        
    } catch (const std::exception& e) {
        std::cerr << "\nОШИБКА: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

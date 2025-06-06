/**
 * @file aes_cfb.h
 * @brief Заголовочный файл для реализации AES-128 в режиме CFB
 * 
 * Содержит объявления функций для шифрования/дешифрования в режиме CFB,
 * работы с ключами и вспомогательных функций.
 */

#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <array>

class AES_CFB {
public:
    static constexpr size_t BLOCK_SIZE = 16;
    
    using Key = std::array<unsigned char, BLOCK_SIZE>;
    using IV = std::array<unsigned char, BLOCK_SIZE>;

    AES_CFB();
    
    // Генерация ключа и IV
    static Key generate_key();
    static IV generate_iv();
    
    // Основные операции
    std::vector<unsigned char> encrypt(const std::string& plaintext);
    std::string decrypt(const std::vector<unsigned char>& ciphertext);
    
    // Управление ключами
    void set_key(const Key& key) { key_ = key; }
    void set_iv(const IV& iv) { iv_ = iv; }
    const Key& get_key() const { return key_; }
    const IV& get_iv() const { return iv_; }
    
    // Утилиты
    static std::string bytes_to_hex(const unsigned char* data, size_t length);
    void save_key_to_file(const std::string& filename) const;
    void load_key_from_file(const std::string& filename);

private:
    Key key_;
    IV iv_;
    
    void validate_key_iv() const;
    std::string add_padding(const std::string& data) const;
    std::string remove_padding(const std::string& data) const;
};

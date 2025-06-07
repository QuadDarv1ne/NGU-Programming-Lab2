/**
 * @file aes_cfb.h
 * @brief Реализация AES-128 в режиме CFB (Cipher Feedback) с использованием EVP интерфейса
 */

#pragma once

#include <array>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <openssl/evp.h>

class AES_CFB {
public:
    static constexpr size_t BLOCK_SIZE = 16; ///< Размер блока AES в байтах
    
    using Key = std::array<uint8_t, BLOCK_SIZE>;  ///< Тип ключа (128 бит)
    using IV = std::array<uint8_t, BLOCK_SIZE>;   ///< Тип вектора инициализации
    
    AES_CFB();
    ~AES_CFB();
    
    std::vector<uint8_t> encrypt(const std::string& plaintext);
    std::string decrypt(const std::vector<uint8_t>& ciphertext);
    
    const Key& key() const noexcept { return key_; }
    const IV& iv() const noexcept { return iv_; }
    void set_key(const Key& key) noexcept { key_ = key; }
    void set_iv(const IV& iv) noexcept { iv_ = iv; }
    
    void save_key(const std::string& filename) const;
    void load_key(const std::string& filename);
    
    static std::string bytes_to_hex(const uint8_t* data, size_t length);

private:
    Key key_;
    IV iv_;
    
    void validate() const;
};

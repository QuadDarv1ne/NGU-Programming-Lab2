/**
 * @file aes_cfb.cpp
 * @brief Реализация AES-128 в режиме CFB
 * 
 * Содержит реализацию функций для шифрования/дешифрования в режиме CFB,
 * работы с ключами и вспомогательных функций.
 */

#include "aes_cfb.h"
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <fstream>
#include <iomanip>
#include <sstream>

AES_CFB::AES_CFB() : key_(generate_key()), iv_(generate_iv()) {}

AES_CFB::Key AES_CFB::generate_key() {
    Key key;
    if (RAND_bytes(key.data(), key.size()) != 1) {
        throw std::runtime_error("Failed to generate random key");
    }
    return key;
}

AES_CFB::IV AES_CFB::generate_iv() {
    IV iv;
    if (RAND_bytes(iv.data(), iv.size()) != 1) {
        throw std::runtime_error("Failed to generate random IV");
    }
    return iv;
}

void AES_CFB::validate_key_iv() const {
    if (key_.empty() || iv_.empty()) {
        throw std::runtime_error("Key or IV not initialized");
    }
}

std::vector<unsigned char> AES_CFB::encrypt(const std::string& plaintext) {
    validate_key_iv();
    const std::string padded = add_padding(plaintext);
    
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key_.data(), 128, &aes_key) < 0) {
        throw std::runtime_error("Failed to set encryption key");
    }
    
    std::vector<unsigned char> ciphertext(padded.size());
    IV ivec = iv_;
    int num = 0;
    
    AES_cfb128_encrypt(
        reinterpret_cast<const unsigned char*>(padded.data()),
        ciphertext.data(),
        padded.size(),
        &aes_key,
        ivec.data(),
        &num,
        AES_ENCRYPT
    );
    
    return ciphertext;
}

std::string AES_CFB::decrypt(const std::vector<unsigned char>& ciphertext) {
    validate_key_iv();
    
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key_.data(), 128, &aes_key) < 0) {
        throw std::runtime_error("Failed to set encryption key");
    }
    
    std::string plaintext(ciphertext.size(), '\0');
    IV ivec = iv_;
    int num = 0;
    
    AES_cfb128_encrypt(
        ciphertext.data(),
        reinterpret_cast<unsigned char*>(&plaintext[0]),
        ciphertext.size(),
        &aes_key,
        ivec.data(),
        &num,
        AES_DECRYPT
    );
    
    return remove_padding(plaintext);
}

std::string AES_CFB::add_padding(const std::string& data) const {
    size_t pad_len = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    if (pad_len == 0) pad_len = BLOCK_SIZE;
    return data + std::string(pad_len, static_cast<char>(pad_len));
}

std::string AES_CFB::remove_padding(const std::string& data) const {
    if (data.empty()) return data;
    size_t pad_len = static_cast<unsigned char>(data.back());
    if (pad_len > BLOCK_SIZE) return data;
    return data.substr(0, data.size() - pad_len);
}

std::string AES_CFB::bytes_to_hex(const unsigned char* data, size_t length) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        oss << std::setw(2) << static_cast<int>(data[i]);
    }
    return oss.str();
}

void AES_CFB::save_key_to_file(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open key file for writing");
    }
    file.write(reinterpret_cast<const char*>(key_.data()), key_.size());
}

void AES_CFB::load_key_from_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open key file for reading");
    }
    file.read(reinterpret_cast<char*>(key_.data()), key_.size());
}

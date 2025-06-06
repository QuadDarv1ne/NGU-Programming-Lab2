/**
 * @file aes_cfb.cpp
 * @brief Реализация AES-128 в режиме CFB
 * 
 * Содержит реализацию функций для шифрования/дешифрования в режиме CFB,
 * работы с ключами и вспомогательных функций.
 */

#include "aes_cfb.h"
#include <iostream>
#include <iomanip>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <stdexcept>

void generate_random_bytes(unsigned char* buffer, int length) {
    if (RAND_bytes(buffer, length) != 1) {
        throw std::runtime_error("Error generating random bytes");
    }
}

void print_state(const unsigned char* state, const std::string& label) {
    std::cout << label << ": ";
    for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                 << (int)state[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

std::vector<unsigned char> aes_cfb_encrypt(const std::string& plaintext, 
                                         const unsigned char* key, 
                                         const unsigned char* iv) {
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 128, &aes_key) < 0) {
        throw std::runtime_error("Failed to set encryption key");
    }
    
    std::vector<unsigned char> ciphertext(plaintext.size());
    unsigned char ivec[AES_BLOCK_SIZE];
    memcpy(ivec, iv, AES_BLOCK_SIZE);
    
    int num = 0;
    
    AES_cfb128_encrypt(
        reinterpret_cast<const unsigned char*>(plaintext.data()),
        ciphertext.data(),
        plaintext.size(),
        &aes_key,
        ivec,
        &num,
        AES_ENCRYPT
    );
    
    return ciphertext;
}

std::string aes_cfb_decrypt(const std::vector<unsigned char>& ciphertext, 
                           const unsigned char* key, 
                           const unsigned char* iv) {
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 128, &aes_key) < 0) {
        throw std::runtime_error("Failed to set encryption key");
    }
    
    std::string plaintext(ciphertext.size(), '\0');
    unsigned char ivec[AES_BLOCK_SIZE];
    memcpy(ivec, iv, AES_BLOCK_SIZE);
    
    int num = 0;
    
    AES_cfb128_encrypt(
        ciphertext.data(),
        reinterpret_cast<unsigned char*>(&plaintext[0]),
        ciphertext.size(),
        &aes_key,
        ivec,
        &num,
        AES_DECRYPT
    );
    
    return plaintext;
}

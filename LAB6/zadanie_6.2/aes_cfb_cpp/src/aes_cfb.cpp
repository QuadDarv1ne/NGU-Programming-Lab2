#include "../include/aes_cfb.h"
#include <openssl/rand.h>
#include <openssl/err.h>
#include <fstream>
#include <sstream>
#include <iomanip>

AES_CFB::AES_CFB() {
    if (RAND_bytes(key_.data(), key_.size()) != 1) {
        throw std::runtime_error("Ошибка генерации ключа");
    }
    if (RAND_bytes(iv_.data(), iv_.size()) != 1) {
        throw std::runtime_error("Ошибка генерации вектора инициализации");
    }
}

AES_CFB::~AES_CFB() {}

void AES_CFB::validate() const {
    if (key_.size() != BLOCK_SIZE) {
        throw std::invalid_argument("Неверный размер ключа");
    }
    if (iv_.size() != BLOCK_SIZE) {
        throw std::invalid_argument("Неверный размер вектора инициализации");
    }
}

std::vector<uint8_t> AES_CFB::encrypt(const std::string& plaintext) {
    validate();
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Ошибка создания контекста шифрования");
    }
    
    // Инициализация контекста для шифрования
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cfb128(), NULL, key_.data(), iv_.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка инициализации шифрования");
    }
    
    std::vector<uint8_t> ciphertext(plaintext.size() + BLOCK_SIZE);
    int out_len = 0;
    
    // Обработка данных
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len,
                              reinterpret_cast<const uint8_t*>(plaintext.data()),
                              static_cast<int>(plaintext.size()))) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка шифрования данных");
    }
    
    int final_len = 0;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка финализации шифрования");
    }
    
    ciphertext.resize(out_len + final_len);
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

std::string AES_CFB::decrypt(const std::vector<uint8_t>& ciphertext) {
    validate();
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Ошибка создания контекста дешифрования");
    }
    
    // Инициализация контекста для дешифрования
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cfb128(), NULL, key_.data(), iv_.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка инициализации дешифрования");
    }
    
    std::string plaintext(ciphertext.size(), '\0');
    int out_len = 0;
    
    // Обработка данных
    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<uint8_t*>(&plaintext[0]), &out_len,
                              ciphertext.data(), static_cast<int>(ciphertext.size()))) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка дешифрования данных");
    }
    
    int final_len = 0;
    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<uint8_t*>(&plaintext[0]) + out_len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка финализации дешифрования");
    }
    
    plaintext.resize(out_len + final_len);
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

std::string AES_CFB::bytes_to_hex(const uint8_t* data, size_t length) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        oss << std::setw(2) << static_cast<int>(data[i]);
    }
    return oss.str();
}

void AES_CFB::save_key(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла ключа для записи");
    }
    file.write(reinterpret_cast<const char*>(key_.data()), key_.size());
}

void AES_CFB::load_key(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла ключа для чтения");
    }
    file.read(reinterpret_cast<char*>(key_.data()), key_.size());
}

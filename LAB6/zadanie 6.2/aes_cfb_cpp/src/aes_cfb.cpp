#include "../include/aes_cfb.h"
#include <openssl/rand.h>
#include <fstream>
#include <sstream>
#include <iomanip>

// Конструктор с автоматической генерацией ключей
AES_CFB::AES_CFB() {
    // Генерация криптографически безопасного ключа
    if (RAND_bytes(key_.data(), key_.size()) != 1) {
        throw std::runtime_error("Ошибка генерации ключа");
    }
    
    // Генерация криптографически безопасного вектора инициализации
    if (RAND_bytes(iv_.data(), iv_.size()) != 1) {
        throw std::runtime_error("Ошибка генерации вектора инициализации");
    }
}

// Шифрование данных
std::vector<uint8_t> AES_CFB::encrypt(const std::string& plaintext) {
    validate();
    const std::string padded = add_padding(plaintext);
    
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key_.data(), 128, &aes_key) < 0) {
        throw std::runtime_error("Ошибка установки ключа шифрования");
    }
    
    std::vector<uint8_t> ciphertext(padded.size());
    IV ivec = iv_;
    int num = 0;
    
    AES_cfb128_encrypt(
        reinterpret_cast<const uint8_t*>(padded.data()),
        ciphertext.data(),
        padded.size(),
        &aes_key,
        ivec.data(),
        &num,
        AES_ENCRYPT
    );
    
    return ciphertext;
}

// Дешифрование данных
std::string AES_CFB::decrypt(const std::vector<uint8_t>& ciphertext) {
    validate();
    
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key_.data(), 128, &aes_key) < 0) {
        throw std::runtime_error("Ошибка установки ключа дешифрования");
    }
    
    std::string plaintext(ciphertext.size(), '\0');
    IV ivec = iv_;
    int num = 0;
    
    AES_cfb128_encrypt(
        ciphertext.data(),
        reinterpret_cast<uint8_t*>(plaintext.data()),
        ciphertext.size(),
        &aes_key,
        ivec.data(),
        &num,
        AES_DECRYPT
    );
    
    return remove_padding(plaintext);
}

// Сохранение ключа в файл
void AES_CFB::save_key(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла ключа для записи");
    }
    file.write(reinterpret_cast<const char*>(key_.data()), key_.size());
}

// Загрузка ключа из файла
void AES_CFB::load_key(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла ключа для чтения");
    }
    file.read(reinterpret_cast<char*>(key_.data()), key_.size());
}

// Преобразование байтов в HEX-строку
std::string AES_CFB::bytes_to_hex(const uint8_t* data, size_t length) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        oss << std::setw(2) << static_cast<int>(data[i]);
    }
    return oss.str();
}

// Проверка валидности ключей
void AES_CFB::validate() const {
    if (key_.size() != BLOCK_SIZE) {
        throw std::invalid_argument("Неверный размер ключа");
    }
    if (iv_.size() != BLOCK_SIZE) {
        throw std::invalid_argument("Неверный размер вектора инициализации");
    }
}

// Добавление PKCS7 заполнения
std::string AES_CFB::add_padding(const std::string& data) const {
    size_t pad_len = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    if (pad_len == 0) pad_len = BLOCK_SIZE;
    return data + std::string(pad_len, static_cast<char>(pad_len));
}

// Удаление PKCS7 заполнения
std::string AES_CFB::remove_padding(const std::string& data) const {
    if (data.empty()) return data;
    size_t pad_len = static_cast<uint8_t>(data.back());
    if (pad_len > BLOCK_SIZE || pad_len == 0) return data;
    return data.substr(0, data.size() - pad_len);
}

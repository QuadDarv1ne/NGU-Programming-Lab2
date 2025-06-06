/**
 * @file aes_cfb.h
 * @brief Реализация AES-128 в режиме CFB (Cipher Feedback)
 * 
 * Предоставляет высокопроизводительную и безопасную реализацию AES-128 в режиме CFB
 * с полной документацией и обработкой ошибок.
 */

#pragma once

#include <array>
#include <vector>
#include <string>
#include <stdexcept>
#include <openssl/aes.h>

/**
 * @class AES_CFB
 * @brief Шифрование/дешифрование AES-128 в режиме CFB
 * 
 * Реализует AES-128 CFB с:
 * - Автоматической генерацией ключа и вектора инициализации
 * - PKCS7 заполнением
 * - Безопасным управлением ключами
 * - Проверкой состояния
 */
class AES_CFB {
public:
    static constexpr size_t BLOCK_SIZE = 16; ///< Размер блока AES в байтах
    
    using Key = std::array<uint8_t, BLOCK_SIZE>;  ///< Тип ключа (128 бит)
    using IV = std::array<uint8_t, BLOCK_SIZE>;   ///< Тип вектора инициализации
    
    /**
     * @brief Создает новый объект AES_CFB со случайным ключом и вектором инициализации
     * @throws std::runtime_error при ошибке инициализации криптографии
     */
    AES_CFB();
    
    /**
     * @brief Шифрует открытый текст с использованием AES-128 CFB
     * @param plaintext Входные данные для шифрования
     * @return std::vector<uint8_t> Зашифрованный текст
     * @throws std::runtime_error при ошибке шифрования
     */
    std::vector<uint8_t> encrypt(const std::string& plaintext);
    
    /**
     * @brief Дешифрует зашифрованный текст с использованием AES-128 CFB
     * @param ciphertext Зашифрованные данные для дешифрования
     * @return std::string Дешифрованный открытый текст
     * @throws std::runtime_error при ошибке дешифрования
     */
    std::string decrypt(const std::vector<uint8_t>& ciphertext);
    
    // Управление ключами
    const Key& key() const noexcept { return key_; }
    const IV& iv() const noexcept { return iv_; }
    void set_key(const Key& key) noexcept { key_ = key; }
    void set_iv(const IV& iv) noexcept { iv_ = iv; }
    
    /**
     * @brief Сохраняет ключ в файл
     * @param filename Имя файла для сохранения
     * @throws std::runtime_error при ошибке записи файла
     */
    void save_key(const std::string& filename) const;
    
    /**
     * @brief Загружает ключ из файла
     * @param filename Имя файла для загрузки
     * @throws std::runtime_error при ошибке чтения файла
     */
    void load_key(const std::string& filename);
    
    /**
     * @brief Преобразует двоичные данные в шестнадцатеричную строку
     * @param data Указатель на двоичные данные
     * @param length Длина данных в байтах
     * @return std::string Шестнадцатеричное представление
     */
    static std::string bytes_to_hex(const uint8_t* data, size_t length);

private:
    Key key_;  ///< Ключ шифрования
    IV iv_;    ///< Вектор инициализации
    
    /**
     * @brief Проверяет корректность ключа и вектора инициализации
     * @throws std::invalid_argument при неверных размерах
     */
    void validate() const;
    
    /**
     * @brief Добавляет PKCS7 заполнение к данным
     * @param data Входные данные
     * @return std::string Данные с заполнением
     */
    std::string add_padding(const std::string& data) const;
    
    /**
     * @brief Удаляет PKCS7 заполнение из данных
     * @param data Данные с заполнением
     * @return std::string Данные без заполнения
     */
    std::string remove_padding(const std::string& data) const;
};

/**
 * @file aes_cfb.h
 * @brief Заголовочный файл для реализации AES-128 в режиме CFB
 * 
 * Содержит объявления функций для шифрования/дешифрования в режиме CFB,
 * работы с ключами и вспомогательных функций.
 */

#ifndef AES_CFB_H
#define AES_CFB_H

#include <vector>
#include <string>

/**
 * @brief Генерирует случайные байты
 * @param buffer Буфер для записи случайных данных
 * @param length Длина буфера
 * @throws std::runtime_error Если не удалось сгенерировать случайные байты
 */
void generate_random_bytes(unsigned char* buffer, int length);

/**
 * @brief Выводит состояние (state) в шестнадцатеричном формате
 * @param state Указатель на данные
 * @param label Метка для вывода
 */
void print_state(const unsigned char* state, const std::string& label);

/**
 * @brief Шифрует данные в режиме CFB
 * @param plaintext Исходный текст
 * @param key Ключ шифрования (16 байт)
 * @param iv Вектор инициализации (16 байт)
 * @return Вектор с зашифрованными данными
 */
std::vector<unsigned char> aes_cfb_encrypt(const std::string& plaintext, 
                                         const unsigned char* key, 
                                         const unsigned char* iv);

/**
 * @brief Дешифрует данные в режиме CFB
 * @param ciphertext Зашифрованные данные
 * @param key Ключ шифрования (16 байт)
 * @param iv Вектор инициализации (16 байт)
 * @return Расшифрованная строка
 */
std::string aes_cfb_decrypt(const std::vector<unsigned char>& ciphertext, 
                           const unsigned char* key, 
                           const unsigned char* iv);

#endif // AES_CFB_H

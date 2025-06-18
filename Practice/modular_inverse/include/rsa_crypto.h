#ifndef RSA_CRYPTO_H
#define RSA_CRYPTO_H

#include <cstdint>

// Структура для хранения ключей RSA
struct RSAKeys {
    int64_t publicKey;
    int64_t privateKey;
    int64_t n;
};

// Основная функция для запуска RSA-шифрования
void run_rsa_crypto();

#endif

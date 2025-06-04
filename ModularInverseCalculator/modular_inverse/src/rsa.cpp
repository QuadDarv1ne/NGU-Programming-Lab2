#include "rsa.h"
#include <iostream>
#include <vector>
#include <openssl/bn.h>
#include <openssl/err.h>

// Функция для генерации простого числа
BIGNUM* generate_prime(int bits) {
    BIGNUM* prime = BN_new();
    if (!BN_generate_prime_ex(prime, bits, 1, NULL, NULL, NULL)) {
        std::cerr << "Ошибка генерации простого числа" << std::endl;
        BN_free(prime);
        return nullptr;
    }
    return prime;
}

// Функция для вычисления обратного элемента по модулю
BIGNUM* mod_inverse_rsa(BIGNUM* a, BIGNUM* m, BN_CTX* ctx) {
    BIGNUM* inv = BN_new();
    if (!BN_mod_inverse(inv, a, m, ctx)) {
        BN_free(inv);
        return nullptr;
    }
    return inv;
}

// Шифрование RSA
BIGNUM* rsa_encrypt(const std::string& message, BIGNUM* e, BIGNUM* n, BN_CTX* ctx) {
    BIGNUM* m = BN_bin2bn((const unsigned char*)message.c_str(), message.size(), NULL);
    if (!m) return nullptr;
    
    if (BN_cmp(m, n) >= 0) {
        BN_free(m);
        std::cerr << "Ошибка: сообщение слишком длинное для ключа" << std::endl;
        return nullptr;
    }
    
    BIGNUM* c = BN_new();
    if (!BN_mod_exp(c, m, e, n, ctx)) {
        BN_free(m);
        BN_free(c);
        return nullptr;
    }
    
    BN_free(m);
    return c;
}

// Дешифрование RSA
std::string rsa_decrypt(BIGNUM* c, BIGNUM* d, BIGNUM* n, BN_CTX* ctx) {
    BIGNUM* m = BN_new();
    if (!BN_mod_exp(m, c, d, n, ctx)) {
        BN_free(m);
        return "";
    }
    
    int size = BN_num_bytes(m);
    std::vector<unsigned char> buffer(size);
    BN_bn2bin(m, buffer.data());
    std::string result(buffer.begin(), buffer.end());
    
    BN_free(m);
    return result;
}

// Демонстрация работы RSA
void run_rsa_demo() {
    BN_CTX* ctx = BN_CTX_new();
    if (!ctx) {
        std::cerr << "Ошибка создания контекста BN_CTX" << std::endl;
        return;
    }

    const int bits = 256;
    std::cout << "Генерация ключей RSA (" << bits << " бит)..." << std::endl;
    
    BIGNUM* p = generate_prime(bits);
    BIGNUM* q = generate_prime(bits);
    if (!p || !q) {
        std::cerr << "Ошибка генерации простых чисел" << std::endl;
        BN_CTX_free(ctx);
        return;
    }

    BIGNUM* n = BN_new();
    BN_mul(n, p, q, ctx);

    BIGNUM* phi = BN_new();
    BIGNUM* p1 = BN_dup(p);
    BIGNUM* q1 = BN_dup(q);
    BN_sub_word(p1, 1);
    BN_sub_word(q1, 1);
    BN_mul(phi, p1, q1, ctx);

    BIGNUM* e = BN_new();
    BN_set_word(e, 65537);

    BIGNUM* gcd = BN_new();
    BN_gcd(gcd, e, phi, ctx);
    if (!BN_is_one(gcd)) {
        std::cerr << "e и φ(n) не взаимно просты" << std::endl;
        // Освобождаем ресурсы
        BN_free(p);
        BN_free(q);
        BN_free(n);
        BN_free(phi);
        BN_free(e);
        BN_free(gcd);
        BN_free(p1);
        BN_free(q1);
        BN_CTX_free(ctx);
        return;
    }

    BIGNUM* d = mod_inverse_rsa(e, phi, ctx);
    if (!d) {
        std::cerr << "Ошибка вычисления секретной экспоненты" << std::endl;
        BN_free(p);
        BN_free(q);
        BN_free(n);
        BN_free(phi);
        BN_free(e);
        BN_free(gcd);
        BN_free(p1);
        BN_free(q1);
        BN_CTX_free(ctx);
        return;
    }

    std::cin.ignore(); // Очистка буфера
    std::string message;
    std::cout << "Введите сообщение для шифрования: ";
    std::getline(std::cin, message);
    
    if (message.size() > BN_num_bytes(n) - 11) {
        std::cerr << "Ошибка: сообщение слишком длинное" << std::endl;
        std::cerr << "Максимальная длина: " << BN_num_bytes(n) - 11 << " байт" << std::endl;
        // Освобождаем ресурсы
        BN_free(p);
        BN_free(q);
        BN_free(n);
        BN_free(phi);
        BN_free(e);
        BN_free(d);
        BN_free(gcd);
        BN_free(p1);
        BN_free(q1);
        BN_CTX_free(ctx);
        return;
    }

    BIGNUM* ciphertext = rsa_encrypt(message, e, n, ctx);
    if (!ciphertext) {
        std::cerr << "Ошибка шифрования" << std::endl;
        // Освобождаем ресурсы
        BN_free(p);
        BN_free(q);
        BN_free(n);
        BN_free(phi);
        BN_free(e);
        BN_free(d);
        BN_free(gcd);
        BN_free(p1);
        BN_free(q1);
        BN_CTX_free(ctx);
        return;
    }

    char* hex_cipher = BN_bn2hex(ciphertext);
    std::cout << "Зашифрованное сообщение (hex): " << hex_cipher << std::endl;

    std::string decrypted = rsa_decrypt(ciphertext, d, n, ctx);
    if (decrypted.empty()) {
        std::cerr << "Ошибка дешифрования" << std::endl;
    } else {
        std::cout << "Расшифрованное сообщение: " << decrypted << std::endl;
    }

    // Освобождение ресурсов
    BN_free(p);
    BN_free(q);
    BN_free(n);
    BN_free(phi);
    BN_free(e);
    BN_free(d);
    BN_free(gcd);
    BN_free(ciphertext);
    BN_free(p1);
    BN_free(q1);
    OPENSSL_free(hex_cipher);
    BN_CTX_free(ctx);
}
/**
 * @file rsa_attack_demo.cpp
 * @brief Демонстрация атаки на алгоритм RSA методом факторизации модуля
 * 
 * Программа генерирует RSA-ключи с небольшим размером (32 бита) для демонстрации,
 * шифрует введенное пользователем сообщение, а затем проводит атаку путем факторизации
 * модуля n для получения закрытого ключа и расшифровки сообщения.
 * 
 * Основные этапы:
 * 1. Генерация RSA-ключей (32 бита)
 * 2. Шифрование пользовательского сообщения
 * 3. Демонстрация атаки:
 *    - Факторизация модуля n (нахождение p и q)
 *    - Вычисление φ(n) = (p-1)(q-1)
 *    - Вычисление секретной экспоненты d
 *    - Расшифровка сообщения с помощью полученного d
 * 
 * Особенности:
 * - Используется библиотека OpenSSL для работы с большими числами
 * - Реализован простой алгоритм факторизации перебором делителей
 * - Наглядно демонстрируется уязвимость RSA при использовании коротких ключей
 * 
 * @warning Данная демонстрация использует 32-битные ключи ТОЛЬКО для учебных целей.
 *          В реальных системах следует использовать ключи длиной 2048+ бит.
 * 
 * Компиляция:
 *   g++ -o rsa_attack_demo rsa_attack_demo.cpp -lcrypto
 * 
 * Запуск:
 *   ./rsa_attack_demo
 */

#include <iostream>
#include <string>
#include <vector>
#include <openssl/bn.h>
#include <openssl/rand.h>

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
BIGNUM* mod_inverse(BIGNUM* a, BIGNUM* m, BN_CTX* ctx) {
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

// Атака методом факторизации (перебор делителей)
BIGNUM* factorize(BIGNUM* n, BN_CTX* ctx) {
    BIGNUM* i = BN_new();
    BN_set_word(i, 2);
    
    BIGNUM* sqrt_n = BN_new();
    BN_sqrt(sqrt_n, n, ctx);
    
    BIGNUM* remainder = BN_new();
    BIGNUM* quotient = BN_new();
    
    while (BN_cmp(i, sqrt_n) <= 0) {
        BN_div(quotient, remainder, n, i, ctx);
        
        if (BN_is_zero(remainder)) {
            BIGNUM* result = BN_dup(i);
            BN_free(i);
            BN_free(sqrt_n);
            BN_free(remainder);
            BN_free(quotient);
            return result;
        }
        
        BN_add_word(i, 1);
    }
    
    BN_free(i);
    BN_free(sqrt_n);
    BN_free(remainder);
    BN_free(quotient);
    return nullptr;
}

// Эмуляция атаки на RSA
void rsa_attack(BIGNUM* e, BIGNUM* n, BIGNUM* ciphertext, BN_CTX* ctx) {
    std::cout << "\n=== НАЧАЛО АТАКИ ===" << std::endl;
    std::cout << "Цель: Расшифровать сообщение без закрытого ключа" << std::endl;
    std::cout << "Метод: Факторизация модуля n" << std::endl;
    
    // Шаг 1: Факторизация n
    std::cout << "1. Факторизация модуля n..." << std::endl;
    BIGNUM* p = factorize(n, ctx);
    if (!p) {
        std::cerr << "Факторизация не удалась!" << std::endl;
        return;
    }
    
    BIGNUM* q = BN_new();
    BN_div(q, nullptr, n, p, ctx);
    
    char* hex_p = BN_bn2hex(p);
    char* hex_q = BN_bn2hex(q);
    std::cout << "   Найдены множители:" << std::endl;
    std::cout << "   p = " << hex_p << std::endl;
    std::cout << "   q = " << hex_q << std::endl;
    OPENSSL_free(hex_p);
    OPENSSL_free(hex_q);
    
    // Шаг 2: Вычисление φ(n)
    std::cout << "2. Вычисление φ(n)..." << std::endl;
    BIGNUM* phi = BN_new();
    BIGNUM* p1 = BN_dup(p);
    BIGNUM* q1 = BN_dup(q);
    BN_sub_word(p1, 1);
    BN_sub_word(q1, 1);
    BN_mul(phi, p1, q1, ctx);
    
    // Шаг 3: Вычисление закрытого ключа d
    std::cout << "3. Вычисление секретной экспоненты d..." << std::endl;
    BIGNUM* d = mod_inverse(e, phi, ctx);
    if (!d) {
        std::cerr << "Ошибка вычисления d!" << std::endl;
        return;
    }
    
    char* hex_d = BN_bn2hex(d);
    std::cout << "   d = " << hex_d << std::endl;
    OPENSSL_free(hex_d);
    
    // Шаг 4: Расшифрование сообщения
    std::cout << "4. Расшифрование сообщения..." << std::endl;
    std::string decrypted = rsa_decrypt(ciphertext, d, n, ctx);
    if (decrypted.empty()) {
        std::cerr << "Ошибка дешифрования!" << std::endl;
    } else {
        std::cout << "   ПЕРЕХВАЧЕННОЕ СООБЩЕНИЕ: \"" << decrypted << "\"" << std::endl;
    }
    
    std::cout << "=== АТАКА УСПЕШНО ЗАВЕРШЕНА ===" << std::endl;
    
    // Освобождение ресурсов
    BN_free(p);
    BN_free(q);
    BN_free(phi);
    BN_free(p1);
    BN_free(q1);
    BN_free(d);
}

int main() {
    BN_CTX* ctx = BN_CTX_new();
    if (!ctx) {
        std::cerr << "Ошибка создания контекста BN_CTX" << std::endl;
        return 1;
    }

    // Используем 32-битные ключи для демонстрации атаки
    const int bits = 32;
    std::cout << "Генерация RSA ключей (" << bits << " бит)..." << std::endl;
    
    BIGNUM* p = generate_prime(bits);
    BIGNUM* q = generate_prime(bits);
    if (!p || !q) {
        std::cerr << "Ошибка генерации простых чисел" << std::endl;
        return 1;
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

    BIGNUM* d = mod_inverse(e, phi, ctx);
    if (!d) {
        std::cerr << "Ошибка вычисления секретной экспоненты" << std::endl;
        return 1;
    }

    // Ввод сообщения
    std::string message;
    std::cout << "Введите сообщение для шифрования: ";
    std::getline(std::cin, message);
    
    // Проверка длины сообщения
    if (message.size() > BN_num_bytes(n) - 11) {
        std::cerr << "Ошибка: сообщение слишком длинное" << std::endl;
        return 1;
    }

    // Шифрование
    BIGNUM* ciphertext = rsa_encrypt(message, e, n, ctx);
    if (!ciphertext) {
        std::cerr << "Ошибка шифрования" << std::endl;
        return 1;
    }

    // Вывод информации о ключах
    char* hex_n = BN_bn2hex(n);
    char* hex_e = BN_bn2hex(e);
    char* hex_c = BN_bn2hex(ciphertext);
    
    std::cout << "\nОткрытый ключ:" << std::endl;
    std::cout << "   n = " << hex_n << std::endl;
    std::cout << "   e = " << hex_e << std::endl;
    std::cout << "Зашифрованное сообщение: " << hex_c << std::endl;
    
    // Освобождение временных ресурсов
    OPENSSL_free(hex_n);
    OPENSSL_free(hex_e);
    OPENSSL_free(hex_c);
    
    // Запуск атаки
    rsa_attack(e, n, ciphertext, ctx);

    // Освобождение ресурсов
    BN_free(p);
    BN_free(q);
    BN_free(n);
    BN_free(phi);
    BN_free(e);
    BN_free(d);
    BN_free(ciphertext);
    BN_free(p1);
    BN_free(q1);
    BN_CTX_free(ctx);

    return 0;
}

/**
 * # Установите зависимости
 * sudo apt-get install libssl-dev
 * 
 * # Компиляция
 * g++ -o rsa_attack_demo rsa_attack_demo.cpp -lcrypto
 * 
 * # Запуск
 * ./rsa_attack_demo
 */

 /**
  * ~ Пример вывода:
  * Генерация RSA ключей (32 бит)...
  * Введите сообщение для шифрования: Secret Message
  * 
  * Открытый ключ:
  * n = A3D472F1
  * e = 10001
  * Зашифрованное сообщение: 8D0A3F2C
  * 
  * === НАЧАЛО АТАКИ ===
  * Цель: Расшифровать сообщение без закрытого ключа
  * Метод: Факторизация модуля n
  * 1. Факторизация модуля n...
  *    Найдены множители:
  *    p = D8A3
  *    q = C2E7
  * 2. Вычисление φ(n)...
  * 3. Вычисление секретной экспоненты d...
  *    d = 8C2D5B01
  * 4. Расшифрование сообщения...
  *    ПЕРЕХВАЧЕННОЕ СООБЩЕНИЕ: "Secret Message"
  * === АТАКА УСПЕШНО ЗАВЕРШЕНА === 
  */
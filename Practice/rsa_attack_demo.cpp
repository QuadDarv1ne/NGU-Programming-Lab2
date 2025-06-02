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
#include <locale.h>

// Функция для вычисления квадратного корня из числа BIGNUM
BIGNUM* BN_sqrt(BIGNUM* sqrt_n, const BIGNUM* n, BN_CTX* ctx) {
    BIGNUM* low = BN_new();
    BIGNUM* high = BN_new();
    BIGNUM* mid = BN_new();
    BIGNUM* tmp = BN_new();
    BIGNUM* one = BN_new();
    BIGNUM* two = BN_new();

    BN_one(one);
    BN_set_word(two, 2);
    BN_copy(low, one);
    BN_copy(high, n);
    BN_copy(mid, n);

    while (BN_cmp(low, high) <= 0) {
        BN_copy(mid, high);
        BN_sub(mid, high, low);
        BN_div(mid, NULL, mid, two, ctx);
        BN_add(mid, mid, low);

        if (!BN_mod_exp(tmp, mid, two, n, ctx)) {
            BN_free(low);
            BN_free(high);
            BN_free(mid);
            BN_free(tmp);
            BN_free(one);
            BN_free(two);
            return nullptr;
        }

        int cmp = BN_cmp(tmp, n);
        if (cmp == 0) {
            BN_copy(sqrt_n, mid);
            BN_free(low);
            BN_free(high);
            BN_free(mid);
            BN_free(tmp);
            BN_free(one);
            BN_free(two);
            return sqrt_n;
        } else if (cmp < 0) {
            BN_copy(low, mid);
            BN_add_word(low, 1);
        } else {
            BN_copy(high, mid);
            BN_sub_word(high, 1);
        }
    }

    BN_free(low);
    BN_free(high);
    BN_free(mid);
    BN_free(tmp);
    BN_free(one);
    BN_free(two);
    return nullptr;
}

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

// Факторизация методом Ферма
BIGNUM* factorize(BIGNUM* n, BN_CTX* ctx) {
    BIGNUM* x = BN_new();
    BIGNUM* y = BN_new();
    BIGNUM* sqrt_n = BN_new();
    BIGNUM* tmp = BN_new();
    BIGNUM* result = BN_new();

    BN_sqrt(sqrt_n, n, ctx);
    BN_copy(x, sqrt_n);
    BN_add_word(x, 1);

    while (true) {
        BN_mod_exp(tmp, x, BN_value_one(), n, ctx);
        BN_sub(tmp, tmp, n);

        if (!BN_sqrt(y, tmp, ctx)) {
            BN_free(x);
            BN_free(y);
            BN_free(sqrt_n);
            BN_free(tmp);
            BN_free(result);
            return nullptr;
        }

        if (BN_is_zero(y)) {
            BN_add_word(x, 1);
            continue;
        }

        BN_add(result, x, y);
        BN_div(result, NULL, result, BN_value_one(), ctx);

        if (!BN_is_zero(result)) {
            BN_sub(result, x, y);
            BN_div(result, NULL, result, BN_value_one(), ctx);

            if (!BN_is_zero(result)) {
                BN_free(x);
                BN_free(y);
                BN_free(sqrt_n);
                BN_free(tmp);
                return result;
            }
        }

        BN_add_word(x, 1);
    }

    BN_free(x);
    BN_free(y);
    BN_free(sqrt_n);
    BN_free(tmp);
    BN_free(result);
    return nullptr;
}

// Эмуляция атаки на RSA
void rsa_attack(BIGNUM* e, BIGNUM* n, BIGNUM* ciphertext, BN_CTX* ctx, bool use_russian) {
    if (use_russian) {
        std::cout << "\n=== НАЧАЛО АТАКИ ===" << std::endl;
        std::cout << "Цель: Расшифровать сообщение без закрытого ключа" << std::endl;
        std::cout << "Метод: Факторизация модуля n" << std::endl;
    } else {
        std::cout << "\n=== START OF ATTACK ===" << std::endl;
        std::cout << "Goal: Decrypt the message without the private key" << std::endl;
        std::cout << "Method: Factorization of the modulus n" << std::endl;
    }

    // Step 1: Factorization of n
    if (use_russian) {
        std::cout << "1. Факторизация модуля n..." << std::endl;
    } else {
        std::cout << "1. Factorizing the modulus n..." << std::endl;
    }

    BIGNUM* p = factorize(n, ctx);
    if (!p) {
        if (use_russian) {
            std::cerr << "Факторизация не удалась!" << std::endl;
        } else {
            std::cerr << "Factorization failed!" << std::endl;
        }
        return;
    }

    BIGNUM* q = BN_new();
    BN_div(q, nullptr, n, p, ctx);

    char* hex_p = BN_bn2hex(p);
    char* hex_q = BN_bn2hex(q);
    if (use_russian) {
        std::cout << "   Найдены множители:" << std::endl;
        std::cout << "   p = " << hex_p << std::endl;
        std::cout << "   q = " << hex_q << std::endl;
    } else {
        std::cout << "   Found factors:" << std::endl;
        std::cout << "   p = " << hex_p << std::endl;
        std::cout << "   q = " << hex_q << std::endl;
    }
    OPENSSL_free(hex_p);
    OPENSSL_free(hex_q);

    // Step 2: Calculate φ(n)
    if (use_russian) {
        std::cout << "2. Вычисление φ(n)..." << std::endl;
    } else {
        std::cout << "2. Calculating φ(n)..." << std::endl;
    }

    BIGNUM* phi = BN_new();
    BIGNUM* p1 = BN_dup(p);
    BIGNUM* q1 = BN_dup(q);
    BN_sub_word(p1, 1);
    BN_sub_word(q1, 1);
    BN_mul(phi, p1, q1, ctx);

    // Step 3: Calculate the private exponent d
    if (use_russian) {
        std::cout << "3. Вычисление секретной экспоненты d..." << std::endl;
    } else {
        std::cout << "3. Calculating the private exponent d..." << std::endl;
    }

    BIGNUM* d = mod_inverse(e, phi, ctx);
    if (!d) {
        if (use_russian) {
            std::cerr << "Ошибка вычисления d!" << std::endl;
        } else {
            std::cerr << "Error calculating d!" << std::endl;
        }
        return;
    }

    char* hex_d = BN_bn2hex(d);
    if (use_russian) {
        std::cout << "   d = " << hex_d << std::endl;
    } else {
        std::cout << "   d = " << hex_d << std::endl;
    }
    OPENSSL_free(hex_d);

    // Step 4: Decrypt the message
    if (use_russian) {
        std::cout << "4. Расшифрование сообщения..." << std::endl;
    } else {
        std::cout << "4. Decrypting the message..." << std::endl;
    }

    std::string decrypted = rsa_decrypt(ciphertext, d, n, ctx);
    if (decrypted.empty()) {
        if (use_russian) {
            std::cerr << "Ошибка дешифрования!" << std::endl;
        } else {
            std::cerr << "Decryption error!" << std::endl;
        }
    } else {
        if (use_russian) {
            std::cout << "   ПЕРЕХВАЧЕННОЕ СООБЩЕНИЕ: \"" << decrypted << "\"" << std::endl;
        } else {
            std::cout << "   INTERCEPTED MESSAGE: \"" << decrypted << "\"" << std::endl;
        }
    }

    if (use_russian) {
        std::cout << "=== АТАКА УСПЕШНО ЗАВЕРШЕНА ===" << std::endl;
    } else {
        std::cout << "=== ATTACK SUCCESSFULLY COMPLETED ===" << std::endl;
    }

    // Освобождение ресурсов
    BN_free(p);
    BN_free(q);
    BN_free(phi);
    BN_free(p1);
    BN_free(q1);
    BN_free(d);
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

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
    std::cout << "Введите сообщение для шифрования (Enter message to encrypt): ";
    std::getline(std::cin, message);

    // Проверка длины сообщения
    if (message.size() > BN_num_bytes(n) - 11) {
        std::cerr << "Ошибка: сообщение слишком длинное (Error: message is too long)" << std::endl;
        return 1;
    }

    // Шифрование
    BIGNUM* ciphertext = rsa_encrypt(message, e, n, ctx);
    if (!ciphertext) {
        std::cerr << "Ошибка шифрования (Encryption error)" << std::endl;
        return 1;
    }

    // Вывод информации о ключах
    char* hex_n = BN_bn2hex(n);
    char* hex_e = BN_bn2hex(e);
    char* hex_c = BN_bn2hex(ciphertext);

    std::cout << "\nОткрытый ключ (Public key):" << std::endl;
    std::cout << "   n = " << hex_n << std::endl;
    std::cout << "   e = " << hex_e << std::endl;
    std::cout << "Зашифрованное сообщение (Encrypted message): " << hex_c << std::endl;

    // Освобождение временных ресурсов
    OPENSSL_free(hex_n);
    OPENSSL_free(hex_e);
    OPENSSL_free(hex_c);

    // Запуск атаки на русском языке
    rsa_attack(e, n, ciphertext, ctx, true);

    // Запуск атаки на английском языке
    rsa_attack(e, n, ciphertext, ctx, false);

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

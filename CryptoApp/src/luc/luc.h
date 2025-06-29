#ifndef LUC_H
#define LUC_H

#include <cstdint>
#include <vector>
#include <string>

// Объявление недостающих функций
int64_t modInverse(int64_t a, int64_t m);
int64_t gcd(int64_t a, int64_t b);
int64_t lcm(int64_t a, int64_t b);
int legendreSymbol(int64_t a, int64_t p);

struct LUCKeys {
    int64_t publicKey;
    int64_t privateKey;
    int64_t n;
};

LUCKeys generateLUCKeys();
std::vector<int64_t> encryptMessageLUC(const std::string& message, int64_t e, int64_t n);
std::string decryptMessageLUC(const std::vector<int64_t>& encrypted, int64_t d, int64_t n);

#endif
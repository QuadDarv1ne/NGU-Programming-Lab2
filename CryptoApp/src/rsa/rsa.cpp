#include "rsa.h"
#include <random>
#include <cmath>

int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int64_t modInverse(int64_t a, int64_t m) {
    int64_t m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        int64_t q = a / m;
        int64_t t = m;
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

RSAKeys generateRSAKeys() {
    RSAKeys keys;
    int64_t p = 61;
    int64_t q = 53;
    keys.n = p * q;
    int64_t phi = (p - 1) * (q - 1);
    int64_t e = 17;
    keys.publicKey = e;
    keys.privateKey = modInverse(e, phi);
    return keys;
}

std::vector<int64_t> encryptMessageRSA(const std::string& message, int64_t e, int64_t n) {
    std::vector<int64_t> encrypted;
    for (char c : message) {
        int64_t m = static_cast<int64_t>(c);
        int64_t encryptedChar = 1;
        for (int64_t i = 0; i < e; ++i) {
            encryptedChar = (encryptedChar * m) % n;
        }
        encrypted.push_back(encryptedChar);
    }
    return encrypted;
}

std::string decryptMessageRSA(const std::vector<int64_t>& encrypted, int64_t d, int64_t n) {
    std::string decrypted;
    for (int64_t num : encrypted) {
        int64_t decryptedChar = 1;
        for (int64_t i = 0; i < d; ++i) {
            decryptedChar = (decryptedChar * num) % n;
        }
        decrypted += static_cast<char>(decryptedChar);
    }
    return decrypted;
}

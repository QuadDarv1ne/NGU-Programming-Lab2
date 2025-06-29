#ifndef RSA_H
#define RSA_H

#include <vector>
#include <string>
#include <cstdint>

struct RSAKeys {
    int64_t publicKey;
    int64_t privateKey;
    int64_t n;
};

RSAKeys generateRSAKeys();
std::vector<int64_t> encryptMessageRSA(const std::string& message, int64_t e, int64_t n);
std::string decryptMessageRSA(const std::vector<int64_t>& encrypted, int64_t d, int64_t n);

#endif // RSA_H

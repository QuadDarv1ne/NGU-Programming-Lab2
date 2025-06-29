#ifndef CAST128_H
#define CAST128_H

#include <vector>
#include <string>
#include <array>
#include <cstdint>

constexpr size_t BLOCK_SIZE = 8;
constexpr size_t ROUNDS = 16;

struct CastKey {
    std::array<uint32_t, ROUNDS> Km;
    std::array<uint8_t, ROUNDS> Kr;
};

CastKey generateCastKey(const std::string& password);
void encryptBlock(std::array<uint8_t, BLOCK_SIZE>& block, const CastKey& key);
void decryptBlock(std::array<uint8_t, BLOCK_SIZE>& block, const CastKey& key);
std::vector<uint8_t> encryptMessage(const std::string& message, const CastKey& key);
std::string decryptMessage(const std::vector<uint8_t>& encrypted, const CastKey& key);

#endif // CAST128_H

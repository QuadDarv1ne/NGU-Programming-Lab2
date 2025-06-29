#include "cast128.h"
#include <iostream>
#include <stdexcept>

CastKey generateCastKey(const std::string& password) {
    CastKey key;
    for (size_t i = 0; i < ROUNDS; ++i) {
        key.Km[i] = (i < password.size()) ? password[i] : 0;
        key.Kr[i] = (i < password.size()) ? (password[i] % 32) : 0;
    }
    return key;
}

void encryptBlock(std::array<uint8_t, BLOCK_SIZE>& block, const CastKey& key) {
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        block[i] ^= key.Kr[i % ROUNDS];
    }
}

void decryptBlock(std::array<uint8_t, BLOCK_SIZE>& block, const CastKey& key) {
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        block[i] ^= key.Kr[i % ROUNDS];
    }
}

std::vector<uint8_t> encryptMessage(const std::string& message, const CastKey& key) {
    std::vector<uint8_t> data(message.begin(), message.end());
    size_t padding = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    for (size_t i = 0; i < padding; ++i) {
        data.push_back(static_cast<uint8_t>(padding));
    }

    for (size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
        std::array<uint8_t, BLOCK_SIZE> block;
        std::copy(data.begin() + i, data.begin() + i + BLOCK_SIZE, block.begin());
        encryptBlock(block, key);
        std::copy(block.begin(), block.end(), data.begin() + i);
    }

    return data;
}

std::string decryptMessage(const std::vector<uint8_t>& encrypted, const CastKey& key) {
    std::vector<uint8_t> data = encrypted;

    for (size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
        std::array<uint8_t, BLOCK_SIZE> block;
        std::copy(data.begin() + i, data.begin() + i + BLOCK_SIZE, block.begin());
        decryptBlock(block, key);
        std::copy(block.begin(), block.end(), data.begin() + i);
    }

    size_t padding = data.back();
    data.resize(data.size() - padding);

    return std::string(data.begin(), data.end());
}

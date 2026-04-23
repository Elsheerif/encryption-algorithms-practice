#pragma once
#include <cstdint>
#include <string>
#include <array>

class DES
{
public:
    explicit DES(uint64_t key64);

    uint64_t encrypt(uint64_t plaintext, bool verbose = false) const;

    uint64_t decrypt(uint64_t ciphertext, bool verbose = false) const;

    const std::array<uint64_t, 16> &getRoundKeys() const { return roundKeys; }

private:
    std::array<uint64_t, 16> roundKeys;

    uint64_t applyPermutation(uint64_t input, const int *table,
                              int outBits, int inputBits) const;
    uint64_t feistel(uint64_t R, uint64_t subkey) const;
    uint64_t process(uint64_t block, bool encryptMode, bool verbose) const;

    void generateRoundKeys(uint64_t key64);

    static uint64_t getBit(uint64_t val, int bit, int totalBits);
    static uint64_t circularLeftShift28(uint64_t val, int shifts);

    static const int IP[64];
    static const int FP[64];
    static const int PC1[56];
    static const int PC2[48];
    static const int E[48];
    static const int P[32];
    static const int SHIFTS[16];
    static const int SBOX[8][4][16];
};
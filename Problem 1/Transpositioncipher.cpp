#include "TranspositionCipher.h"
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>


std::vector<int> TranspositionCipher::parseKey(const std::string& key) {
    std::vector<int> perm;
    std::istringstream ss(key);
    int val;
    while (ss >> val) perm.push_back(val);
    return perm;
}


bool TranspositionCipher::isValidKey(const std::string& key) {
    auto perm = parseKey(key);
    if (perm.empty()) return false;

    int n = static_cast<int>(perm.size());
    std::vector<bool> seen(n, false);
    for (int v : perm) {
        if (v < 0 || v >= n) return false;
        if (seen[v])         return false;
        seen[v] = true;
    }
    return true;
}


TranspositionCipher::TranspositionCipher(const std::string& key) {
    if (!isValidKey(key))
        throw std::invalid_argument(
            "Transposition key must be a space-separated permutation of "
            "0..(N-1), e.g. \"2 0 1\".");

    order   = parseKey(key);
    numCols = static_cast<int>(order.size());
}


std::string TranspositionCipher::encrypt(const std::string& text) const {
    int len      = static_cast<int>(text.size());
    int numRows  = (len + numCols - 1) / numCols;
    int gridSize = numRows * numCols;

    std::string padded = text;
    padded.resize(gridSize, PAD_CHAR);
    std::string ciphertext;
    ciphertext.reserve(gridSize);

    for (int col : order) {
        for (int row = 0; row < numRows; ++row) {
            ciphertext += padded[row * numCols + col];
        }
    }
    return ciphertext;
}


std::string TranspositionCipher::decrypt(const std::string& ciphertext) const {
    int len     = static_cast<int>(ciphertext.size());
    int numRows = len / numCols;

    if (len % numCols != 0)
        throw std::invalid_argument(
            "Ciphertext length is not a multiple of the key length. "
            "Was it encrypted with this key?");


    std::vector<int> invOrder(numCols);
    for (int i = 0; i < numCols; ++i)
        invOrder[order[i]] = i;


    std::vector<std::vector<char>> grid(numRows, std::vector<char>(numCols));

    int pos = 0;
    for (int col : order) {
        for (int row = 0; row < numRows; ++row) {
            grid[row][col] = ciphertext[pos++];
        }
    }


    std::string plaintext;
    plaintext.reserve(len);
    for (int row = 0; row < numRows; ++row)
        for (int col = 0; col < numCols; ++col)
            plaintext += grid[row][col];


    while (!plaintext.empty() && plaintext.back() == PAD_CHAR)
        plaintext.pop_back();

    return plaintext;
}
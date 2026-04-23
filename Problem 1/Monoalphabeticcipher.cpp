#include "MonoalphabeticCipher.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>


MonoalphabeticCipher::MonoalphabeticCipher(const std::string& key) {
    if (!isValidKey(key))
        throw std::invalid_argument(
            "Monoalphabetic key must be exactly 26 unique lowercase letters.");
    buildTables(key);
}


void MonoalphabeticCipher::buildTables(const std::string& key) {
    for (int i = 0; i < 26; ++i) {
        encryptTable[i] = key[i];
        decryptTable[key[i] - 'a'] = 'a' + i;
    }
}


std::string MonoalphabeticCipher::encrypt(const std::string& text) const {
    std::string result = text;
    for (char& c : result) {
        if (std::isalpha(c)) {
            bool upper = std::isupper(c);
            char base  = static_cast<char>(std::tolower(c));
            char enc   = encryptTable[base - 'a'];
            result[&c - &result[0]] = upper ? static_cast<char>(std::toupper(enc)) : enc;
        }
    }
    return result;
}


std::string MonoalphabeticCipher::decrypt(const std::string& text) const {
    std::string result = text;
    for (char& c : result) {
        if (std::isalpha(c)) {
            bool upper = std::isupper(c);
            char base  = static_cast<char>(std::tolower(c));
            char dec   = decryptTable[base - 'a'];
            result[&c - &result[0]] = upper ? static_cast<char>(std::toupper(dec)) : dec;
        }
    }
    return result;
}


bool MonoalphabeticCipher::isValidKey(const std::string& key) {
    if (key.size() != 26) return false;

    bool seen[26] = {};
    for (char c : key) {
        if (!std::islower(c)) return false;
        int idx = c - 'a';
        if (seen[idx])  return false;
        seen[idx] = true;
    }
    return true;
}
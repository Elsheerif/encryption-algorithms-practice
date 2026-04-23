#include "PolyalphabeticCipher.h"
#include <stdexcept>
#include <cctype>
#include <algorithm>


PolyalphabeticCipher::PolyalphabeticCipher(const std::string& keyword) {
    if (!isValidKey(keyword))
        throw std::invalid_argument(
            "Polyalphabetic key must be a non-empty alphabetic string.");


    key = keyword;
    std::transform(key.begin(), key.end(), key.begin(),
                   [](unsigned char c){ return std::tolower(c); });
}


bool PolyalphabeticCipher::isValidKey(const std::string& keyword) {
    if (keyword.empty()) return false;
    for (char c : keyword)
        if (!std::isalpha(c)) return false;
    return true;
}


std::string PolyalphabeticCipher::process(const std::string& text, bool encode) const {
    std::string result;
    result.reserve(text.size());

    int keyLen = static_cast<int>(key.size());
    int keyIdx = 0;

    for (char c : text) {
        if (std::isalpha(c)) {
            bool upper = std::isupper(c);
            int plain  = std::tolower(c) - 'a';
            int shift  = key[keyIdx % keyLen] - 'a';

            int cipher = encode
                ? (plain + shift) % 26
                : (plain - shift + 26) % 26;

            char out = static_cast<char>('a' + cipher);
            result += upper ? static_cast<char>(std::toupper(out)) : out;
            ++keyIdx;
        } else {
            result += c;
        }
    }
    return result;
}


std::string PolyalphabeticCipher::encrypt(const std::string& text) const {
    return process(text, true);
}

std::string PolyalphabeticCipher::decrypt(const std::string& text) const {
    return process(text, false);
}
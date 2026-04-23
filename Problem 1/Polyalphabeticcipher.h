#pragma once
#include <string>


class PolyalphabeticCipher {
public:
    explicit PolyalphabeticCipher(const std::string& keyword);

    std::string encrypt(const std::string& text) const;
    std::string decrypt(const std::string& text) const;

    static bool isValidKey(const std::string& keyword);

private:
    std::string key;


    std::string process(const std::string& text, bool encode) const;
};
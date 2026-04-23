#pragma once
#include <string>


class MonoalphabeticCipher {
public:

    explicit MonoalphabeticCipher(const std::string& key);


    std::string encrypt(const std::string& text) const;


    std::string decrypt(const std::string& text) const;


    static bool isValidKey(const std::string& key);

private:
    char encryptTable[26];
    char decryptTable[26];

    void buildTables(const std::string& key);
};
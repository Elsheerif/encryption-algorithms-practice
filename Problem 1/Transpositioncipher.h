#pragma once
#include <string>
#include <vector>


class TranspositionCipher {
public:

    explicit TranspositionCipher(const std::string& key);

    std::string encrypt(const std::string& text) const;
    std::string decrypt(const std::string& text) const;


    static std::vector<int> parseKey(const std::string& key);
    static bool isValidKey(const std::string& key);


    static constexpr char PAD_CHAR = 'X';

private:
    std::vector<int> order;
    int numCols;
};
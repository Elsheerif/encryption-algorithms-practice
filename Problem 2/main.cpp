#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <sstream>
#include "DES.h"

static uint64_t parseHex64(const std::string &hex)
{
    if (hex.size() != 16)
        throw std::invalid_argument("Hex value must be exactly 16 hex characters (64 bits).");

    for (char c : hex)
        if (!std::isxdigit(static_cast<unsigned char>(c)))
            throw std::invalid_argument("Invalid hex character: " + std::string(1, c));

    uint64_t val = 0;
    std::istringstream ss(hex);
    ss >> std::hex >> val;
    return val;
}

static void printBinary64(uint64_t val)
{
    for (int i = 63; i >= 0; --i)
    {
        std::cout << ((val >> i) & 1);
        if (i % 8 == 0 && i > 0)
            std::cout << ' ';
    }
}

static void printRoundKeys(const DES &des)
{
    const auto &keys = des.getRoundKeys();
    std::cout << "\n+-------------------------------------+\n";
    std::cout << "|        Round Sub-Key Schedule        |\n";
    std::cout << "+--------+----------------------------+\n";
    std::cout << "| Round  | Sub-Key (48-bit hex)       |\n";
    std::cout << "+--------+----------------------------+\n";
    for (int i = 0; i < 16; ++i)
    {
        std::cout << "|  K" << std::setw(2) << std::setfill('0') << (i + 1)
                  << std::setfill(' ') << "   | 0x"
                  << std::hex << std::uppercase
                  << std::setfill('0') << std::setw(12) << keys[i]
                  << std::setfill(' ') << std::dec
                  << "                |\n";
    }
    std::cout << "+--------+----------------------------+\n";
}

static void runKnownAnswerTest()
{
    uint64_t key = 0x133457799BBCDFF1ULL;
    uint64_t plain = 0x0123456789ABCDEFULL;
    uint64_t expect = 0x85E813540F0AB405ULL;

    std::cout << "\n========================================\n";
    std::cout << "  Known-Answer Test (Stallings textbook)\n";
    std::cout << "========================================\n";
    std::cout << "  Key       : 0x133457799BBCDFF1\n";
    std::cout << "  Plaintext : 0x0123456789ABCDEF\n";
    std::cout << "  Expected  : 0x85E813540F0AB405\n";

    DES des(key);
    uint64_t cipher = des.encrypt(plain);
    std::cout << "  Computed  : 0x" << std::hex << std::uppercase
              << std::setfill('0') << std::setw(16) << cipher << std::dec << "\n";

    if (cipher == expect)
        std::cout << "  Result    : PASS\n";
    else
        std::cout << "  Result    : FAIL\n";

    uint64_t decrypted = des.decrypt(cipher);
    std::cout << "  Decrypted : 0x" << std::hex << std::uppercase
              << std::setfill('0') << std::setw(16) << decrypted << std::dec;
    std::cout << (decrypted == plain ? "  matches original\n" : "  MISMATCH!\n");
    std::cout << "========================================\n";
}

int main()
{
    std::cout << "\n";
    std::cout << "+=================================+\n";
    std::cout << "|    DES Encryption Tool  (Problem 2)      |\n";
    std::cout << "+=================================+\n";

    runKnownAnswerTest();

    std::cout << "\nEnter your own values (or press Enter to use NIST vector):\n";

    std::string input;
    bool runAgain = true;
    while (runAgain)
    {
        uint64_t key = 0x133457799BBCDFF1ULL;
        std::cout << "\n64-bit key as 16 hex chars (e.g. 133457799BBCDFF1): ";
        std::getline(std::cin, input);
        if (!input.empty())
        {
            try
            {
                key = parseHex64(input);
            }
            catch (const std::exception &e)
            {
                std::cerr << "[ERROR] " << e.what() << "\n";
                continue;
            }
        }
        else
        {
            std::cout << "(Using Stallings key 133457799BBCDFF1)\n";
        }

        uint64_t plaintext = 0x0123456789ABCDEFULL;
        std::cout << "64-bit plaintext as 16 hex chars (e.g. 0123456789ABCDEF): ";
        std::getline(std::cin, input);
        if (!input.empty())
        {
            try
            {
                plaintext = parseHex64(input);
            }
            catch (const std::exception &e)
            {
                std::cerr << "[ERROR] " << e.what() << "\n";
                continue;
            }
        }
        else
        {
            std::cout << "(Using NIST plaintext 0123456789ABCDEF)\n";
        }

        DES des(key);

        printRoundKeys(des);

        std::cout << "\nPlaintext in binary:\n";
        printBinary64(plaintext);
        std::cout << "\n";

        uint64_t ciphertext = des.encrypt(plaintext, /*verbose=*/true);

        uint64_t recovered = des.decrypt(ciphertext, /*verbose=*/true);

        std::cout << "+----------------------------------------------+\n";
        std::cout << "|                       Summary                        |\n";
        std::cout << "+--------------------+---------------------------------+\n";
        std::cout << "| Key                | 0x" << std::hex << std::uppercase
                  << std::setfill('0') << std::setw(16) << key
                  << std::setfill(' ') << std::dec << "          |\n";
        std::cout << "| Plaintext          | 0x" << std::hex << std::uppercase
                  << std::setfill('0') << std::setw(16) << plaintext
                  << std::setfill(' ') << std::dec << "          |\n";
        std::cout << "| Ciphertext         | 0x" << std::hex << std::uppercase
                  << std::setfill('0') << std::setw(16) << ciphertext
                  << std::setfill(' ') << std::dec << "          |\n";
        std::cout << "| Decrypted          | 0x" << std::hex << std::uppercase
                  << std::setfill('0') << std::setw(16) << recovered
                  << std::setfill(' ') << std::dec << "          |\n";
        std::cout << "| Round-trip match   | "
                  << (recovered == plaintext ? "YES" : "NO")
                  << "                              |\n";
        std::cout << "+--------------------+---------------------------------+\n";

        std::cout << "\nRun again? (y/n): ";
        std::getline(std::cin, input);
        if (input != "y" && input != "Y")
            runAgain = false;
    }

    return 0;
}


#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "MonoalphabeticCipher.h"
#include "PolyalphabeticCipher.h"
#include "TranspositionCipher.h"

static std::string readFile(const std::string &path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw std::runtime_error("Cannot open input file: " + path);

    return std::string(std::istreambuf_iterator<char>(in),
                    std::istreambuf_iterator<char>());
}

static void writeFile(const std::string &path, const std::string &content)
{
    std::ofstream out(path);
    if (!out.is_open())
        throw std::runtime_error("Cannot open output file: " + path);
    out << content;
}

static void printBanner()
{
    std::cout << "\n";
    std::cout << "+=================================+\n";
    std::cout << "|      Classical Encryption Tool           |\n";
    std::cout << "+=================================+\n\n";
}

static void printCipherMenu()
{
    std::cout << "Select cipher type:\n";
    std::cout << "  1. Monoalphabetic Substitution Cipher\n";
    std::cout << "  2. Polyalphabetic (Vigenère) Cipher\n";
    std::cout << "  3. Transposition (Columnar) Cipher\n";
    std::cout << "Choice (1-3): " << std::flush;
}

static void printModeMenu()
{
    std::cout << "\nSelect mode:\n";
    std::cout << "  1. Encrypt\n";
    std::cout << "  2. Decrypt\n";
    std::cout << "Choice (1-2): " << std::flush;
}

static std::string getMonoKey()
{
    std::string key;
    while (true)
    {
        std::cout << "\nEnter 26-letter substitution key "
                    "(all lowercase, all unique, e.g. qwertyuiopasdfghjklzxcvbnm):\n> ";
        std::cin >> key;
        if (MonoalphabeticCipher::isValidKey(key))
            break;
        std::cerr << "  [!] Invalid key — must be exactly 26 unique lowercase letters.\n";
    }
    return key;
}

static std::string getPolyKey()
{
    std::string key;
    while (true)
    {
        std::cout << "\nEnter keyword (letters only, e.g. SECRET):\n> " << std::flush;
        std::cin >> key;
        if (PolyalphabeticCipher::isValidKey(key))
            break;
        std::cerr << "  [!] Invalid keyword — must be non-empty and contain only letters.\n";
    }
    return key;
}

static std::string getTransKey()
{
    std::string key;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true)
    {
        std::cout << "\nEnter column permutation key\n"
                     "  (space-separated 0-based indices, e.g. \"2 0 1\" for 3 columns):\n> ";
        std::getline(std::cin, key);
        if (TranspositionCipher::isValidKey(key))
            break;
        std::cerr << "  [!] Invalid key — must be a permutation of 0..(N-1).\n";
    }
    return key;
}

int main()
{
    FILE *log = fopen("C:\\Users\\AUSU\\OneDrive\\Desktop\\20226016\\debug.log", "a");
    if (log)
    {
        fprintf(log, "=== Program execution started ===\n");
        fprintf(log, "DEBUG: main() called\n");
        fflush(log);
    }

    fprintf(stderr, "ERROR OUTPUT TEST\n");
    fflush(stderr);

    printf("STANDARD OUTPUT TEST\n");
    fflush(stdout);

    printBanner();
    if (log)
        fprintf(log, "DEBUG: Banner printed\n");
    if (log)
        fflush(log);

    std::string input;
    bool runAgain = true;
    while (runAgain)
    {
        printCipherMenu();
        fprintf(stderr, "DEBUG: Cipher menu printed, waiting for input\n");
        fflush(stderr);
        int cipherChoice = 0;
        std::cin >> cipherChoice;
        fprintf(stderr, "DEBUG: Cipher choice: %d\n", cipherChoice);
        fflush(stderr);

        printModeMenu();
        int modeChoice = 0;
        std::cin >> modeChoice;
        bool encrypt = (modeChoice == 1);

        std::string inputPath, outputPath;

        std::cout << "\nInput file path: " << std::flush;
        std::cin >> inputPath;
        std::cout << "Output file path: " << std::flush;
        std::cin >> outputPath;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string text;
        try
        {
            text = readFile(inputPath);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR] " << e.what() << "\n";
            continue;
        }

        std::cout << "\n--- Input text (" << text.size() << " chars) ---\n"
                  << text << "\n";

        std::string result;
        try
        {
            switch (cipherChoice)
            {

            case 1:
            {
                std::string key = getMonoKey();
                MonoalphabeticCipher cipher(key);

                std::cout << "\n[Monoalphabetic] Key: " << key << "\n";
                result = encrypt ? cipher.encrypt(text) : cipher.decrypt(text);
                break;
            }

            case 2:
            {
                std::string key = getPolyKey();
                PolyalphabeticCipher cipher(key);

                std::cout << "\n[Vigenère] Keyword: " << key << "\n";
                result = encrypt ? cipher.encrypt(text) : cipher.decrypt(text);
                break;
            }

            case 3:
            {
                std::string key = getTransKey();
                TranspositionCipher cipher(key);

                std::cout << "\n[Transposition] Column order: " << key << "\n";
                result = encrypt ? cipher.encrypt(text) : cipher.decrypt(text);
                break;
            }

            default:
                std::cerr << "[ERROR] Invalid cipher choice.\n";
                continue;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR] " << e.what() << "\n";
            continue;
        }

        try
        {
            writeFile(outputPath, result);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR] " << e.what() << "\n";
            continue;
        }

        std::cout << "\n--- Output text (" << result.size() << " chars) ---\n"
                  << result << "\n";
        std::cout << "\n[OK] Result written to: " << outputPath << "\n";

        std::cout << "\nRun again? (y/n): ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        runAgain = (input == "y" || input == "Y");
        std::cout << "[DEBUG] Your input was: '" << input << "' (length: " << input.length() << ")\n";
    }

    return 0;
}
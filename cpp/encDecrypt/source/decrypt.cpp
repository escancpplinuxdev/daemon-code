
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "decrypt.h"

#define SALT_SIZE 16
#define IV_SIZE 16
#define KEY_SIZE 32
#define ITERATIONS 10000

void handleOpenSSLError()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}

bool deriveKeyIV(const std::string& password, const unsigned char* salt,
                 unsigned char* key, unsigned char* iv)
{
    unsigned char combined[KEY_SIZE + IV_SIZE];
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.size(),
                          salt, SALT_SIZE, ITERATIONS,
                          EVP_sha256(), KEY_SIZE + IV_SIZE,
                          combined) != 1)
    {
        return false;
    }
    memcpy(key, combined, KEY_SIZE);
    memcpy(iv, combined + KEY_SIZE, IV_SIZE);
    return true;
}

bool decryptFile(const std::string& inFile, const std::string& outFile,
                 const std::string& password)
{
    // 1. Read salt, IV, ciphertext
    std::ifstream fin(inFile, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Cannot open input file: " << inFile << std::endl;
        return false;
    }
    unsigned char salt[SALT_SIZE];
    unsigned char iv[IV_SIZE];
    fin.read(reinterpret_cast<char*>(salt), SALT_SIZE);
    fin.read(reinterpret_cast<char*>(iv), IV_SIZE);
    if (!fin)
    {
        std::cerr << "Failed to read salt/IV" << std::endl;
        return false;
    }
    std::vector<unsigned char> ciphertext((std::istreambuf_iterator<char>(fin)),
                                          std::istreambuf_iterator<char>());
    fin.close();

    // 2. Derive key
    unsigned char key[KEY_SIZE];
    if (!deriveKeyIV(password, salt, key, iv))
    {
        std::cerr << "Key derivation failed" << std::endl;
        return false;
    }

    // 3. Decrypt
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create cipher context" << std::endl;
        return false;
    }
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
    {
        std::cerr << "Decryption init failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::vector<unsigned char> plaintext(ciphertext.size() + EVP_CIPHER_CTX_block_size(ctx));
    int len = 0;
    int totalLen = 0;

    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len,
                          ciphertext.data(), ciphertext.size()) != 1)
    {
        std::cerr << "Decryption update failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    totalLen += len;

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + totalLen, &len) != 1)
    {
        std::cerr << "Decryption final failed (wrong password?)" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    totalLen += len;
    plaintext.resize(totalLen);
    EVP_CIPHER_CTX_free(ctx);

    // 4. Write output
    std::ofstream fout(outFile, std::ios::binary);
    if (!fout)
    {
        std::cerr << "Cannot open output file: " << outFile << std::endl;
        return false;
    }
    fout.write(reinterpret_cast<char*>(plaintext.data()), plaintext.size());
    fout.close();

    std::cout << "Decryption successful: " << outFile << std::endl;
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <password>" << std::endl;
        return 1;
    }
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    if (!decryptFile(argv[1], argv[2], argv[3]))
    {
        std::cerr << "Decryption failed." << std::endl;
        return 1;
    }
    return 0;
}

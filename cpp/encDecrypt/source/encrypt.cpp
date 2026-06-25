
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#include "encrypt.h"

#define SALT_SIZE 16
#define IV_SIZE 16
#define KEY_SIZE 32      // AES-256
#define ITERATIONS 10000

// Helper: handle OpenSSL errors
void handleOpenSSLError()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}

// Derive key and IV from password using PBKDF2
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

// Encrypt file
bool encryptFile(const std::string& inFile, const std::string& outFile,
                 const std::string& password)
{
    // 1. Read plaintext
    std::ifstream fin(inFile, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Cannot open input file: " << inFile << std::endl;
        return false;
    }
    std::vector<unsigned char> plaintext((std::istreambuf_iterator<char>(fin)),
                                         std::istreambuf_iterator<char>());
    fin.close();

    // 2. Generate salt and IV
    unsigned char salt[SALT_SIZE];
    unsigned char iv[IV_SIZE];
    if (RAND_bytes(salt, sizeof(salt)) != 1)
    {
        std::cerr << "Failed to generate salt" << std::endl;
        return false;
    }
    if (RAND_bytes(iv, sizeof(iv)) != 1)
    {
        std::cerr << "Failed to generate IV" << std::endl;
        return false;
    }

    // 3. Derive key
    unsigned char key[KEY_SIZE];
    if (!deriveKeyIV(password, salt, key, iv))
    {
        std::cerr << "Key derivation failed" << std::endl;
        return false;
    }

    // 4. Encrypt
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create cipher context" << std::endl;
        return false;
    }
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
    {
        std::cerr << "Encryption init failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_CTX_block_size(ctx));
    int len = 0;
    int totalLen = 0;

    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                          plaintext.data(), plaintext.size()) != 1)
    {
        std::cerr << "Encryption update failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    totalLen += len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + totalLen, &len) != 1)
    {
        std::cerr << "Encryption final failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    totalLen += len;
    ciphertext.resize(totalLen);
    EVP_CIPHER_CTX_free(ctx);

    // 5. Write output: salt + IV + ciphertext
    std::ofstream fout(outFile, std::ios::binary);
    if (!fout)
    {
        std::cerr << "Cannot open output file: " << outFile << std::endl;
        return false;
    }
    fout.write(reinterpret_cast<char*>(salt), SALT_SIZE);
    fout.write(reinterpret_cast<char*>(iv), IV_SIZE);
    fout.write(reinterpret_cast<char*>(ciphertext.data()), ciphertext.size());
    fout.close();

    std::cout << "Encryption successful: " << outFile << std::endl;
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

    if (!encryptFile(argv[1], argv[2], argv[3]))
    {
        std::cerr << "Encryption failed." << std::endl;
        return 1;
    }
    return 0;
}

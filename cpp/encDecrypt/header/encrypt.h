#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string>

/**
 * Encrypts a file using AES-256-CBC with a password-derived key.
 * @param inFile   Path to the input (plaintext) file.
 * @param outFile  Path to the output (ciphertext) file.
 * @param password The password used to derive the encryption key.
 * @return true on success, false on failure.
 */
bool encryptFile(const std::string& inFile, const std::string& outFile, const std::string& password);

#endif

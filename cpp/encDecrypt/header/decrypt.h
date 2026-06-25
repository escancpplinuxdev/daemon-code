#ifndef DECRYPT_H
#define DECRYPT_H

#include <string>

/**
 * Decrypts a file that was encrypted with `encryptFile`.
 * @param inFile   Path to the input (ciphertext) file.
 * @param outFile  Path to the output (plaintext) file.
 * @param password The password used to derive the decryption key.
 * @return true on success, false on failure.
 */
bool decryptFile(const std::string& inFile, const std::string& outFile, const std::string& password);

#endif

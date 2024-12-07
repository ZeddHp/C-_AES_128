#ifndef AES_H
#define AES_H

#include <QString>
#include <vector>
#include <cstdint>

class AES {
public:
    // For AES-128:
    static const int Nk = 4;  // Key length in 32-bit words (128 bits -> 4 words)
    static const int Nb = 4;  // Block size in 32-bit words (AES block is always 128 bits)
    static const int Nr = 10; // Number of rounds for AES-128

    // S-box and inverse S-box
    static const uint8_t sbox[256];
    static const uint8_t rsbox[256];

    // Round constants
    static const uint8_t Rcon[11];

    // Public vectors for IV
    std::vector<uint8_t> IV;

    // Constructors
    AES(const QString &key, const QString &iv = QString());

    // Set IV after construction if needed
    void setIV(const QString &iv);

    // High-level encryption/decryption (operates on QString and returns Base64-encoded ciphertext)
    QString encrypt(const QString &plaintext);
    QString decrypt(const QString &ciphertext);

    // Encryption/Decryption for raw binary data (returns raw encrypted/decrypted bytes)
    std::vector<uint8_t> encryptData(const std::vector<uint8_t> &plaintextData);
    std::vector<uint8_t> decryptData(const std::vector<uint8_t> &ciphertextData);

private:
    std::vector<uint32_t> RoundKey;

    // Key expansion
    void KeyExpansion(const std::vector<uint8_t>& key);

    // AES block encryption/decryption
    void EncryptBlock(std::vector<uint8_t> &block);
    void DecryptBlock(std::vector<uint8_t> &block);

    // AES transformations
    static uint32_t RotWord(uint32_t word);
    static uint32_t SubWord(uint32_t word);
    static uint8_t mul(uint8_t a, uint8_t b);

    void AddRoundKey(int round, std::vector<uint8_t> &state);
    void SubBytes(std::vector<uint8_t> &state);
    void InvSubBytes(std::vector<uint8_t> &state);
    void ShiftRows(std::vector<uint8_t> &state);
    void InvShiftRows(std::vector<uint8_t> &state);
    void MixColumns(std::vector<uint8_t> &state);
    void InvMixColumns(std::vector<uint8_t> &state);

    // Padding methods
    void padData(std::vector<uint8_t> &data);
    void unpadData(std::vector<uint8_t> &data);

    // Block preparation
    std::vector<uint8_t> prepareBlock(const QString &text);
    QString blockToString(const std::vector<uint8_t> &block);
};

#endif // AES_H

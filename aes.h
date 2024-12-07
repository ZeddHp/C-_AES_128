#ifndef AES_H
#define AES_H

#include <QString>
#include <vector>

class AES {
public:
    //explicit AES(const std::vector<uint8_t>& key, bool padding = true);  // Direct key initialization
    //explicit AES(const QString &key);  // Converts QString key to vector

    //NEW
    explicit AES(const QString &key, const QString &iv = QString());  // Constructor now optionally takes an IV


    QString encrypt(const QString &plaintext);
    QString decrypt(const QString &ciphertext);

    //NEW
    void setIV(const QString &iv);

    void setPadding(bool enable);  // Enable/disable padding
    void padData(std::vector<uint8_t>& data);  // Add PKCS#7 padding
    void unpadData(std::vector<uint8_t>& data);  // Remove PKCS#7 padding

private:
    static const int Nb = 4;  // Block size in 32-bit words, always 4 for AES.
    static const int Nk = 4;  // Key size in 32-bit words, 4 for AES-128.
    static const int Nr = 10; // Number of rounds, 10 for AES-128.

    std::vector<uint32_t> RoundKey;  // Expanded round keys
    std::vector<uint8_t> IV;         // Initialization Vector for CBC mode

    bool usePadding;  // Toggle padding on or off

    // AES S-box and Inverse S-box
    static const uint8_t sbox[256];
    static const uint8_t rsbox[256];

    // Round constant word array
    static const uint8_t Rcon[11];

    // Utility functions for AES algorithm
    std::vector<uint8_t> prepareBlock(const QString &text);
    QString blockToString(const std::vector<uint8_t> &block);

    // Key expansion utilities
    void KeyExpansion(const std::vector<uint8_t>& key);
    uint32_t RotWord(uint32_t word);
    uint32_t SubWord(uint32_t word);

    // Core AES transformations
    void AddRoundKey(int round, std::vector<uint8_t>& state);
    void SubBytes(std::vector<uint8_t>& state);
    void InvSubBytes(std::vector<uint8_t>& state);
    void ShiftRows(std::vector<uint8_t>& state);
    void InvShiftRows(std::vector<uint8_t>& state);
    void MixColumns(std::vector<uint8_t>& state);
    void InvMixColumns(std::vector<uint8_t>& state);

    // Helper functions for MixColumns
    static uint8_t mul(uint8_t a, uint8_t b);  // Perform multiplication in GF(2^8)

    // Block encrypt/decrypt
    void EncryptBlock(std::vector<uint8_t>& block);
    void DecryptBlock(std::vector<uint8_t>& block);

    // Initialize IV with random data
    void initIV();
};

#endif // AES_H

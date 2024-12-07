#include "aes.h"
#include <QByteArray>
#include <QDebug>
#include <vector>
#include <random>
#include <stdexcept>

// AES S-box
const uint8_t AES::sbox[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// AES inverse S-box
const uint8_t AES::rsbox[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

// Round constants
const uint8_t AES::Rcon[11] = {
    0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36
};


AES::AES(const QString &key, const QString &iv) {
    QByteArray keyBytes = key.toUtf8();
    std::vector<uint8_t> keyVector(keyBytes.begin(), keyBytes.end());
    KeyExpansion(keyVector);
    if (!iv.isEmpty()) {
        setIV(iv);
    } else {
        // If IV not provided, default to zero IV (not secure, better to generate a random IV)
        IV.resize(16, 0);
    }
}

void AES::setIV(const QString &iv) {
    QByteArray ivBytes = iv.toUtf8();
    IV.clear();
    IV.insert(IV.end(), ivBytes.begin(), ivBytes.end());
    while (IV.size() < 16) {
        IV.push_back(0);
    }
}

QString AES::encrypt(const QString &plaintext) {
    // Convert plaintext to vector
    std::vector<uint8_t> data = prepareBlock(plaintext);
    padData(data);

    std::vector<uint8_t> prevCipherBlock = IV;
    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(data.size());

    for (size_t i = 0; i < data.size(); i += 16) {
        std::vector<uint8_t> tempBlock(data.begin() + i, data.begin() + i + 16);
        for (int j = 0; j < 16; ++j) {
            tempBlock[j] ^= prevCipherBlock[j];
        }
        EncryptBlock(tempBlock);
        prevCipherBlock = tempBlock;
        ciphertext.insert(ciphertext.end(), tempBlock.begin(), tempBlock.end());
    }

    // Prepend IV
    std::vector<uint8_t> output;
    output.insert(output.end(), IV.begin(), IV.end());
    output.insert(output.end(), ciphertext.begin(), ciphertext.end());

    QByteArray ba(reinterpret_cast<const char*>(output.data()), (int)output.size());
    return ba.toBase64();
}

QString AES::decrypt(const QString &ciphertextWithIV) {
    QByteArray decoded = QByteArray::fromBase64(ciphertextWithIV.toUtf8());
    if (decoded.size() < 16) {
        throw std::runtime_error("Invalid ciphertext");
    }
    QByteArray iv = decoded.left(16);
    QByteArray ciphertext = decoded.mid(16);

    std::vector<uint8_t> prevCipherBlock(iv.begin(), iv.end());
    std::vector<uint8_t> plain;
    plain.reserve(ciphertext.size());

    for (int i = 0; i < ciphertext.size(); i += 16) {
        QByteArray tempCipherBlock = ciphertext.mid(i, 16);
        std::vector<uint8_t> tempBlock(tempCipherBlock.begin(), tempCipherBlock.end());
        DecryptBlock(tempBlock);

        for (int j = 0; j < 16; ++j) {
            tempBlock[j] ^= prevCipherBlock[j];
        }

        prevCipherBlock.assign(tempCipherBlock.begin(), tempCipherBlock.end());
        plain.insert(plain.end(), tempBlock.begin(), tempBlock.end());
    }

    unpadData(plain);

    QByteArray plaintextBytes(reinterpret_cast<const char*>(plain.data()), (int)plain.size());
    return QString::fromUtf8(plaintextBytes);
}

std::vector<uint8_t> AES::encryptData(const std::vector<uint8_t> &plaintextData) {
    if (IV.size() != 16) {
        throw std::runtime_error("Invalid IV size");
    }

    std::vector<uint8_t> data = plaintextData;
    padData(data);

    std::vector<uint8_t> prevCipherBlock = IV;
    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(data.size());

    for (size_t i = 0; i < data.size(); i += 16) {
        std::vector<uint8_t> tempBlock(data.begin() + i, data.begin() + i + 16);
        for (int j = 0; j < 16; ++j) {
            tempBlock[j] ^= prevCipherBlock[j];
        }
        EncryptBlock(tempBlock);
        prevCipherBlock = tempBlock;
        ciphertext.insert(ciphertext.end(), tempBlock.begin(), tempBlock.end());
    }

    return ciphertext;
}

std::vector<uint8_t> AES::decryptData(const std::vector<uint8_t> &ciphertextData) {
    if (IV.size() != 16) {
        throw std::runtime_error("Invalid IV size");
    }

    if (ciphertextData.size() % 16 != 0) {
        throw std::runtime_error("Ciphertext length not multiple of 16");
    }

    std::vector<uint8_t> prevCipherBlock = IV;
    std::vector<uint8_t> plain;
    plain.reserve(ciphertextData.size());

    for (size_t i = 0; i < ciphertextData.size(); i += 16) {
        std::vector<uint8_t> tempBlock(ciphertextData.begin() + i, ciphertextData.begin() + i + 16);
        std::vector<uint8_t> blockCopy = tempBlock;

        DecryptBlock(tempBlock);
        for (int j = 0; j < 16; ++j) {
            tempBlock[j] ^= prevCipherBlock[j];
        }

        prevCipherBlock = blockCopy;
        plain.insert(plain.end(), tempBlock.begin(), tempBlock.end());
    }

    unpadData(plain);
    return plain;
}

std::vector<uint8_t> AES::prepareBlock(const QString &text) {
    QByteArray bytes = text.toUtf8();
    return std::vector<uint8_t>(bytes.begin(), bytes.end());
}

QString AES::blockToString(const std::vector<uint8_t> &block) {
    QByteArray bytes(reinterpret_cast<const char*>(block.data()), (int)block.size());
    return QString::fromUtf8(bytes);
}

uint32_t AES::RotWord(uint32_t word) {
    return (word << 8) | (word >> 24);
}

uint32_t AES::SubWord(uint32_t word) {
    return (sbox[(word >> 24) & 0xFF] << 24)
    | (sbox[(word >> 16) & 0xFF] << 16)
        | (sbox[(word >> 8) & 0xFF] << 8)
        | sbox[word & 0xFF];
}

uint8_t AES::mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void AES::KeyExpansion(const std::vector<uint8_t> &key) {
    RoundKey.resize(Nb * (Nr + 1));
    uint32_t temp;
    int i = 0;

    for (i = 0; i < Nk; i++) {
        RoundKey[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | key[4 * i + 3];
    }

    for (; i < Nb * (Nr + 1); i++) {
        temp = RoundKey[i - 1];
        if (i % Nk == 0) {
            temp = SubWord(RotWord(temp)) ^ (Rcon[i / Nk] << 24);
        }
        RoundKey[i] = RoundKey[i - Nk] ^ temp;
    }
}

void AES::EncryptBlock(std::vector<uint8_t> &block) {
    AddRoundKey(0, block);
    for (int round = 1; round < Nr; round++) {
        SubBytes(block);
        ShiftRows(block);
        MixColumns(block);
        AddRoundKey(round, block);
    }
    SubBytes(block);
    ShiftRows(block);
    AddRoundKey(Nr, block);
}

void AES::DecryptBlock(std::vector<uint8_t> &block) {
    AddRoundKey(Nr, block);
    for (int round = Nr - 1; round > 0; round--) {
        InvShiftRows(block);
        InvSubBytes(block);
        AddRoundKey(round, block);
        InvMixColumns(block);
    }
    InvShiftRows(block);
    InvSubBytes(block);
    AddRoundKey(0, block);
}

void AES::AddRoundKey(int round, std::vector<uint8_t>& state) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= (RoundKey[round * Nb + i / 4] >> (8 * (3 - (i % 4)))) & 0xFF;
    }
}

void AES::SubBytes(std::vector<uint8_t>& state) {
    for (int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
    }
}

void AES::InvSubBytes(std::vector<uint8_t>& state) {
    for (int i = 0; i < 16; i++) {
        state[i] = rsbox[state[i]];
    }
}

void AES::ShiftRows(std::vector<uint8_t>& state) {
    std::vector<uint8_t> temp(16);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            temp[r + 4 * c] = state[r + 4 * ((c + r) % 4)];
        }
    }
    state = temp;
}

void AES::InvShiftRows(std::vector<uint8_t>& state) {
    std::vector<uint8_t> temp(16);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            temp[r + 4 * c] = state[r + 4 * ((c - r + 4) % 4)];
        }
    }
    state = temp;
}

void AES::MixColumns(std::vector<uint8_t>& state) {
    uint8_t temp[4];
    for (int i = 0; i < 4; i++) {
        temp[0] = (uint8_t)(mul(0x02, state[i * 4 + 0]) ^ mul(0x03, state[i * 4 + 1]) ^ state[i * 4 + 2] ^ state[i * 4 + 3]);
        temp[1] = (uint8_t)(state[i * 4 + 0] ^ mul(0x02, state[i * 4 + 1]) ^ mul(0x03, state[i * 4 + 2]) ^ state[i * 4 + 3]);
        temp[2] = (uint8_t)(state[i * 4 + 0] ^ state[i * 4 + 1] ^ mul(0x02, state[i * 4 + 2]) ^ mul(0x03, state[i * 4 + 3]));
        temp[3] = (uint8_t)(mul(0x03, state[i * 4 + 0]) ^ state[i * 4 + 1] ^ state[i * 4 + 2] ^ mul(0x02, state[i * 4 + 3]));
        for (int j = 0; j < 4; j++) {
            state[i * 4 + j] = temp[j];
        }
    }
}

void AES::InvMixColumns(std::vector<uint8_t>& state) {
    uint8_t temp[4];
    for (int i = 0; i < 4; i++) {
        temp[0] = (uint8_t)(mul(0x0e, state[i * 4 + 0]) ^ mul(0x0b, state[i * 4 + 1]) ^ mul(0x0d, state[i * 4 + 2]) ^ mul(0x09, state[i * 4 + 3]));
        temp[1] = (uint8_t)(mul(0x09, state[i * 4 + 0]) ^ mul(0x0e, state[i * 4 + 1]) ^ mul(0x0b, state[i * 4 + 2]) ^ mul(0x0d, state[i * 4 + 3]));
        temp[2] = (uint8_t)(mul(0x0d, state[i * 4 + 0]) ^ mul(0x09, state[i * 4 + 1]) ^ mul(0x0e, state[i * 4 + 2]) ^ mul(0x0b, state[i * 4 + 3]));
        temp[3] = (uint8_t)(mul(0x0b, state[i * 4 + 0]) ^ mul(0x0d, state[i * 4 + 1]) ^ mul(0x09, state[i * 4 + 2]) ^ mul(0x0e, state[i * 4 + 3]));
        for (int j = 0; j < 4; j++) {
            state[i * 4 + j] = temp[j];
        }
    }
}

void AES::padData(std::vector<uint8_t>& data) {
    size_t block_size = 16;
    uint8_t paddingValue = (uint8_t)(block_size - (data.size() % block_size));
    data.insert(data.end(), paddingValue, paddingValue);
}

void AES::unpadData(std::vector<uint8_t>& data) {
    if (data.empty()) return;
    uint8_t paddingValue = data.back();
    if (paddingValue > 0 && paddingValue <= 16 && paddingValue <= data.size()) {
        data.resize(data.size() - paddingValue);
    }
}

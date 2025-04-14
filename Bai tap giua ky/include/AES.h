#ifndef AES_H
#define AES_H

#include "Sbox.h"
#include "Rcon.h"

#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 16
#define AES_ROUNDS 10
#define AES_EXPANDED_KEY_SIZE 176

void aes_key_expansion(const uint8_t *key, uint8_t *roundKeys);

// Encrypt AES 128-bit: plaintext 16 bytes, ciphertext 16 bytes
void aes_encrypt_block(const uint8_t *in, uint8_t *out, const uint8_t *roundKeys);

// Decrypt AES 128-bit: ciphertext 16 bytes, plaintext 16 bytes
void aes_decrypt_block(const uint8_t *in, uint8_t *out, const uint8_t *roundKeys);
#endif
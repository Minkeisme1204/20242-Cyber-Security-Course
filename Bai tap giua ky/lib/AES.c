#include "AES.h"

// x2 GF(2^8)
static uint8_t xtime(uint8_t b) {
    return (b << 1) ^ ((b & 0x80) ? 0x1B : 0x00);
    // check bit overflow
    // 0x80 = 1000 0000
    // 0x1B = x^4 + x^3 + x + 1
}

// multiply in GF(2^8)
static uint8_t multiply(uint8_t a, uint8_t b) {
    uint8_t r = 0, temp = a;
    for(int i = 0; i < 8; i++) {
        if(b & 1) r ^= temp;
        uint8_t hi_bit = temp & 0x80;
        temp <<= 1;
        if(hi_bit) temp ^= 0x1B;
        b >>= 1;
    }
    return r;
}

static void add_round_key(uint8_t *state, const uint8_t *roundKey) {
    for(int i = 0; i < 16; i++) {
        state[i] ^= roundKey[i];
    }
}

static void SubWord(uint8_t *word) {
    for(int i = 0; i < 4; i++) {
        word[i] = sbox[word[i]];
    }
}

static void RotWord(uint8_t *word) {
    uint8_t temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void aes_key_expansion(const uint8_t *key, uint8_t *roundKeys) {
    memcpy(roundKeys, key, AES_KEY_SIZE);

    for(int i = 4; i < 4 * (AES_ROUNDS + 1); i++) {
        uint8_t temp[4];
        memcpy(temp, &roundKeys[4*(i - 1)], 4);

        if(i % 4 == 0) {
            RotWord(temp);
            SubWord(temp);
            temp[0] ^= Rcon[i/4];
        }

        for(int j = 0; j < 4; j++) {
            roundKeys[4*i + j] = roundKeys[4*(i - 4) + j] ^ temp[j];
        }
    }
}


static void sub_bytes(uint8_t *state) {
    for(int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
    }
}

static void shift_rows(uint8_t *state) {
    // Row0: byte  0  -> 0 
    // Row1: byte  1  -> 5  -> 9  -> 13
    // Row2: byte  2  -> 6  -> 10 -> 14
    // Row3: byte  3  -> 7  -> 11 -> 15
    uint8_t temp[16];
    memcpy(temp, state, 16);

    state[0] = temp[0];
    state[1] = temp[5];
    state[2] = temp[10];
    state[3] = temp[15];

    state[4] = temp[4];
    state[5] = temp[9];
    state[6] = temp[14];
    state[7] = temp[3];

    state[8] = temp[8];
    state[9] = temp[13];
    state[10] = temp[2];
    state[11] = temp[7];

    state[12] = temp[12];
    state[13] = temp[1];
    state[14] = temp[6];
    state[15] = temp[11];
}

static void mix_columns(uint8_t *state) {
    for(int i = 0; i < 4; i++) {
        int c = 4 * i;
        uint8_t a0 = state[c + 0];
        uint8_t a1 = state[c + 1];
        uint8_t a2 = state[c + 2];
        uint8_t a3 = state[c + 3];

        state[c + 0] = multiply(0x02, a0) ^ multiply(0x03, a1) ^ a2 ^ a3;
        state[c + 1] = a0 ^ multiply(0x02, a1) ^ multiply(0x03, a2) ^ a3;
        state[c + 2] = a0 ^ a1 ^ multiply(0x02, a2) ^ multiply(0x03, a3);
        state[c + 3] = multiply(0x03, a0) ^ a1 ^ a2 ^ multiply(0x02, a3);
    }
}

void aes_encrypt_block(const uint8_t *in, uint8_t *out, const uint8_t *roundKeys) {
    uint8_t state[16];
    memcpy(state, in, 16);

    // 1st round
    add_round_key(state, roundKeys);

    // 9 rounds
    for(int round = 1; round < AES_ROUNDS; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, roundKeys + 16 * round);
    }

    // last round
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, roundKeys + 16 * AES_ROUNDS);

    memcpy(out, state, 16);
}

static void inv_sub_bytes(uint8_t *state) {
    for(int i = 0; i < 16; i++) {
        state[i] = inv_sbox[state[i]];
    }
}

static void inv_shift_rows(uint8_t *state) {
    uint8_t temp[16];
    memcpy(temp, state, 16);

    state[0] = temp[0];
    state[1] = temp[13];
    state[2] = temp[10];
    state[3] = temp[7];

    state[4] = temp[4];
    state[5] = temp[1];
    state[6] = temp[14];
    state[7] = temp[11];

    state[8] = temp[8];
    state[9] = temp[5];
    state[10] = temp[ 2];
    state[11] = temp[15];

    state[12] = temp[12];
    state[13] = temp[9];
    state[14] = temp[6];
    state[15] = temp[3];
}

static void inv_mix_columns(uint8_t *state) {
    for(int i = 0; i < 4; i++) {
        int c = 4 * i;
        uint8_t a0 = state[c + 0];
        uint8_t a1 = state[c + 1];
        uint8_t a2 = state[c + 2];
        uint8_t a3 = state[c + 3];

        state[c + 0] = multiply(0x0e, a0) ^ multiply(0x0b, a1) ^ multiply(0x0d, a2) ^ multiply(0x09, a3);
        state[c + 1] = multiply(0x09, a0) ^ multiply(0x0e, a1) ^ multiply(0x0b, a2) ^ multiply(0x0d, a3);
        state[c + 2] = multiply(0x0d, a0) ^ multiply(0x09, a1) ^ multiply(0x0e, a2) ^ multiply(0x0b, a3);
        state[c + 3] = multiply(0x0b, a0) ^ multiply(0x0d, a1) ^ multiply(0x09, a2) ^ multiply(0x0e, a3);
    }
}

void aes_decrypt_block(const uint8_t *in, uint8_t *out, const uint8_t *roundKeys) {
    uint8_t state[16];
    memcpy(state, in, 16);

    // last round
    add_round_key(state, roundKeys + 16 * AES_ROUNDS);

    // 9 rounds
    for(int round = AES_ROUNDS - 1; round > 0; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, roundKeys + 16 * round);
        inv_mix_columns(state);
    }

    // 1st rounds
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, roundKeys);

    memcpy(out, state, 16);
}
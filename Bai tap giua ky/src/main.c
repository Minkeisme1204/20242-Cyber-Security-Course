#include "encrypt.h"
#include "decrypt.h"
#include "utils.h"
#include <stdio.h>

int main() {
    FILE *fin = fopen("/home/minkescanor/Desktop/Study/Cyber Security/20242-Cyber-Security-Course/Bai tap giua ky/src/input.txt", "r");
    FILE *fout = fopen("/home/minkescanor/Desktop/Study/Cyber Security/20242-Cyber-Security-Course/Bai tap giua ky/src/output.txt", "w");
    if(!fin || !fout) {
        printf("Files open error\n");
        return 1;
    }

    char line1[128], line2[128];
    if(!fgets(line1, sizeof(line1), fin) || !fgets(line2, sizeof(line2), fin)) {
        printf("Read data error\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    // delete '\n'
    line1[strcspn(line1, "\r\n")] = 0;
    line2[strcspn(line2, "\r\n")] = 0;

    uint8_t inData[AES_BLOCK_SIZE];
    uint8_t key[AES_KEY_SIZE];
    uint8_t outData[AES_BLOCK_SIZE];
    uint8_t roundKeys[AES_EXPANDED_KEY_SIZE];

    if(!hexStringToBytes(line1, inData, AES_BLOCK_SIZE)) {
        printf("Invalid texts\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    if(!hexStringToBytes(line2, key, AES_KEY_SIZE)) {
        printf("Invalid Keys\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    aes_key_expansion(key, roundKeys);
    
    // Encrypt process
    aes_encrypt_block(inData, outData, roundKeys);
    for(int i = 0; i < AES_BLOCK_SIZE; i++) {
        fprintf(fout, "%02X", outData[i]);
    }
    fprintf(fout, "\n");

    // Decrypt process
    uint8_t decryptedData[AES_BLOCK_SIZE];
    aes_decrypt_block(outData, decryptedData, roundKeys);

    for(int i = 0; i < AES_BLOCK_SIZE; i++) {
        fprintf(fout, "%02X", decryptedData[i]);
    }
    fprintf(fout, "\n");

    fclose(fin);
    fclose(fout);
    return 0;
}
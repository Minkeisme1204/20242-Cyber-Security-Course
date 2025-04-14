#include "utils.h"

int hexCharToVal(char c) {
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    if(c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1; 
}

int hexStringToBytes(const char *hexStr, uint8_t *outBytes, int outLen) {
    // outLen = 16 (AES_BLOCK_SIZE) AES_KEY_SIZE
    if(strlen(hexStr) < (size_t)outLen * 2) return 0;
    for(int i = 0; i < outLen; i++) {
        int high = hexCharToVal(hexStr[2*i]);
        int low  = hexCharToVal(hexStr[2*i + 1]);
        if(high < 0 || low < 0) return 0;
        outBytes[i] = (uint8_t)((high << 4) | low);
    }
    return 1;
}

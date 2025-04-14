#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <string.h>

int hexCharToVal(char c);

int hexStringToBytes(const char *hexStr, uint8_t *outBytes, int outLen);

#endif
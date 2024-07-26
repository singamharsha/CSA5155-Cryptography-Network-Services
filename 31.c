#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CONSTANT_64 0x1B
#define CONSTANT_128 0x87

void generate_subkey_64(uint8_t *block, uint8_t *subkey1, uint8_t *subkey2) {
    uint8_t temp[8];
    int i;  
    memcpy(temp, block, 8);

    uint8_t carry = (temp[0] & 0x80) >> 7;  
    for (i = 0; i < 7; ++i) {
        temp[i] = (temp[i] << 1) | ((temp[i + 1] & 0x80) >> 7);
    }
    temp[7] = (temp[7] << 1) | carry;

    if (carry) {
        for (i = 0; i < 8; ++i) {
            subkey1[i] = temp[i] ^ CONSTANT_64;
        }
    } else {
        memcpy(subkey1, temp, 8);
    }

    memcpy(temp, subkey1, 8);
    carry = (temp[0] & 0x80) >> 7;  
    for (i = 0; i < 7; ++i) {
        temp[i] = (temp[i] << 1) | ((temp[i + 1] & 0x80) >> 7);
    }
    temp[7] = (temp[7] << 1) | carry;

    if (carry) {
        for (i = 0; i < 8; ++i) {
            subkey2[i] = temp[i] ^ CONSTANT_64;
        }
    } else {
        memcpy(subkey2, temp, 8);
    }
}

void generate_subkey_128(uint8_t *block, uint8_t *subkey1, uint8_t *subkey2) {
    uint8_t temp[16];
    int i; 
    memcpy(temp, block, 16);

    uint8_t carry = (temp[0] & 0x80) >> 7;  
    for (i = 0; i < 15; ++i) {
        temp[i] = (temp[i] << 1) | ((temp[i + 1] & 0x80) >> 7);
    }
    temp[15] = (temp[15] << 1) | carry;

    if (carry) {
        for (i = 0; i < 16; ++i) {
            subkey1[i] = temp[i] ^ CONSTANT_128;
        }
    } else {
        memcpy(subkey1, temp, 16);
    }

    memcpy(temp, subkey1, 16);
    carry = (temp[0] & 0x80) >> 7;  
    for (i = 0; i < 15; ++i) {
        temp[i] = (temp[i] << 1) | ((temp[i + 1] & 0x80) >> 7);
    }
    temp[15] = (temp[15] << 1) | carry;

    if (carry) {
        for (i = 0; i < 16; ++i) {
            subkey2[i] = temp[i] ^ CONSTANT_128;
        }
    } else {
        memcpy(subkey2, temp, 16);
    }
}

int main() {
    uint8_t block64[8] = {0}; 
    uint8_t subkey1_64[8], subkey2_64[8];

    uint8_t block128[16] = {0}; 
    uint8_t subkey1_128[16], subkey2_128[16];

    generate_subkey_64(block64, subkey1_64, subkey2_64);
    printf("64-bit Subkey 1: ");
    int i;
    for (i = 0; i < 8; ++i) printf("%02x ", subkey1_64[i]);
    printf("\n64-bit Subkey 2: ");
    for (i = 0; i < 8; ++i) printf("%02x ", subkey2_64[i]);
    printf("\n");

    generate_subkey_128(block128, subkey1_128, subkey2_128);
    printf("128-bit Subkey 1: ");
    for (i = 0; i < 16; ++i) printf("%02x ", subkey1_128[i]);
    printf("\n128-bit Subkey 2: ");
    for (i = 0; i < 16; ++i) printf("%02x ", subkey2_128[i]);
    printf("\n");

    return 0;
}


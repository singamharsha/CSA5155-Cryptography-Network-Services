#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define AES_BLOCK_SIZE 16

void xor_block(uint8_t *dst, const uint8_t *src) {
    int i;
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        dst[i] ^= src[i];
    }
}

void aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key) {
    
    int i;
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        output[i] = input[i] ^ key[i];  
    }
}

void aes_cbc_encrypt(const uint8_t *plaintext, size_t length, uint8_t *ciphertext,
                     const uint8_t *key, const uint8_t *iv) {
    uint8_t block[AES_BLOCK_SIZE];
    uint8_t iv_copy[AES_BLOCK_SIZE];
    memcpy(iv_copy, iv, AES_BLOCK_SIZE);

    size_t i;
    for (i = 0; i < length; i += AES_BLOCK_SIZE) {
        size_t block_size = AES_BLOCK_SIZE;
        if (length - i < AES_BLOCK_SIZE) {
            block_size = length - i;
        }
        memcpy(block, plaintext + i, block_size);
        if (block_size < AES_BLOCK_SIZE) {
            memset(block + block_size, 0, AES_BLOCK_SIZE - block_size);
        }
        xor_block(block, iv_copy);
        aes_encrypt(block, ciphertext + i, key);
        memcpy(iv_copy, ciphertext + i, AES_BLOCK_SIZE);
    }
}

int main() {
    uint8_t key[AES_BLOCK_SIZE] = {0x00};
    uint8_t iv[AES_BLOCK_SIZE] = {0x00};

    const char *plaintext = "This is a test message.";
    size_t length = strlen(plaintext);
    size_t padded_length = ((length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    uint8_t *ciphertext = (uint8_t *)malloc(padded_length);
    if (!ciphertext) {
        perror("malloc");
        return 1;
    }

    aes_cbc_encrypt((const uint8_t *)plaintext, padded_length, ciphertext, key, iv);

    printf("Ciphertext:\n");
    size_t i;
    for (i = 0; i < padded_length; ++i) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    free(ciphertext);
    
    return 0;
}


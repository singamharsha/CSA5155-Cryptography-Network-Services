#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define AES_BLOCK_SIZE 16

void aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key) {
    int i;
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        output[i] = input[i] ^ key[i];  
    }
}

void aes_ecb_encrypt(const uint8_t *plaintext, size_t length, uint8_t *ciphertext,
                     const uint8_t *key) {
    size_t i;
    for (i = 0; i < length; i += AES_BLOCK_SIZE) {
        aes_encrypt(plaintext + i, ciphertext + i, key);
    }
}

int main() {
    uint8_t key[AES_BLOCK_SIZE] = {0x00};

    const char *plaintext = "This is a test message.";
    size_t length = strlen(plaintext);
    size_t padded_length = ((length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    uint8_t *ciphertext = (uint8_t *)malloc(padded_length);
    if (!ciphertext) {
        perror("malloc");
        return 1;
    }

    aes_ecb_encrypt((const uint8_t *)plaintext, padded_length, ciphertext, key);
    printf("Ciphertext:\n");
    size_t i;
    for (i = 0; i < padded_length; ++i) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    free(ciphertext);
    return 0;
}


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 8 

void xor_blocks(uint8_t *out, const uint8_t *a, const uint8_t *b, size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        out[i] = a[i] ^ b[i];
    }
}

size_t pad(uint8_t *data, size_t length) {
    size_t padding_needed = BLOCK_SIZE - (length % BLOCK_SIZE);
    if (padding_needed == 0) {
        padding_needed = BLOCK_SIZE;
    }
    size_t new_length = length + padding_needed;
    data = realloc(data, new_length);
    memset(data + length, 0, padding_needed - 1);
    data[length + padding_needed - 1] = (uint8_t)padding_needed; 
    return new_length;
}

size_t unpad(uint8_t *data, size_t length) {
    uint8_t padding_value = data[length - 1];
    if (padding_value > BLOCK_SIZE) {
        return length; 
    }
    return length - padding_value;
}

void block_cipher_encrypt(uint8_t *out, const uint8_t *in, const uint8_t *key) {
    xor_blocks(out, in, key, BLOCK_SIZE);
}

void block_cipher_decrypt(uint8_t *out, const uint8_t *in, const uint8_t *key) {
    xor_blocks(out, in, key, BLOCK_SIZE);
}

void ecb_encrypt(uint8_t *data, size_t length, const uint8_t *key) {
    size_t i;
    for (i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher_encrypt(data + i, data + i, key);
    }
}

void ecb_decrypt(uint8_t *data, size_t length, const uint8_t *key) {
    size_t i;
    for (i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher_decrypt(data + i, data + i, key);
    }
}

void cbc_encrypt(uint8_t *data, size_t length, const uint8_t *key, const uint8_t *iv) {
    size_t i;
    uint8_t prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);
    for (i = 0; i < length; i += BLOCK_SIZE) {
        xor_blocks(data + i, data + i, prev_block, BLOCK_SIZE);
        block_cipher_encrypt(data + i, data + i, key);
        memcpy(prev_block, data + i, BLOCK_SIZE);
    }
}

void cbc_decrypt(uint8_t *data, size_t length, const uint8_t *key, const uint8_t *iv) {
    size_t i;
    uint8_t prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);
    uint8_t temp_block[BLOCK_SIZE];
    for (i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher_decrypt(temp_block, data + i, key);
        xor_blocks(data + i, temp_block, prev_block, BLOCK_SIZE);
        memcpy(prev_block, data + i, BLOCK_SIZE);
    }
}

void cfb_encrypt(uint8_t *data, size_t length, const uint8_t *key, const uint8_t *iv) {
    size_t i;
    uint8_t feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);
    uint8_t encrypted_block[BLOCK_SIZE];
    for (i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher_encrypt(encrypted_block, feedback, key);
        xor_blocks(data + i, data + i, encrypted_block, BLOCK_SIZE);
        memcpy(feedback, data + i, BLOCK_SIZE);
    }
}

void cfb_decrypt(uint8_t *data, size_t length, const uint8_t *key, const uint8_t *iv) {
    size_t i;
    uint8_t feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);
    uint8_t encrypted_block[BLOCK_SIZE];
    for (i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher_encrypt(encrypted_block, feedback, key);
        xor_blocks(data + i, data + i, encrypted_block, BLOCK_SIZE);
        memcpy(feedback, data + i, BLOCK_SIZE);
    }
}

int main() {
    const uint8_t key[BLOCK_SIZE] = "mykey123"; 
    uint8_t iv[BLOCK_SIZE] = "initvect";       
    uint8_t data[] = "This is some data to encrypt. It must be padded to match block size.";

    size_t length = sizeof(data) - 1; 

    printf("Original data: %s\n", data);

    size_t padded_length = pad(data, length);

    uint8_t *ecb_data = malloc(padded_length);
    memcpy(ecb_data, data, padded_length);
    ecb_encrypt(ecb_data, padded_length, key);
    printf("ECB Encrypted: %s\n", ecb_data);
    ecb_decrypt(ecb_data, padded_length, key);
    printf("ECB Decrypted: %s\n", ecb_data);
    free(ecb_data);

    uint8_t *cbc_data = malloc(padded_length);
    memcpy(cbc_data, data, padded_length);
    cbc_encrypt(cbc_data, padded_length, key, iv);
    printf("CBC Encrypted: %s\n", cbc_data);
    cbc_decrypt(cbc_data, padded_length, key, iv);
    printf("CBC Decrypted: %s\n", cbc_data);
    free(cbc_data);

    uint8_t *cfb_data = malloc(padded_length);
    memcpy(cfb_data, data, padded_length);
    cfb_encrypt(cfb_data, padded_length, key, iv);
    printf("CFB Encrypted: %s\n", cfb_data);
    cfb_decrypt(cfb_data, padded_length, key, iv);
    printf("CFB Decrypted: %s\n", cfb_data);
    free(cfb_data);

    return 0;
}


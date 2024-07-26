#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 8

void pad(unsigned char *input, int length, unsigned char *output) {
    int pad_len = BLOCK_SIZE - (length % BLOCK_SIZE);
    memcpy(output, input, length);
    int i;
    for (i = 0; i < pad_len; i++) {
        output[length + i] = (i == 0) ? 0x80 : 0x00;
    }
}
void dummy_encrypt(unsigned char *input, unsigned char *key, unsigned char *output) {
    int i;
    for (i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}

void ecb_mode(unsigned char *key, unsigned char *plaintext, int length, unsigned char *ciphertext) {
    unsigned char input_block[BLOCK_SIZE];
    unsigned char output_block[BLOCK_SIZE];
    int i;

    for (i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(input_block, plaintext + i, BLOCK_SIZE);
        dummy_encrypt(input_block, key, output_block);
        memcpy(ciphertext + i, output_block, BLOCK_SIZE);
    }
}

void cbc_mode(unsigned char *key, unsigned char *iv, unsigned char *plaintext, int length, unsigned char *ciphertext) {
    unsigned char input_block[BLOCK_SIZE];
    unsigned char output_block[BLOCK_SIZE];
    unsigned char temp_block[BLOCK_SIZE];
    int i, j;

    memcpy(temp_block, iv, BLOCK_SIZE);

    for (i = 0; i < length; i += BLOCK_SIZE) {
        for (j = 0; j < BLOCK_SIZE; j++) {
            input_block[j] = plaintext[i + j] ^ temp_block[j];
        }
        dummy_encrypt(input_block, key, output_block);
        memcpy(ciphertext + i, output_block, BLOCK_SIZE);
        memcpy(temp_block, output_block, BLOCK_SIZE);
    }
}

void cfb_mode(unsigned char *key, unsigned char *iv, unsigned char *plaintext, int length, unsigned char *ciphertext) {
    unsigned char output_block[BLOCK_SIZE];
    unsigned char temp_block[BLOCK_SIZE];
    int i, j;

    memcpy(temp_block, iv, BLOCK_SIZE);

    for (i = 0; i < length; i += BLOCK_SIZE) {
        dummy_encrypt(temp_block, key, output_block);
        for (j = 0; j < BLOCK_SIZE; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ output_block[j];
            temp_block[j] = ciphertext[i + j];
        }
    }
}

int main() {
    unsigned char key_data[BLOCK_SIZE] = "12345678";
    unsigned char iv_data[BLOCK_SIZE] = "87654321";
    unsigned char plaintext[] = "This is a test plaintext for DES encryption modes.";
    int length = strlen((char *)plaintext);
    int padded_length = ((length / BLOCK_SIZE) + 1) * BLOCK_SIZE;

    unsigned char padded_plaintext[padded_length];
    pad(plaintext, length, padded_plaintext);

    unsigned char ciphertext[padded_length];
    memset(ciphertext, 0, padded_length);

    printf("Plaintext: %s\n", plaintext);

    ecb_mode(key_data, padded_plaintext, padded_length, ciphertext);
    printf("ECB Ciphertext: ");
    int i;
    for (i = 0; i < padded_length; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    cbc_mode(key_data, iv_data, padded_plaintext, padded_length, ciphertext);
    printf("CBC Ciphertext: ");
    for (i = 0; i < padded_length; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    cfb_mode(key_data, iv_data, padded_plaintext, padded_length, ciphertext);
    printf("CFB Ciphertext: ");
    for (i = 0; i < padded_length; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    return 0;
}


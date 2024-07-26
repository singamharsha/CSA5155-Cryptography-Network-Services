#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t permute(uint8_t input, const int* perm, int n) {
    uint8_t output = 0;
    int i;
    for (i = 0; i < n; ++i) {
        output |= ((input >> (perm[i] - 1)) & 1) << (n - i - 1);
    }
    return output;
}

uint8_t left_shift(uint8_t input, int shifts, int n) {
    return ((input << shifts) | (input >> (n - shifts))) & ((1 << n) - 1);
}

const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
const int P4[] = {2, 4, 3, 1};
const int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV[] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};
const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

void generate_keys(uint16_t key, uint8_t* k1, uint8_t* k2) {
    key = permute(key, P10, 10);
    uint8_t left = key >> 5;
    uint8_t right = key & 0x1F;
    left = left_shift(left, 1, 5);
    right = left_shift(right, 1, 5);
    *k1 = permute((left << 5) | right, P8, 8);
    left = left_shift(left, 2, 5);
    right = left_shift(right, 2, 5);
    *k2 = permute((left << 5) | right, P8, 8);
}

uint8_t s_box(uint8_t input, const int sbox[4][4]) {
    int row = ((input & 0x8) >> 2) | (input & 0x1);
    int col = (input & 0x6) >> 1;
    return sbox[row][col];
}

uint8_t f(uint8_t input, uint8_t key) {
    uint8_t left = input >> 4;
    uint8_t right = input & 0xF;
    uint8_t ep = permute(right, EP, 8);
    uint8_t x = ep ^ key;
    uint8_t s0 = s_box(x >> 4, S0);
    uint8_t s1 = s_box(x & 0xF, S1);
    return permute((s0 << 2) | s1, P4, 4);
}

uint8_t sdes(uint8_t input, uint8_t key1, uint8_t key2, int mode) {
    uint8_t ip = permute(input, IP, 8);
    uint8_t left = ip >> 4;
    uint8_t right = ip & 0xF;
    uint8_t temp;
    if (mode == 0) {  
        temp = left;
        left = right ^ f(left, key1);
        right = temp;
        left = right ^ f(left, key2);
        right = temp;
    } else {  
        temp = left;
        left = right ^ f(left, key2);
        right = temp;
        left = right ^ f(left, key1);
        right = temp;
    }
    return permute((left << 4) | right, IP_INV, 8);
}

void cbc_encrypt(uint8_t* plaintext, uint8_t* ciphertext, int length, uint8_t iv, uint8_t k1, uint8_t k2) {
    uint8_t previous_block = iv;
    int i;
    for (i = 0; i < length; ++i) {
        uint8_t block = plaintext[i] ^ previous_block;
        ciphertext[i] = sdes(block, k1, k2, 0);
        previous_block = ciphertext[i];
    }
}

void cbc_decrypt(uint8_t* ciphertext, uint8_t* plaintext, int length, uint8_t iv, uint8_t k1, uint8_t k2) {
    uint8_t previous_block = iv;
    int i;
    for (i = 0; i < length; ++i) {
        uint8_t block = sdes(ciphertext[i], k1, k2, 1);
        plaintext[i] = block ^ previous_block;
        previous_block = ciphertext[i];
    }
}

void print_binary(uint8_t data) {
    int i;
    for (i = 7; i >= 0; --i) {
        printf("%d", (data >> i) & 1);
    }
}

int main() {
    uint8_t plaintext[] = {0x01, 0x23};  
    uint8_t key = 0x7D;  
    uint8_t iv = 0xAA;  
    uint8_t k1, k2;
    int length = 2;

    generate_keys(key, &k1, &k2);

    uint8_t ciphertext[length];
    cbc_encrypt(plaintext, ciphertext, length, iv, k1, k2);

    printf("Encrypted ciphertext: ");
    int i;
    for (i = 0; i < length; ++i) {
        print_binary(ciphertext[i]);
        printf(" ");
    }
    printf("\n");

    uint8_t decrypted[length];
    cbc_decrypt(ciphertext, decrypted, length, iv, k1, k2);

    printf("Decrypted plaintext: ");
    for (i = 0; i < length; ++i) {
        print_binary(decrypted[i]);
        printf(" ");
    }
    printf("\n");

    return 0;
}


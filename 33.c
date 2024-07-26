#include <stdio.h>
#include <stdint.h>
#include <string.h>

const uint8_t IP[64] = {58, 50, 42, 34, 26, 18, 10, 2, 
                        60, 52, 44, 36, 28, 20, 12, 4, 
                        62, 54, 46, 38, 30, 22, 14, 6, 
                        64, 56, 48, 40, 32, 24, 16, 8, 
                        57, 49, 41, 33, 25, 17, 9, 1, 
                        59, 51, 43, 35, 27, 19, 11, 3, 
                        61, 53, 45, 37, 29, 21, 13, 5, 
                        63, 55, 47, 39, 31, 23, 15, 7};

const uint8_t FP[64] = {40, 8, 48, 16, 56, 24, 64, 32, 
                        39, 7, 47, 15, 55, 23, 63, 31, 
                        38, 6, 46, 14, 54, 22, 62, 30, 
                        37, 5, 45, 13, 53, 21, 61, 29, 
                        36, 4, 44, 12, 52, 20, 60, 28, 
                        35, 3, 43, 11, 51, 19, 59, 27, 
                        34, 2, 42, 10, 50, 18, 58, 26, 
                        33, 1, 41, 9, 49, 17, 57, 25};

const uint8_t E[48] = {32, 1, 2, 3, 4, 5, 
                       4, 5, 6, 7, 8, 9, 
                       8, 9, 10, 11, 12, 13, 
                       12, 13, 14, 15, 16, 17, 
                       16, 17, 18, 19, 20, 21, 
                       20, 21, 22, 23, 24, 25, 
                       24, 25, 26, 27, 28, 29, 
                       28, 29, 30, 31, 32, 1};

const uint8_t P[32] = {16, 7, 20, 21, 
                       29, 12, 28, 17, 
                       1, 15, 23, 26, 
                       5, 18, 31, 10, 
                       2, 8, 24, 14, 
                       32, 27, 3, 9, 
                       19, 13, 30, 6, 
                       22, 11, 4, 25};

const uint8_t PC1[56] = {56, 48, 40, 32, 24, 16, 8, 
                        0, 57, 49, 41, 33, 25, 17, 
                        9, 1, 58, 50, 42, 34, 26, 
                        18, 10, 2, 59, 51, 43, 35, 
                        62, 54, 46, 38, 30, 22, 14, 
                        6, 61, 53, 45, 37, 29, 21, 
                        13, 5, 60, 52, 44, 36, 28, 
                        20, 12, 4, 63, 55, 47, 39, 
                        31, 23, 15, 7};

const uint8_t PC2[48] = {13, 16, 10, 23, 0, 4, 
                        2, 27, 14, 5, 20, 9, 
                        22, 18, 11, 3, 25, 7, 
                        15, 6, 26, 19, 12, 1, 
                        40, 51, 30, 36, 46, 54, 
                        29, 39, 50, 44, 32, 47, 
                        43, 48, 38, 55, 33, 52, 
                        45, 41, 49, 35, 28, 31};

const uint8_t ROTATIONS[16] = {1, 1, 2, 2, 
                                2, 2, 1, 2, 
                                2, 2, 2, 2, 
                                1, 2, 2, 1};

const uint8_t S[8][4][16] = {
    
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
   
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 9, 5, 0, 12, 14}
    },
  
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 8, 12, 7, 11, 4, 2},
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 11, 8, 5, 12, 4, 15},
        {9, 7, 8, 15, 0, 12, 5, 6, 2, 14, 13, 1, 10, 3, 11, 4},
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}
    },
    
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 11, 8, 5, 12, 4, 15},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 11, 8, 5, 12, 4, 15}
    },
   
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 8, 10, 6, 15, 3, 9, 0},
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 8, 12, 7, 11, 4, 2},
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 11, 8, 5, 12, 4, 15}
    },
   
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 7, 0, 12, 5, 14, 2, 6, 13, 1, 10, 15, 3, 11, 8, 4},
        {3, 13, 7, 8, 11, 5, 10, 12, 0, 6, 9, 4, 15, 14, 1, 2}
    },
   
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 11, 8, 5, 12, 4, 15}
    },
    {
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 9, 10, 3, 4, 0, 5},
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 8, 12, 7, 11, 4, 2},
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 11, 8, 5, 12, 4, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 9, 5, 0, 12, 14}
    }
};

void print_binary(uint64_t n) {
    int i;
    for (i = 63; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

uint64_t permute(uint64_t block, const uint8_t *table, size_t size) {
    uint64_t result = 0;
    size_t i;
    for (i = 0; i < size; ++i) {
        if (block & (1ULL << (64 - table[i] - 1))) {
            result |= 1ULL << (size - i - 1);
        }
    }
    return result;
}

uint64_t f(uint64_t r, uint64_t k) {
    uint64_t expanded_r = permute(r, E, 48);
    uint64_t sbox_output = 0;
    size_t i;
    
    expanded_r ^= k;

    for (i = 0; i < 8; ++i) {
        uint8_t row = ((expanded_r >> (42 - 6 * i)) & 0x20) | ((expanded_r >> (41 - 6 * i)) & 0x1);
        uint8_t col = (expanded_r >> (41 - 6 * i)) & 0xF;
        sbox_output |= (S[i][row][col] << (28 - 4 * i));
    }
    
    return permute(sbox_output, P, 32);
}

uint64_t ip(uint64_t block) {
    return permute(block, IP, 64);
}

uint64_t fp(uint64_t block) {
    return permute(block, FP, 64);
}

void key_schedule(const uint64_t key, uint64_t subkeys[16]) {
    uint64_t permuted_key = permute(key, PC1, 56);
    uint64_t left = permuted_key >> 28;
    uint64_t right = permuted_key & 0xFFFFFFF;
    size_t i;
    
    for (i = 0; i < 16; ++i) {
        left = (left << ROTATIONS[i] | left >> (28 - ROTATIONS[i])) & 0xFFFFFFF;
        right = (right << ROTATIONS[i] | right >> (28 - ROTATIONS[i])) & 0xFFFFFFF;
        
        uint64_t combined = (left << 28) | right;
        subkeys[i] = permute(combined, PC2, 48);
    }
}

void des_encrypt(const uint64_t plaintext, const uint64_t key, uint64_t *ciphertext) {
    uint64_t permuted_block = ip(plaintext);
    uint64_t left = permuted_block >> 32;
    uint64_t right = permuted_block & 0xFFFFFFFF;
    uint64_t subkeys[16];
    uint64_t temp;
    size_t i;
    
    key_schedule(key, subkeys);

    for (i = 0; i < 16; ++i) {
        temp = right;
        right = left ^ f(right, subkeys[i]);
        left = temp;
    }

    *ciphertext = fp((right << 32) | left);
}

void des_decrypt(const uint64_t ciphertext, const uint64_t key, uint64_t *plaintext) {
    uint64_t permuted_block = ip(ciphertext);
    uint64_t left = permuted_block >> 32;
    uint64_t right = permuted_block & 0xFFFFFFFF;
    uint64_t subkeys[16];
    uint64_t temp;
    size_t i;
    
    key_schedule(key, subkeys);

    for (i = 15; i >= 0; --i) {
        temp = right;
        right = left ^ f(right, subkeys[i]);
        left = temp;
    }

    *plaintext = fp((right << 32) | left);
}

int main() {
    uint64_t plaintext = 0x0123456789ABCDEF; 
    uint64_t key = 0x133457799BBCDFF1;      
    uint64_t ciphertext, decrypted;

    des_encrypt(plaintext, key, &ciphertext);
    printf("Ciphertext: ");
    print_binary(ciphertext);

    des_decrypt(ciphertext, key, &decrypted);
    printf("Decrypted: ");
    print_binary(decrypted);

    return 0;
}


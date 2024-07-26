#include <stdio.h>
#include <stdint.h>

const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

const int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// Permutation (P) table
const int P[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

// S-box tables
const int S[8][4][16] = {
    { // S1
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    { // S2
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    { // S3
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    { // S4
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    { // S5
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    { // S6
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    { // S7
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    { // S8
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

// Permuted Choice 1 (PC-1) table
const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

// Permuted Choice 2 (PC-2) table
const int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Number of left shifts per round
const int shifts[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

// Function prototypes
void initialPermutation(uint8_t* input, uint8_t* output);
void finalPermutation(uint8_t* input, uint8_t* output);
void expansion(uint8_t* input, uint8_t* output);
void permute(uint8_t* input, uint8_t* output, const int* table, int n);
void sBox(uint8_t* input, uint8_t* output);
void keySchedule(uint8_t* key, uint8_t subKeys[16][6]);
void desDecrypt(uint8_t* cipherText, uint8_t* plainText, uint8_t subKeys[16][6]);
void feistel(uint8_t* halfBlock, uint8_t* subKey);

int main() {
    // Example key and ciphertext
    uint8_t key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    uint8_t cipherText[8] = {0x85, 0xE8, 0x13, 0x54, 0x0F, 0x0A, 0xB4, 0x05};
    uint8_t plainText[8];
    uint8_t subKeys[16][6];

    keySchedule(key, subKeys);
    desDecrypt(cipherText, plainText, subKeys);

    printf("Decrypted plaintext: ");
    int i;
    for (i = 0; i < 8; i++) {
        printf("%02X ", plainText[i]);
    }
    printf("\n");

    return 0;
}

void initialPermutation(uint8_t* input, uint8_t* output) {
    permute(input, output, IP, 64);
}

void finalPermutation(uint8_t* input, uint8_t* output) {
    permute(input, output, FP, 64);
}

void expansion(uint8_t* input, uint8_t* output) {
    permute(input, output, E, 48);
}

void permute(uint8_t* input, uint8_t* output, const int* table, int n) {
    int i;
    for (i = 0; i < n; i++) {
        int bit = (input[(table[i] - 1) / 8] >> (7 - ((table[i] - 1) % 8))) & 0x01;
        output[i / 8] |= (bit << (7 - (i % 8)));
    }
}

void sBox(uint8_t* input, uint8_t* output) {
    uint8_t row, col, value;
    int i;
    for (i = 0; i < 8; i++) {
        row = ((input[i / 6] >> (7 - (i % 6))) & 0x02) | ((input[(i / 6) + 1] >> (7 - (i % 6))) & 0x01);
        col = (input[i / 6] >> (3 - (i % 6))) & 0x0F;
        value = S[i][row][col];
        output[i / 2] |= (value << ((1 - (i % 2)) * 4));
    }
}

void keySchedule(uint8_t* key, uint8_t subKeys[16][6]) {
    uint8_t permutedKey[7] = {0};
    uint8_t C[4] = {0}, D[4] = {0};
    int i, j, k;

    permute(key, permutedKey, PC1, 56);
    for (i = 0; i < 28; i++) {
        C[i / 8] |= ((permutedKey[i / 8] >> (7 - (i % 8))) & 0x01) << (7 - (i % 8));
        D[i / 8] |= ((permutedKey[(i + 28) / 8] >> (7 - ((i + 28) % 8))) & 0x01) << (7 - ((i + 28) % 8));
    }

    for (i = 0; i < 16; i++) {
        for (j = 0; j < shifts[i]; j++) {
            uint8_t tempC = C[0] >> 7;
            uint8_t tempD = D[0] >> 7;
            for (k = 0; k < 4; k++) {
                C[k] = (C[k] << 1) | ((C[(k + 1) % 4] >> 7) & 0x01);
                D[k] = (D[k] << 1) | ((D[(k + 1) % 4] >> 7) & 0x01);
            }
            C[3] = (C[3] & 0xFE) | tempC;
            D[3] = (D[3] & 0xFE) | tempD;
        }
        uint8_t combinedKey[7] = {0};
        for (j = 0; j < 28; j++) {
            combinedKey[j / 8] |= ((C[j / 8] >> (7 - (j % 8))) & 0x01) << (7 - (j % 8));
            combinedKey[(j + 28) / 8] |= ((D[j / 8] >> (7 - (j % 8))) & 0x01) << (7 - (j % 8));
        }
        permute(combinedKey, subKeys[15 - i], PC2, 48);
    }
}

void desDecrypt(uint8_t* cipherText, uint8_t* plainText, uint8_t subKeys[16][6]) {
    uint8_t initialPermuted[8] = {0};
    uint8_t finalPermuted[8] = {0};
    uint8_t L[4] = {0}, R[4] = {0};
    int i, j;

    initialPermutation(cipherText, initialPermuted);
    for (i = 0; i < 4; i++) {
        L[i] = initialPermuted[i];
        R[i] = initialPermuted[i + 4];
    }

    for (i = 0; i < 16; i++) {
        uint8_t temp[4] = {0};
        for (j = 0; j < 4; j++) {
            temp[j] = R[j];
        }
        feistel(R, subKeys[i]);
        for (j = 0; j < 4; j++) {
            R[j] ^= L[j];
            L[j] = temp[j];
        }
    }

    for (i = 0; i < 4; i++) {
        finalPermuted[i] = R[i];
        finalPermuted[i + 4] = L[i];
    }

    finalPermutation(finalPermuted, plainText);
}

void feistel(uint8_t* halfBlock, uint8_t* subKey) {
    uint8_t expanded[6] = {0};
    uint8_t sBoxInput[6] = {0};
    uint8_t sBoxOutput[4] = {0};
    uint8_t permutedOutput[4] = {0};
    int i;

    expansion(halfBlock, expanded);
    for (i = 0; i < 6; i++) {
        sBoxInput[i] = expanded[i] ^ subKey[i];
    }
    sBox(sBoxInput, sBoxOutput);
    permute(sBoxOutput, permutedOutput, P, 32);

    for (i = 0; i < 4; i++) {
        halfBlock[i] = permutedOutput[i];
    }
}


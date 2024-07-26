#include <stdio.h>
#include <stdint.h>
uint32_t left_shift(uint32_t key, int shifts) {
    return ((key << shifts) & 0x0FFFFFFF) | (key >> (28 - shifts));
}

int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

int PC2[48] = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int shift_schedule[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

uint64_t apply_PC1(uint64_t key) {
    uint64_t permuted_key = 0;
    int i;
    for (i = 0; i < 56; ++i) {
        permuted_key <<= 1;
        permuted_key |= (key >> (64 - PC1[i])) & 0x01;
    }
    return permuted_key;
}

uint64_t apply_PC2(uint64_t key) {
    uint64_t permuted_key = 0;
    int i;
    for (i = 0; i < 48; ++i) {
        permuted_key <<= 1;
        permuted_key |= (key >> (56 - PC2[i])) & 0x01;
    }
    return permuted_key;
}

void generate_subkeys(uint64_t initial_key) {
    uint64_t permuted_key = apply_PC1(initial_key);

    uint32_t left_half = (permuted_key >> 28) & 0x0FFFFFFF;
    uint32_t right_half = permuted_key & 0x0FFFFFFF;

    uint64_t subkeys[16];

    int i;
    for (i = 0; i < 16; ++i) {
        int shift_amount = shift_schedule[i];
        
        left_half = left_shift(left_half, shift_amount);
        right_half = left_shift(right_half, shift_amount);

        uint64_t combined_key = ((uint64_t)left_half << 28) | right_half;
        uint64_t subkey = apply_PC2(combined_key);
        
        subkeys[i] = subkey;
    }

    printf("Generated subkeys:\n");
    for (i = 0; i < 16; ++i) {
        printf("Subkey %d: %llx\n", i+1, subkeys[i]);
    }
}

int main() {
    uint64_t initial_key = 0x0123456789ABCDEF;

    generate_subkeys(initial_key);

    return 0;
}


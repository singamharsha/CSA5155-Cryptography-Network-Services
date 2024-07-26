#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 2
#define MOD 26

void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrix_mult(int matrix1[MATRIX_SIZE][MATRIX_SIZE], int matrix2[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j, k;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] = (result[i][j] + matrix1[i][k] * matrix2[k][j]) % MOD;
            }
        }
    }
}

int mod_inverse(int a, int mod) {
    int m0 = mod, t, q;
    int x0 = 0, x1 = 1;
    if (mod == 1) return 0;
    while (a > 1) {
        q = a / mod;
        t = mod;
        mod = a % mod;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

void matrix_inverse(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inverse[MATRIX_SIZE][MATRIX_SIZE]) {
    int det, inv_det;
    
    det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % MOD;
    det = (det + MOD) % MOD;  // Make sure det is non-negative
    inv_det = mod_inverse(det, MOD);

    inverse[0][0] = (matrix[1][1] * inv_det) % MOD;
    inverse[1][1] = (matrix[0][0] * inv_det) % MOD;
    inverse[0][1] = (-matrix[0][1] * inv_det + MOD) % MOD;
    inverse[1][0] = (-matrix[1][0] * inv_det + MOD) % MOD;
}

void text_to_matrix(char *text, int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = text[i * MATRIX_SIZE + j] - 'A';
        }
    }
}

void matrix_to_text(int matrix[MATRIX_SIZE][MATRIX_SIZE], char *text) {
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            text[i * MATRIX_SIZE + j] = matrix[i][j] + 'A';
        }
    }
}

void encrypt(char *plaintext, int key[MATRIX_SIZE][MATRIX_SIZE], char *ciphertext) {
    int text_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int cipher_matrix[MATRIX_SIZE][MATRIX_SIZE];
    
    text_to_matrix(plaintext, text_matrix);
    matrix_mult(key, text_matrix, cipher_matrix);
    matrix_to_text(cipher_matrix, ciphertext);
}

void decrypt(char *ciphertext, int key[MATRIX_SIZE][MATRIX_SIZE], char *plaintext) {
    int text_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int cipher_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int inverse_key[MATRIX_SIZE][MATRIX_SIZE];
    
    matrix_inverse(key, inverse_key);
    text_to_matrix(ciphertext, text_matrix);
    matrix_mult(inverse_key, text_matrix, cipher_matrix);
    matrix_to_text(cipher_matrix, plaintext);
}

int main() {
    int key[MATRIX_SIZE][MATRIX_SIZE] = {
        {9, 4},
        {5, 7}
    };

    char plaintext[] = "MEETMEATTHEUSUALPLACEATTENRATHERTHANEIGHTOCLOCK";
    char ciphertext[MATRIX_SIZE * MATRIX_SIZE + 1];
    char decryptedtext[MATRIX_SIZE * MATRIX_SIZE + 1];

    // Make sure plaintext length is a multiple of 2
    int length = strlen(plaintext);
    if (length % (MATRIX_SIZE * MATRIX_SIZE) != 0) {
        int padding = MATRIX_SIZE * MATRIX_SIZE - (length % (MATRIX_SIZE * MATRIX_SIZE));
        int i;
        for (i = length; i < length + padding; i++) {
            plaintext[i] = 'X';
        }
        plaintext[length + padding] = '\0';
    }

    printf("Plaintext: %s\n", plaintext);

    encrypt(plaintext, key, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);

    decrypt(ciphertext, key, decryptedtext);
    decryptedtext[MATRIX_SIZE * MATRIX_SIZE] = '\0'; // Null-terminate the string

    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}


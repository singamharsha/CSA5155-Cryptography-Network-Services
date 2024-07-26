#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 2
#define ALPHABET_SIZE 26

int mod_inverse(int a, int m) {
    int m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

int determinant(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0] + ALPHABET_SIZE) % ALPHABET_SIZE;
}

void inverse_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inverse[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = determinant(matrix);
    int inv_det = mod_inverse(det, ALPHABET_SIZE);
    inverse[0][0] = (matrix[1][1] * inv_det) % ALPHABET_SIZE;
    inverse[1][1] = (matrix[0][0] * inv_det) % ALPHABET_SIZE;
    inverse[0][1] = (-matrix[0][1] * inv_det + ALPHABET_SIZE) % ALPHABET_SIZE;
    inverse[1][0] = (-matrix[1][0] * inv_det + ALPHABET_SIZE) % ALPHABET_SIZE;
}

void multiply_matrices(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j, k;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] = (result[i][j] + a[i][k] * b[k][j]) % ALPHABET_SIZE;
            }
        }
    }
}

void encrypt_hill(char *plaintext, int key[MATRIX_SIZE][MATRIX_SIZE], char *ciphertext) {
    int block[MATRIX_SIZE], encrypted[MATRIX_SIZE];
    int i, j;
    int length = strlen(plaintext);
    
    for (i = 0; i < length; i += MATRIX_SIZE) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            block[j] = plaintext[i + j] - 'A';
        }
        multiply_matrices(key, (int[][MATRIX_SIZE]){ {block[0]}, {block[1]} }, encrypted);
        for (j = 0; j < MATRIX_SIZE; j++) {
            ciphertext[i + j] = encrypted[j] + 'A';
        }
    }
    ciphertext[length] = '\0';
}

void decrypt_hill(char *ciphertext, int key[MATRIX_SIZE][MATRIX_SIZE], char *plaintext) {
    int block[MATRIX_SIZE], decrypted[MATRIX_SIZE];
    int i, j;
    int length = strlen(ciphertext);
    
    int inv_key[MATRIX_SIZE][MATRIX_SIZE];
    inverse_matrix(key, inv_key);

    for (i = 0; i < length; i += MATRIX_SIZE) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            block[j] = ciphertext[i + j] - 'A';
        }
        multiply_matrices(inv_key, (int[][MATRIX_SIZE]){ {block[0]}, {block[1]} }, decrypted);
        for (j = 0; j < MATRIX_SIZE; j++) {
            plaintext[i + j] = decrypted[j] + 'A';
        }
    }
    plaintext[length] = '\0';
}

void recover_key_matrix(char *plaintext, char *ciphertext, int key[MATRIX_SIZE][MATRIX_SIZE]) {
    int pt_matrix[MATRIX_SIZE][MATRIX_SIZE], ct_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int i, j;

    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            pt_matrix[i][j] = plaintext[i * MATRIX_SIZE + j] - 'A';
            ct_matrix[i][j] = ciphertext[i * MATRIX_SIZE + j] - 'A';
        }
    }

    int inv_pt_matrix[MATRIX_SIZE][MATRIX_SIZE];
    inverse_matrix(pt_matrix, inv_pt_matrix);
    multiply_matrices(ct_matrix, inv_pt_matrix, key);
}

int main() {
    char plaintext[100], ciphertext[100];
    int key[MATRIX_SIZE][MATRIX_SIZE] = { {0} };
    int i, j;

    printf("Enter known plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    strtok(plaintext, "\n"); 

    printf("Enter corresponding ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    strtok(ciphertext, "\n"); 
    recover_key_matrix(plaintext, ciphertext, key);

    printf("Recovered Key Matrix:\n");
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", key[i][j]);
        }
        printf("\n");
    }

    char encrypted_text[100], decrypted_text[100];
    printf("Enter plaintext for encryption: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    strtok(plaintext, "\n"); 

    encrypt_hill(plaintext, key, encrypted_text);
    printf("Encrypted Text: %s\n", encrypted_text);

    decrypt_hill(encrypted_text, key, decrypted_text);
    printf("Decrypted Text: %s\n", decrypted_text);

    return 0;
}


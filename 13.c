#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 2 

void multiplyMatrix(int key[MATRIX_SIZE][MATRIX_SIZE], int textVector[], int result[]) {
    int i, j; 

    for (i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0;
        for (j = 0; j < MATRIX_SIZE; j++) {
            result[i] += key[i][j] * textVector[j];
        }
        result[i] %= 26;
    }
}

void encrypt(char plaintext[], int key[MATRIX_SIZE][MATRIX_SIZE], char ciphertext[]) {
    int textVector[MATRIX_SIZE], result[MATRIX_SIZE];
    int i, j; 

    for (i = 0; i < strlen(plaintext); i += MATRIX_SIZE) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            textVector[j] = plaintext[i + j] - 'A';
        }
        multiplyMatrix(key, textVector, result);
        for (j = 0; j < MATRIX_SIZE; j++) {
            ciphertext[i + j] = result[j] + 'A';
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

int main() {
    char plaintext[] = "HELP"; 
    int key[MATRIX_SIZE][MATRIX_SIZE] = {{6, 24}, {1, 13}};
    char ciphertext[sizeof(plaintext)];

    encrypt(plaintext, key, ciphertext);

    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}


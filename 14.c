#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void encrypt(char *plaintext, int *key, char *ciphertext, int keyLength) {
    int len = strlen(plaintext);
    int i;
    for (i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            ciphertext[i] = ((toupper(plaintext[i]) - 'A') + key[i % keyLength]) % 26 + 'A';
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
}

void decrypt(char *ciphertext, int *key, char *plaintext, int keyLength) {
    int len = strlen(ciphertext);
    int i; 
    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            plaintext[i] = ((toupper(ciphertext[i]) - 'A') - key[i % keyLength] + 26) % 26 + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

void findKey(char *ciphertext, char *desiredPlaintext, int *key, int keyLength) {
    int len = strlen(ciphertext);
    int i; 
    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i]) && isalpha(desiredPlaintext[i])) {
            key[i % keyLength] = (toupper(ciphertext[i]) - toupper(desiredPlaintext[i]) + 26) % 26;
        } else {
            key[i % keyLength] = 0; 
        }
    }
}

int main() {
    char plaintext[] = "send more money";
    int keyStream[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int keyLength = sizeof(keyStream) / sizeof(keyStream[0]);
    char ciphertext[strlen(plaintext) + 1];

    encrypt(plaintext, keyStream, ciphertext, keyLength);
    printf("Encrypted Ciphertext: %s\n", ciphertext);

    char desiredPlaintext[] = "cash not needed";
    int foundKey[keyLength];

    findKey(ciphertext, desiredPlaintext, foundKey, keyLength);

    int i; 
    printf("Found Key: ");
    for (i = 0; i < keyLength; i++) {
        printf("%d ", foundKey[i]);
    }
    printf("\n");

    char decryptedText[strlen(ciphertext) + 1];
    decrypt(ciphertext, foundKey, decryptedText, keyLength);
    printf("Decrypted Text with Found Key: %s\n", decryptedText);

    return 0;
}


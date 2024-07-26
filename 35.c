#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

void otp_vigenere_encrypt(char *plaintext, const int *key, size_t key_length, char *ciphertext) {
    size_t i;
    size_t text_length = strlen(plaintext);
    for (i = 0; i < text_length; i++) {
        if (isalpha(plaintext[i])) {
            
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((plaintext[i] - base) + key[i % key_length]) % ALPHABET_SIZE + base;
        } else {
           
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[text_length] = '\0'; 
}

void otp_vigenere_decrypt(char *ciphertext, const int *key, size_t key_length, char *plaintext) {
    size_t i;
    size_t text_length = strlen(ciphertext);
    for (i = 0; i < text_length; i++) {
        if (isalpha(ciphertext[i])) {
           
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = ((ciphertext[i] - base) - key[i % key_length] + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[text_length] = '\0'; 
}

int main() {
    char plaintext[256];
    char ciphertext[256];
    char decryptedtext[256];

    int key[] = {3, 19, 5, 7}; 
    size_t key_length = sizeof(key) / sizeof(key[0]);

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
   
    size_t length = strlen(plaintext);
    if (length > 0 && plaintext[length - 1] == '\n') {
        plaintext[length - 1] = '\0';
    }

    otp_vigenere_encrypt(plaintext, key, key_length, ciphertext);
    printf("Encrypted ciphertext: %s\n", ciphertext);

    otp_vigenere_decrypt(ciphertext, key, key_length, decryptedtext);
    printf("Decrypted plaintext: %s\n", decryptedtext);

    return 0;
}


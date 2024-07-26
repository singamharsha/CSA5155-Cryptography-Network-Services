#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

int mod_inverse(int a, int m) {
    int x;
    a = a % m;
    for (x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; 
}

void affine_encrypt(const char *plaintext, int a, int b, char *ciphertext) {
    int i;
    size_t text_length = strlen(plaintext);
    for (i = 0; i < text_length; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((a * (plaintext[i] - base) + b) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[i] = '\0'; 
}

void affine_decrypt(const char *ciphertext, int a, int b, char *plaintext) {
    int i;
    int a_inv = mod_inverse(a, ALPHABET_SIZE);
    if (a_inv == -1) {
        printf("Error: Modular inverse does not exist for a = %d\n", a);
        return;
    }
    size_t text_length = strlen(ciphertext);
    for (i = 0; i < text_length; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = ((a_inv * ((ciphertext[i] - base) - b + ALPHABET_SIZE)) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[i] = '\0'; 
}

int main() {
    char plaintext[256];
    char ciphertext[256];
    char decryptedtext[256];
    int a, b;

    printf("Enter the value of a (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter the value of b: ");
    scanf("%d", &b);

    if (mod_inverse(a, ALPHABET_SIZE) == -1) {
        printf("Error: Value of a is not coprime with 26. Encryption cannot be performed.\n");
        return 1;
    }

    printf("Enter plaintext: ");
    scanf(" %[^\n]", plaintext); 

    affine_encrypt(plaintext, a, b, ciphertext);
    printf("Encrypted ciphertext: %s\n", ciphertext);

    affine_decrypt(ciphertext, a, b, decryptedtext);
    printf("Decrypted plaintext: %s\n", decryptedtext);

    return 0;
}


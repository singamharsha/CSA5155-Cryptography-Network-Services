#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

void generate_cipher_sequence(const char* keyword, char* cipher) {
    int used[ALPHABET_SIZE] = {0}; // Array to mark used characters
    int index = 0;
    int i;

    for (i = 0; i < strlen(keyword); i++) {
        char ch = toupper(keyword[i]);
        if (!used[ch - 'A']) {
            cipher[index++] = ch;
            used[ch - 'A'] = 1;
        }
    }

    // Add the remaining letters of the alphabet
    char ch;
    for (ch = 'A'; ch <= 'Z'; ch++) {
        if (!used[ch - 'A']) {
            cipher[index++] = ch;
        }
    }
    cipher[index] = '\0';
}

void encrypt(const char* plaintext, const char* cipher, char* encrypted) {
    int i;
    for (i = 0; i < strlen(plaintext); i++) {
        if (isalpha(plaintext[i])) {
            char ch = toupper(plaintext[i]);
            encrypted[i] = cipher[ch - 'A'];
        } else {
            encrypted[i] = plaintext[i];
        }
    }
    encrypted[strlen(plaintext)] = '\0';
}

void decrypt(const char* ciphertext, const char* cipher, char* decrypted) {
    int i, j;
    for (i = 0; i < strlen(ciphertext); i++) {
        if (isalpha(ciphertext[i])) {
            char ch = toupper(ciphertext[i]);
            for (j = 0; j < ALPHABET_SIZE; j++) {
                if (cipher[j] == ch) {
                    decrypted[i] = 'A' + j;
                    break;
                }
            }
        } else {
            decrypted[i] = ciphertext[i];
        }
    }
    decrypted[strlen(ciphertext)] = '\0';
}

int main() {
    char keyword[] = "CIPHER";
    char cipher[ALPHABET_SIZE + 1];
    char plaintext[256];
    char encrypted[256];
    char decrypted[256];

    generate_cipher_sequence(keyword, cipher);

    printf("Generated cipher sequence: %s\n", cipher);

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove the newline character

    encrypt(plaintext, cipher, encrypted);
    printf("Encrypted text: %s\n", encrypted);

    decrypt(encrypted, cipher, decrypted);
    printf("Decrypted text: %s\n", decrypted);

    return 0;
}


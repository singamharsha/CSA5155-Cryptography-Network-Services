#include <stdio.h>
#include <string.h>
#include <ctype.h>

void vigenereEncrypt(char *plaintext, char *key, char *ciphertext) {
    int textLen = strlen(plaintext);
    int keyLen = strlen(key);
    int j = 0;

    for (int i = 0; i < textLen; i++) {
        if (isalpha(plaintext[i])) {
            char offset = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = (plaintext[i] - offset + tolower(key[j % keyLen]) - 'a') % 26 + offset;
            j++;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[textLen] = '\0';
}

int main() {
    char plaintext[100], key[100], ciphertext[100];

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; 

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    vigenereEncrypt(plaintext, key, ciphertext);

    printf("Encrypted message: %s\n", ciphertext);

    return 0;
}

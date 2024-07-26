#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encrypt(char message[], char key[]) {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, j;

    for (i = 0; message[i] != '\0'; i++) {
        char current_char = toupper(message[i]);
        if (isalpha(current_char)) {
            for (j = 0; j < 26; j++) {
                if (alphabet[j] == current_char) {
                    message[i] = islower(message[i]) ? tolower(key[j]) : key[j];
                    break;
                }
            }
        }
    }
}
void decrypt(char message[], char key[]) {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, j;

    for (i = 0; message[i] != '\0'; i++) {
        char current_char = toupper(message[i]);
        if (isalpha(current_char)) {
            for (j = 0; j < 26; j++) {
                if (key[j] == current_char) {
                    message[i] = islower(message[i]) ? tolower(alphabet[j]) : alphabet[j];
                    break;
                }
            }
        }
    }
}

int main() {
    char message[100];
    char key[27] = "QWERTYUIOPASDFGHJKLZXCVBNM"; 
    printf("Enter a message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; 
    encrypt(message, key);
    printf("Encrypted message: %s\n", message);
    decrypt(message, key);
    printf("Decrypted message: %s\n", message);

    return 0;
}

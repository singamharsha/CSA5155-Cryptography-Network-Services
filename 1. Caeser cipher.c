#include <stdio.h>
#include <ctype.h>

void encrypt(char *text, int shift) {
    char c;
    while ((c = *text) != '\0') {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            *text = (c - base + shift) % 26 + base;
        }
        text++;
    }
}

int main() {
    char text[100];
    int shift;

    printf("Enter a string to encrypt: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter the shift value (1-25): ");
    scanf("%d", &shift);

    if (shift < 1 || shift > 25) {
        printf("Shift value must be in the range 1-25.\n");
        return 1;
    }

    encrypt(text, shift);
    printf("Encrypted text: %s\n", text);

    return 0;
}


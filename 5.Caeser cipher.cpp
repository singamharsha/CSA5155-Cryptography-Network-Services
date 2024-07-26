#include <stdio.h>
#include <ctype.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void affineCaesarEncrypt(char *plaintext, int a, int b, char *ciphertext) {
    int i;
    for (i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            char offset = isupper(plaintext[i]) ? 'A' : 'a';
            int p = plaintext[i] - offset;
            int encrypted_char = (a * p + b) % 26;
            if (encrypted_char < 0) {
                encrypted_char += 26;
            }
            ciphertext[i] = encrypted_char + offset;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[i] = '\0';
}

int main() {
    char plaintext[100], ciphertext[100];
    int a, b;
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    printf("Enter the value of a (should be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter the value of b (should be between 0 and 25): ");
    scanf("%d", &b);
    if (gcd(a, 26) != 1) {
        printf("Error: The value of a is not coprime with 26. Encryption cannot proceed.\n");
        return 1;
    }
    affineCaesarEncrypt(plaintext, a, b, ciphertext);
    printf("Encrypted message: %s\n", ciphertext);
    return 0;
}

#include <stdio.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_exp(int m, int e, int n) {
    int result = 1;
    while (e > 0) {
        if (e % 2 == 1) { 
            result = (result * m) % n;
        }
        m = (m * m) % n;
        e /= 2;
    }
    return result;
}

int main() {
    int e = 3;  
    int n = 2537; 
    int plaintext_chars[] = {0, 1, 2, 3, 4}; 
    int num_chars = sizeof(plaintext_chars) / sizeof(plaintext_chars[0]);

    printf("Public Key (e, n): (%d, %d)\n", e, n);

    int i; 
    for (i = 0; i < num_chars; ++i) {
        int m = plaintext_chars[i];
        int c = mod_exp(m, e, n);
        printf("Plaintext: %d -> Ciphertext: %d\n", m, c);
    }

    return 0;
}


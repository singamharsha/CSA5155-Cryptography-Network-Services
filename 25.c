#include <stdio.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int m0 = m;
    int y = 0;
    int x = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0)
        x += m0;
    return x;
}

int main() {
    int e = 31; 
    int n = 3599; 
    int plaintext_block = 1197; 
    int common_factor = gcd(plaintext_block, n);

    printf("Common factor: %d\n", common_factor);

    int p = common_factor;
    int q = n / p;

    printf("p = %d\n", p);
    printf("q = %d\n", q);

    int phi_n = (p - 1) * (q - 1);
    printf("phi(n) = %d\n", phi_n);

    int d = mod_inverse(e, phi_n);
    printf("Private key d = %d\n", d);

    return 0;
}


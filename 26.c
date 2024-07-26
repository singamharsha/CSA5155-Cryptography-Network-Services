#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

void generate_keys(int p, int q, int* e, int* d, int* n) {
    *n = p * q;
    int phi_n = (p - 1) * (q - 1);
    
    for (*e = 2; *e < phi_n; ++(*e)) {
        if (gcd(*e, phi_n) == 1) break;
    }

    *d = mod_inverse(*e, phi_n);
}

int main() {
    int p = 61; 
    int q = 53; 

    int e, d, n;
    generate_keys(p, q, &e, &d, &n);

    printf("Public Key (e, n): (%d, %d)\n", e, n);
    printf("Private Key (d): %d\n", d);

    return 0;
}


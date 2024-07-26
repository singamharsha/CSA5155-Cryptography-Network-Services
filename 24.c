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

void find_factors(int n, int* p, int* q) {
    int i;
    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0) {
            *p = i;
            *q = n / i;
            return;
        }
    }
}

int main() {
    int e = 31; 
    int n = 3599; 
    int p, q;
    
    find_factors(n, &p, &q);
    
    printf("p = %d, q = %d\n", p, q);
    
    int phi_n = (p - 1) * (q - 1);
    
    int d = mod_inverse(e, phi_n);
    
    printf("phi(n) = %d\n", phi_n);
    printf("Private key d = %d\n", d);
    
    return 0;
}


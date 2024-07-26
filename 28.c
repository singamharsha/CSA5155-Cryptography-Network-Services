#include <stdio.h>
#include <math.h>

unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) { 
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    unsigned long long p = 23;  
    unsigned long long g = 5;   

    unsigned long long a = 6;  
    unsigned long long A = mod_exp(g, a, p);  

    unsigned long long b = 15; 
    unsigned long long B = mod_exp(g, b, p);  

    unsigned long long shared_secret_Alice = mod_exp(B, a, p);
    unsigned long long shared_secret_Bob = mod_exp(A, b, p);

    printf("Public parameters:\n");
    printf("p = %llu\n", p);
    printf("g = %llu\n", g);
    printf("Alice's private key: %llu\n", a);
    printf("Bob's private key: %llu\n", b);
    printf("Alice's public value: %llu\n", A);
    printf("Bob's public value: %llu\n", B);
    printf("Shared secret (Alice's computation): %llu\n", shared_secret_Alice);
    printf("Shared secret (Bob's computation): %llu\n", shared_secret_Bob);

    return 0;
}


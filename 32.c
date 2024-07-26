#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define P 0xF9A8D6E4  
#define Q 0xC24A5A3F  
#define G 0x2A3B4C5D  
#define N 0xB16F00A2  
#define D 0x6A1B2C3D  

void generate_dsa_signature(const uint8_t *message, size_t message_len, 
                            uint32_t private_key, uint32_t *k, uint32_t *r, uint32_t *s);
void generate_rsa_signature(const uint8_t *message, size_t message_len, 
                            uint32_t private_key, uint32_t *signature);

uint32_t mod_exp(uint32_t base, uint32_t exp, uint32_t mod);
uint32_t mod_mul(uint32_t a, uint32_t b, uint32_t mod);

int main() {
    
    uint32_t private_key = D;
    uint32_t k, r, s, signature;
    uint8_t message[] = "Hello, World!";
    size_t message_len = sizeof(message) - 1; 
    generate_dsa_signature(message, message_len, private_key, &k, &r, &s);
    printf("DSA Signature:\n");
    printf("k: %08X\n", k);
    printf("r: %08X\n", r);
    printf("s: %08X\n", s);

    generate_rsa_signature(message, message_len, private_key, &signature);
    printf("RSA Signature:\n");
    printf("Signature: %08X\n", signature);

    return 0;
}

void generate_dsa_signature(const uint8_t *message, size_t message_len, 
                            uint32_t private_key, uint32_t *k, uint32_t *r, uint32_t *s) {
    uint32_t g_k;
    uint32_t hash;
    uint32_t k_hash;
    uint32_t r_priv;
    size_t i;

    *k = rand() % Q;

    g_k = mod_exp(G, *k, P);
    *r = g_k % Q;

    hash = 0;
    for (i = 0; i < message_len; ++i) {
        hash += message[i];
    }
    hash %= Q;

    k_hash = mod_mul(*k, hash, Q);
    r_priv = mod_mul(*r, private_key, Q);
    *s = (k_hash + r_priv) % Q;
}

void generate_rsa_signature(const uint8_t *message, size_t message_len, 
                            uint32_t private_key, uint32_t *signature) {
    uint32_t hash;
    size_t i;

    hash = 0;
    for (i = 0; i < message_len; ++i) {
        hash += message[i];
    }

    *signature = mod_exp(hash, private_key, N);
}

uint32_t mod_exp(uint32_t base, uint32_t exp, uint32_t mod) {
    uint32_t result;
    uint32_t temp_base;
    uint32_t temp_exp;

    result = 1;
    temp_base = base;
    temp_exp = exp;

    while (temp_exp > 0) {
        if (temp_exp % 2 == 1) {
            result = mod_mul(result, temp_base, mod);
        }
        temp_base = mod_mul(temp_base, temp_base, mod);
        temp_exp /= 2;
    }
    return result;
}

uint32_t mod_mul(uint32_t a, uint32_t b, uint32_t mod) {
    uint32_t result;
    uint32_t temp_a;
    uint32_t temp_b;

    result = 0;
    temp_a = a;
    temp_b = b;

    while (temp_b > 0) {
        if (temp_b % 2 == 1) {
            result = (result + temp_a) % mod;
        }
        temp_a = (temp_a * 2) % mod;
        temp_b /= 2;
    }
    return result;
}


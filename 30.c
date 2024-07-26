#include <stdio.h>
#include <stdint.h>
#include <string.h>

void block_cipher(uint8_t *block, uint8_t *key) {
    int i;
    for (i = 0; i < 16; ++i) {
        block[i] ^= key[i];
    }
}

void compute_cbc_mac(uint8_t *key, uint8_t *message, uint8_t *mac) {
    uint8_t block[16];
    memcpy(block, message, 16);
    
    block_cipher(block, key);
    
    memcpy(mac, block, 16);
}

void print_block(const char *label, uint8_t *block) {
    int i;
    printf("%s: ", label);
    for (i = 0; i < 16; ++i) {
        printf("%02x ", block[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
                       0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10}; 
    uint8_t message[16] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
                           0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20}; 
    uint8_t mac[16];  
    uint8_t xor_block[16];  
    uint8_t two_block_message[32]; 
    int i;
    
    compute_cbc_mac(key, message, mac);
    
    print_block("Message X", message);
    print_block("MAC T", mac);

    for (i = 0; i < 16; ++i) {
        xor_block[i] = message[i] ^ mac[i];
    }
    print_block("X ? T", xor_block);

    memcpy(two_block_message, message, 16);
    memcpy(two_block_message + 16, xor_block, 16);

    uint8_t two_block_mac[16];
    compute_cbc_mac(key, two_block_message, two_block_mac);
    print_block("CBC-MAC for X || (X ? T)", two_block_mac);
    
    return 0;
}


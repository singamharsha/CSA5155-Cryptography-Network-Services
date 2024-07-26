#include <stdio.h>
#include <string.h>

#define CIPHERTEXT "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*;4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81 (‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?"

void frequency_analysis(const char* text, int freq[]) {
    size_t i;
    for (i = 0; i < strlen(text); i++) {
        if (text[i] != ' ') {
            freq[(unsigned char)text[i]]++;
        }
    }
}

void print_frequencies(const int freq[]) {
    int i;
    for (i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            printf("%c: %d\n", i, freq[i]);
        }
    }
}

int main() {
    int freq[256] = {0}; 

    frequency_analysis(CIPHERTEXT, freq);
    print_frequencies(freq);

    return 0;
}


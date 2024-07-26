#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define TOP_N 10

const double english_frequencies[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.316, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.929, 0.074, 1.430, 2.237
};

void calculate_frequencies(char *text, double frequencies[ALPHABET_SIZE]) {
    int count[ALPHABET_SIZE] = {0};
    int length = strlen(text);
    int i;
    
    for (i = 0; i < length; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            count[text[i] - 'A']++;
        }
    }
    
    for (i = 0; i < ALPHABET_SIZE; i++) {
        frequencies[i] = (double)count[i] / length * 100;
    }
}

double calculate_score(double frequencies[ALPHABET_SIZE]) {
    double score = 0.0;
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        score -= (frequencies[i] - english_frequencies[i]) * (frequencies[i] - english_frequencies[i]);
    }
    return score;
}

// Function to decrypt ciphertext with a given shift
void decrypt_with_shift(char *ciphertext, int shift, char *plaintext) {
    int length = strlen(ciphertext);
    int i;
    for (i = 0; i < length; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((ciphertext[i] - 'A' - shift + ALPHABET_SIZE) % ALPHABET_SIZE) + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[length] = '\0';
}

void frequency_attack(char *ciphertext) {
    double frequencies[ALPHABET_SIZE];
    char plaintext[1000];
    char best_plaintexts[TOP_N][1000];
    double best_scores[TOP_N];
    int length = strlen(ciphertext);
    int shift, i, j;
    
    for (i = 0; i < TOP_N; i++) {
        best_scores[i] = -1e9;
    }
    
    for (shift = 0; shift < ALPHABET_SIZE; shift++) {
        decrypt_with_shift(ciphertext, shift, plaintext);
        calculate_frequencies(plaintext, frequencies);
        double score = calculate_score(frequencies);
        
        for (i = 0; i < TOP_N; i++) {
            if (score > best_scores[i]) {
                for (j = TOP_N - 1; j > i; j--) {
                    strcpy(best_plaintexts[j], best_plaintexts[j - 1]);
                    best_scores[j] = best_scores[j - 1];
                }
                strcpy(best_plaintexts[i], plaintext);
                best_scores[i] = score;
                break;
            }
        }
    }
    
    printf("Top %d possible plaintexts:\n", TOP_N);
    for (i = 0; i < TOP_N; i++) {
        printf("Plaintext %d: %s (Score: %.2f)\n", i + 1, best_plaintexts[i], best_scores[i]);
    }
}

int main() {
    char ciphertext[1000];
    
    printf("Enter ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    strtok(ciphertext, "\n"); 
    frequency_attack(ciphertext);
    
    return 0;
}


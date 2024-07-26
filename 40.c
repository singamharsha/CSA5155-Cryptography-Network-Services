#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        if (isalpha(text[i])) {
            char c = toupper(text[i]);
            count[c - 'A']++;
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

void decrypt_with_key(char *ciphertext, char key[ALPHABET_SIZE], char *plaintext) {
    int length = strlen(ciphertext);
    int i;

    for (i = 0; i < length; i++) {
        if (isalpha(ciphertext[i])) {
            char c = toupper(ciphertext[i]);
            plaintext[i] = key[c - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[length] = '\0';
}

void letter_frequency_attack(char *ciphertext) {
    double frequencies[ALPHABET_SIZE];
    char plaintext[1000];
    char best_plaintexts[TOP_N][1000];
    double best_scores[TOP_N];
    int length = strlen(ciphertext);
    int i, j, k, l;

    for (i = 0; i < TOP_N; i++) {
        best_scores[i] = -1e9;
    }

    char test_key[ALPHABET_SIZE] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (i = 0; i < ALPHABET_SIZE; i++) {
        for (j = 0; j < ALPHABET_SIZE; j++) {
            for (k = 0; k < ALPHABET_SIZE; k++) {
                char key[ALPHABET_SIZE];
                for (l = 0; l < ALPHABET_SIZE; l++) {
                    key[l] = test_key[(l + i) % ALPHABET_SIZE];
                }
                decrypt_with_key(ciphertext, key, plaintext);
                calculate_frequencies(plaintext, frequencies);
                double score = calculate_score(frequencies);

                for (l = 0; l < TOP_N; l++) {
                    if (score > best_scores[l]) {
                        int m;
                        for (m = TOP_N - 1; m > l; m--) {
                            strcpy(best_plaintexts[m], best_plaintexts[m - 1]);
                            best_scores[m] = best_scores[m - 1];
                        }
                        strcpy(best_plaintexts[l], plaintext);
                        best_scores[l] = score;
                        break;
                    }
                }
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
    letter_frequency_attack(ciphertext);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define TOP_N 10

const double english_frequencies[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.316, 9.056, 
    2.758, 0.978, 2.560, 0.150, 1.929, 0.074
};

void compute_frequency(const char *text, double freq[ALPHABET_SIZE]) {
    int counts[ALPHABET_SIZE] = {0};
    int length = strlen(text);
    int i;
    for (i = 0; i < length; i++) {
        if (isalpha(text[i])) {
            char ch = tolower(text[i]) - 'a';
            counts[ch]++;
        }
    }
    for (i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (double)counts[i] / length * 100.0;
    }
}

double calculate_similarity(const double freq1[ALPHABET_SIZE], const double freq2[ALPHABET_SIZE]) {
    double score = 0.0;
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        score += 1.0 - fabs(freq1[i] - freq2[i]) / 100.0;
    }
    return score;
}

void generate_possible_plaintexts(const char *ciphertext, int top_n) {
    double cipher_freq[ALPHABET_SIZE];
    compute_frequency(ciphertext, cipher_freq);

    char plaintext[256];
    char mapping[ALPHABET_SIZE];
    double best_scores[TOP_N];
    char best_plaintexts[TOP_N][256];
    int i, j, k;

    for (i = 0; i < TOP_N; i++) {
        best_scores[i] = 0.0;
        best_plaintexts[i][0] = '\0';
    }

    for (i = 0; i < ALPHABET_SIZE; i++) {
        mapping[i] = 'a' + i;
    }

    for (i = 0; i < 100000; i++) {
        
        int index = 0;
        for (j = 0; ciphertext[j] != '\0'; j++) {
            if (isalpha(ciphertext[j])) {
                char ch = tolower(ciphertext[j]) - 'a';
                plaintext[index++] = mapping[ch];
            } else {
                plaintext[index++] = ciphertext[j];
            }
        }
        plaintext[index] = '\0';

        double plain_freq[ALPHABET_SIZE];
        compute_frequency(plaintext, plain_freq);

        double score = calculate_similarity(cipher_freq, plain_freq);

        for (j = 0; j < top_n; j++) {
            if (score > best_scores[j]) {
                for (k = top_n - 1; k > j; k--) {
                    best_scores[k] = best_scores[k - 1];
                    strcpy(best_plaintexts[k], best_plaintexts[k - 1]);
                }
                best_scores[j] = score;
                strcpy(best_plaintexts[j], plaintext);
                break;
            }
        }

        int l = ALPHABET_SIZE - 1;
        while (l >= 0 && mapping[l] == 'z') {
            l--;
        }
        if (l >= 0) {
            mapping[l]++;
            for (k = l + 1; k < ALPHABET_SIZE; k++) {
                mapping[k] = 'a';
            }
        }
    }

    printf("Top %d possible plaintexts:\n", top_n);
    for (i = 0; i < top_n; i++) {
        printf("%d. %s (Score: %.2f)\n", i + 1, best_plaintexts[i], best_scores[i]);
    }
}

int main() {
    char ciphertext[256];
    int top_n;

    printf("Enter ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    size_t length = strlen(ciphertext);
    if (length > 0 && ciphertext[length - 1] == '\n') {
        ciphertext[length - 1] = '\0';
    }

    printf("Enter the number of top plaintexts to display: ");
    scanf("%d", &top_n);
    if (top_n > TOP_N) {
        top_n = TOP_N; 
    }

    generate_possible_plaintexts(ciphertext, top_n);

    return 0;
}


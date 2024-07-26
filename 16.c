#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double englishFrequencies[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

void calculateCiphertextFrequencies(char *ciphertext, double *frequencies) {
    int len = strlen(ciphertext);
    int letterCounts[26] = {0};
    int totalLetters = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            letterCounts[toupper(ciphertext[i]) - 'A']++;
            totalLetters++;
        }
    }

    for (i = 0; i < 26; i++) {
        frequencies[i] = (double)letterCounts[i] / totalLetters * 100;
    }
}

void createSubstitutionMap(double *ciphertextFrequencies, char *substitutionMap) {
    int used[26] = {0};
    int i, j;

    for (i = 0; i < 26; i++) {
        double maxFrequency = -1.0;
        int maxIndex = -1;
        for (j = 0; j < 26; j++) {
            if (!used[j] && ciphertextFrequencies[j] > maxFrequency) {
                maxFrequency = ciphertextFrequencies[j];
                maxIndex = j;
            }
        }
        substitutionMap[maxIndex] = 'A' + i;
        used[maxIndex] = 1;
    }
}

void decryptWithSubstitutionMap(char *ciphertext, char *substitutionMap, char *plaintext) {
    int len = strlen(ciphertext);
    int i;

    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            plaintext[i] = substitutionMap[toupper(ciphertext[i]) - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

double calculateFrequencyScore(char *plaintext) {
    int len = strlen(plaintext);
    int letterCounts[26] = {0};
    int totalLetters = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            letterCounts[toupper(plaintext[i]) - 'A']++;
            totalLetters++;
        }
    }

    double score = 0.0;
    for (i = 0; i < 26; i++) {
        double expectedFrequency = englishFrequencies[i] * totalLetters / 100;
        double observedFrequency = letterCounts[i];
        score += ((observedFrequency - expectedFrequency) * (observedFrequency - expectedFrequency)) / expectedFrequency;
    }

    return score;
}

void frequencyAttack(char *ciphertext, int topN) {
    double ciphertextFrequencies[26];
    char substitutionMap[26];
    char possiblePlaintext[strlen(ciphertext) + 1];
    double scores[1]; 
    calculateCiphertextFrequencies(ciphertext, ciphertextFrequencies);
    createSubstitutionMap(ciphertextFrequencies, substitutionMap);
    decryptWithSubstitutionMap(ciphertext, substitutionMap, possiblePlaintext);
    scores[0] = calculateFrequencyScore(possiblePlaintext);

    printf("Top %d possible plaintexts:\n", topN);
    printf("Plaintext: %s (Score: %f)\n", possiblePlaintext, scores[0]);
}

int main() {
    char ciphertext[] = "L ORYH SURJUDPPLQJ"; 
    int topN = 10; 

    frequencyAttack(ciphertext, topN);

    return 0;
}


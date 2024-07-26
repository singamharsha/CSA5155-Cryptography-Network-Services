#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double englishFrequencies[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

void decrypt(char *ciphertext, int shift, char *plaintext) {
    int len = strlen(ciphertext);
    int i; 
    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            plaintext[i] = ((toupper(ciphertext[i]) - 'A' - shift + 26) % 26) + 'A';
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
    int len = strlen(ciphertext);
    char possiblePlaintexts[26][len + 1];
    double scores[26];
    int shift, i, j;

    for (shift = 0; shift < 26; shift++) {
        decrypt(ciphertext, shift, possiblePlaintexts[shift]);
        scores[shift] = calculateFrequencyScore(possiblePlaintexts[shift]);
    }

    for (i = 0; i < 25; i++) {
        for (j = i + 1; j < 26; j++) {
            if (scores[i] > scores[j]) {
                double tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;
                char tempText[len + 1];
                strcpy(tempText, possiblePlaintexts[i]);
                strcpy(possiblePlaintexts[i], possiblePlaintexts[j]);
                strcpy(possiblePlaintexts[j], tempText);
            }
        }
    }

    printf("Top %d possible plaintexts:\n", topN);
    for (i = 0; i < topN && i < 26; i++) {
        printf("Shift %d: %s (Score: %f)\n", i, possiblePlaintexts[i], scores[i]);
    }
}

int main() {
    char ciphertext[] = "L ORYH SURJUDPPLQJ"; 
    int topN = 10; 
    frequencyAttack(ciphertext, topN);

    return 0;
}

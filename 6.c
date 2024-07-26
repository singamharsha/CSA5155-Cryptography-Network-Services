#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ALPHABET_SIZE 26

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int a, int m) {
    int x;
    a = a % m;
    for (x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void countLetterFrequencies(char *ciphertext, int frequencies[]) {
    int i;
    for (i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            int index = tolower(ciphertext[i]) - 'a';
            frequencies[index]++;
        }
    }
}

void findMostFrequentLetters(int frequencies[], int *maxFreqIndex, int *secondFreqIndex) {
    int maxFreq = 0;
    int secondMaxFreq = 0;
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (frequencies[i] > maxFreq) {
            secondMaxFreq = maxFreq;
            *secondFreqIndex = *maxFreqIndex;
            maxFreq = frequencies[i];
            *maxFreqIndex = i;
        } else if (frequencies[i] > secondMaxFreq && frequencies[i] != maxFreq) {
            secondMaxFreq = frequencies[i];
            *secondFreqIndex = i;
        }
    }
}

void breakAffineCipher(char *ciphertext) {
    int frequencies[ALPHABET_SIZE] = {0};
    int maxFreqIndex = 0, secondFreqIndex = 0;
    countLetterFrequencies(ciphertext, frequencies);
    findMostFrequentLetters(frequencies, &maxFreqIndex, &secondFreqIndex);
    
    // Guessing 'e' (4th letter) and 't' (19th letter) as most frequent
    int possible_plaintext_indices[] = {4, 19};
    
    printf("Possible values:\n");
    int i, j;
    for (i = 0; i < sizeof(possible_plaintext_indices) / sizeof(int); i++) {
        for (j = 0; j < sizeof(possible_plaintext_indices) / sizeof(int); j++) {
            if (i != j) {
                int a_guess = (maxFreqIndex - possible_plaintext_indices[i] + ALPHABET_SIZE) % ALPHABET_SIZE;
                if (gcd(a_guess, ALPHABET_SIZE) == 1) {
                    int a_inverse = modInverse(a_guess, ALPHABET_SIZE);
                    if (a_inverse != -1) {
                        int b_guess = (secondFreqIndex - (a_inverse * possible_plaintext_indices[j] % ALPHABET_SIZE) + ALPHABET_SIZE) % ALPHABET_SIZE;
                        if (b_guess < 0) {
                            b_guess += ALPHABET_SIZE;
                        }
                        printf("a = %d, b = %d\n", a_guess, b_guess);
                    }
                }
            }
        }
    }
}

int main() {
    char ciphertext[] = "Harsha";
    printf("Ciphertext: %s\n", ciphertext);
    breakAffineCipher(ciphertext);
    return 0;
}


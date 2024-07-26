#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5
#define MAX_TEXT_SIZE 256

// Playfair cipher matrix
char key_table[SIZE][SIZE] = {
    {'M', 'F', 'H', 'I', 'K'},
    {'U', 'N', 'O', 'P', 'Q'},
    {'Z', 'V', 'W', 'X', 'Y'},
    {'E', 'L', 'A', 'R', 'G'},
    {'D', 'S', 'T', 'B', 'C'}
};

void preprocess_text(const char* input, char* output) {
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            output[j++] = toupper(input[i] == 'J' ? 'I' : input[i]);
        }
    }
    output[j] = '\0';
}

void search_in_table(char key_table[SIZE][SIZE], char a, char b, int pos[4]) {
    int i, j;
    if (a == 'J') a = 'I';
    if (b == 'J') b = 'I';

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (key_table[i][j] == a) {
                pos[0] = i;
                pos[1] = j;
            }
            if (key_table[i][j] == b) {
                pos[2] = i;
                pos[3] = j;
            }
        }
    }
}

void encrypt_playfair(const char* plain_text, char* cipher_text, char key_table[SIZE][SIZE]) {
    int i, j, pos[4];
    int length = strlen(plain_text);

    for (i = 0; i < length; i += 2) {
        if (i + 1 == length || plain_text[i] == plain_text[i + 1]) {
            // Handle odd length or duplicate letters by inserting 'X'
            cipher_text[i] = plain_text[i];
            cipher_text[i + 1] = 'X';
            length++;
        } else {
            search_in_table(key_table, plain_text[i], plain_text[i + 1], pos);

            // Same row
            if (pos[0] == pos[2]) {
                cipher_text[i] = key_table[pos[0]][(pos[1] + 1) % SIZE];
                cipher_text[i + 1] = key_table[pos[2]][(pos[3] + 1) % SIZE];
            }
            // Same column
            else if (pos[1] == pos[3]) {
                cipher_text[i] = key_table[(pos[0] + 1) % SIZE][pos[1]];
                cipher_text[i + 1] = key_table[(pos[2] + 1) % SIZE][pos[3]];
            }
            // Rectangle
            else {
                cipher_text[i] = key_table[pos[0]][pos[3]];
                cipher_text[i + 1] = key_table[pos[2]][pos[1]];
            }
        }
    }
    cipher_text[length] = '\0';
}

int main() {
    char plain_text[] = "Must see you over Cadogan West. Coming at once";
    char processed_text[MAX_TEXT_SIZE];
    char cipher_text[MAX_TEXT_SIZE];

    preprocess_text(plain_text, processed_text);

    printf("Processed text: %s\n", processed_text);

    encrypt_playfair(processed_text, cipher_text, key_table);

    printf("Encrypted text: %s\n", cipher_text);

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5
#define MAX_TEXT_SIZE 256

void prepare_key_table(const char* key, char key_table[SIZE][SIZE]) {
    int used[26] = {0};
    int i, j, k, index = 0;
    char modifiable_key[MAX_TEXT_SIZE];

    // Copy key to a modifiable array and convert to uppercase
    strncpy(modifiable_key, key, MAX_TEXT_SIZE);
    for (i = 0; i < strlen(modifiable_key); i++) {
        modifiable_key[i] = toupper(modifiable_key[i]);
    }

    // Remove duplicates from the key and create the key table
    for (i = 0; i < strlen(modifiable_key); i++) {
        if (modifiable_key[i] == 'J') modifiable_key[i] = 'I'; // Treat 'I' and 'J' as the same letter
        if (!used[modifiable_key[i] - 'A']) {
            key_table[index / SIZE][index % SIZE] = modifiable_key[i];
            used[modifiable_key[i] - 'A'] = 1;
            index++;
        }
    }

    // Fill remaining cells with the rest of the alphabet
    char ch;
    for (ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue; // Skip 'J'
        if (!used[ch - 'A']) {
            key_table[index / SIZE][index % SIZE] = ch;
            used[ch - 'A'] = 1;
            index++;
        }
    }
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

void decrypt_playfair(const char* cipher_text, char* plain_text, char key_table[SIZE][SIZE]) {
    int i, j, pos[4];

    for (i = 0; i < strlen(cipher_text); i += 2) {
        search_in_table(key_table, cipher_text[i], cipher_text[i + 1], pos);

        // Same row
        if (pos[0] == pos[2]) {
            plain_text[i] = key_table[pos[0]][(pos[1] + SIZE - 1) % SIZE];
            plain_text[i + 1] = key_table[pos[2]][(pos[3] + SIZE - 1) % SIZE];
        }
        // Same column
        else if (pos[1] == pos[3]) {
            plain_text[i] = key_table[(pos[0] + SIZE - 1) % SIZE][pos[1]];
            plain_text[i + 1] = key_table[(pos[2] + SIZE - 1) % SIZE][pos[3]];
        }
        // Rectangle
        else {
            plain_text[i] = key_table[pos[0]][pos[3]];
            plain_text[i + 1] = key_table[pos[2]][pos[1]];
        }
    }
    plain_text[strlen(cipher_text)] = '\0';
}

int main() {
    const char* keyword = "PLAYFAIREXAMPLE";
    char key_table[SIZE][SIZE];
    char cipher_text[] = "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPOBOTEIZONTXBYBNTGONEYCUZWRGDSONSXBOUYWRHEBAAHYUSEDQ";
    char plain_text[MAX_TEXT_SIZE];

    prepare_key_table(keyword, key_table);

    decrypt_playfair(cipher_text, plain_text, key_table);

    printf("Decrypted text: %s\n", plain_text);

    return 0;
}


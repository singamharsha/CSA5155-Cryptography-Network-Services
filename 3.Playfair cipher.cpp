#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void generateKeyTable(char key[], char keyTable[SIZE][SIZE]) {
    int dict[26] = {0};
    int i, j, k = 0;
    char current_char;

    for (i = 0; key[i] != '\0'; i++) {
        current_char = toupper(key[i]);
        if (current_char == 'J') current_char = 'I';
        if (dict[current_char - 'A'] == 0 && isalpha(current_char)) {
            keyTable[k / SIZE][k % SIZE] = current_char;
            dict[current_char - 'A'] = 1;
            k++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (dict[i] == 0 && (i + 'A') != 'J') {
            keyTable[k / SIZE][k % SIZE] = i + 'A';
            k++;
        }
    }
}

void search(char keyTable[SIZE][SIZE], char a, char b, int pos[]) {
    int i, j;

    if (a == 'J') a = 'I';
    if (b == 'J') b = 'I';

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == a) {
                pos[0] = i;
                pos[1] = j;
            }
            if (keyTable[i][j] == b) {
                pos[2] = i;
                pos[3] = j;
            }
        }
    }
}

void encrypt(char str[], char keyTable[SIZE][SIZE]) {
    int i, len = strlen(str), pos[4];

    for (i = 0; i < len; i += 2) {
        if (i + 1 == len || str[i] == str[i + 1]) {
            for (int j = len; j > i + 1; j--) {
                str[j] = str[j - 1];
            }
            str[i + 1] = (str[i] == 'X') ? 'Q' : 'X';
            len++;
        }
        search(keyTable, str[i], str[i + 1], pos);
        if (pos[0] == pos[2]) {
            str[i] = keyTable[pos[0]][(pos[1] + 1) % SIZE];
            str[i + 1] = keyTable[pos[2]][(pos[3] + 1) % SIZE];
        } else if (pos[1] == pos[3]) {
            str[i] = keyTable[(pos[0] + 1) % SIZE][pos[1]];
            str[i + 1] = keyTable[(pos[2] + 1) % SIZE][pos[3]];
        } else {
            str[i] = keyTable[pos[0]][pos[3]];
            str[i + 1] = keyTable[pos[2]][pos[1]];
        }
    }
}

int main() {
    char key[] = "KEYWORD";
    char keyTable[SIZE][SIZE];
    char message[100];

    printf("Enter a message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    for (int i = 0; message[i]; i++) {
        message[i] = toupper(message[i]);
    }

    generateKeyTable(key, keyTable);

    encrypt(message, keyTable);
    printf("Encrypted message: %s\n", message);

    return 0;
}

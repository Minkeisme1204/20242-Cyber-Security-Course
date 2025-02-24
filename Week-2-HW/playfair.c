#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 5
#define DEFAULT_SIGN 'X'
#define ALPHA_SIZE 26

typedef struct PlayFair {
    char keyTable[TABLE_SIZE][TABLE_SIZE];
} PlayFair;

// Function to get the length of a string manually
int get_length(char *str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

// Function to convert lowercase to uppercase
char to_uppercase(char ch) {
    if (ch >= 'a' && ch <= 'z') return ch - 'a' + 'A';
    return ch;
}

// Function to check if a character is an uppercase letter
int is_uppercase(char ch) {
    return (ch >= 'A' && ch <= 'Z');
}

// Function to remove duplicates and prepare key table
void prepareKey(char *key, PlayFair *c) {
    int used[ALPHA_SIZE] = {0};  // Track used letters
    int row = 0, col = 0;

    // Process key
    for (int i = 0; key[i] != '\0'; i++) {
        char ch = to_uppercase(key[i]);
        if (ch == 'J') ch = 'I'; // Playfair rule: Treat 'J' as 'I'

        if (is_uppercase(ch) && !used[ch - 'A']) {
            c->keyTable[row][col++] = ch;
            used[ch - 'A'] = 1;
            if (col == TABLE_SIZE) { row++; col = 0; }
        }
    }

    // Insert remaining letters (excluding 'J')
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) {
            c->keyTable[row][col++] = ch;
            used[ch - 'A'] = 1;
            if (col == TABLE_SIZE) { row++; col = 0; }
        }
    }
}

// Function to print the Playfair key table
void printKeyTable(PlayFair *c) {
    printf("Playfair Key Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            printf("%c ", c->keyTable[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to find row and column of a letter
void findPosition(PlayFair *c, char letter, int *row, int *col) {
    if (letter == 'J') letter = 'I'; // Playfair rule
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            if (c->keyTable[i][j] == letter) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to prepare plaintext for Playfair encryption
void formatPlaintext(char *input, char *formatted) {
    int len = 0, i = 0;

    while (input[i] != '\0') {
        char ch = to_uppercase(input[i]);
        if (is_uppercase(ch)) {
            if (ch == 'J') ch = 'I'; // Convert 'J' to 'I'

            if (len > 0 && formatted[len - 1] == ch) {
                formatted[len++] = DEFAULT_SIGN; // Insert 'X' if duplicate letter
            }
            formatted[len++] = ch;
        }
        i++;
    }

    if (len % 2 != 0) formatted[len++] = DEFAULT_SIGN; // Ensure even length
    formatted[len] = '\0';
}

// Function to encrypt using Playfair cipher
void encryptPlayfair(PlayFair *c, char *plaintext, char *cipherText) {
    char formatted[100];
    formatPlaintext(plaintext, formatted);
    int len = get_length(formatted);
    int index = 0, i = 0;

    while (i < len) {
        int row1, col1, row2, col2;
        findPosition(c, formatted[i], &row1, &col1);
        findPosition(c, formatted[i + 1], &row2, &col2);

        if (row1 == row2) {
            // Same row: Shift right
            cipherText[index++] = c->keyTable[row1][(col1 + 1) % TABLE_SIZE];
            cipherText[index++] = c->keyTable[row2][(col2 + 1) % TABLE_SIZE];
        } else if (col1 == col2) {
            // Same column: Shift down
            cipherText[index++] = c->keyTable[(row1 + 1) % TABLE_SIZE][col1];
            cipherText[index++] = c->keyTable[(row2 + 1) % TABLE_SIZE][col2];
        } else {
            // Rectangle swap
            cipherText[index++] = c->keyTable[row1][col2];
            cipherText[index++] = c->keyTable[row2][col1];
        }
        i += 2;
    }
    cipherText[index] = '\0'; // Null terminate the encrypted text
}

// Function to decrypt Playfair cipher
void decryptPlayfair(PlayFair *c, char *cipherText, char *decrypted) {
    int len = get_length(cipherText);
    int index = 0, i = 0;

    while (i < len) {
        int row1, col1, row2, col2;
        findPosition(c, cipherText[i], &row1, &col1);
        findPosition(c, cipherText[i + 1], &row2, &col2);

        if (row1 == row2) {
            // Same row: Shift left
            decrypted[index++] = c->keyTable[row1][(col1 - 1 + TABLE_SIZE) % TABLE_SIZE];
            decrypted[index++] = c->keyTable[row2][(col2 - 1 + TABLE_SIZE) % TABLE_SIZE];
        } else if (col1 == col2) {
            // Same column: Shift up
            decrypted[index++] = c->keyTable[(row1 - 1 + TABLE_SIZE) % TABLE_SIZE][col1];
            decrypted[index++] = c->keyTable[(row2 - 1 + TABLE_SIZE) % TABLE_SIZE][col2];
        } else {
            // Rectangle swap
            decrypted[index++] = c->keyTable[row1][col2];
            decrypted[index++] = c->keyTable[row2][col1];
        }
        i += 2;
    }
    decrypted[index] = '\0'; // Null terminate
}

int main() {
    char key[] = "JokerPlaysViolin";
    char plaintext[] = "SieuCapVipProCaoCaoTaiThuong";
    char ciphertext[100], decrypted[100];

    PlayFair playFair;
    prepareKey(key, &playFair);
    printKeyTable(&playFair);

    encryptPlayfair(&playFair, plaintext, ciphertext);
    printf("Encrypted: %s\n", ciphertext);

    decryptPlayfair(&playFair, ciphertext, decrypted);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}

#include <stdio.h>
#include <string.h>

#include "board.h"

#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define GRAY   "\033[90m"
#define RESET  "\033[0m"


// Draw the top/bottom of the board.
void print_line(void) {
    printf("+");
    for (int i = 0; i < WORD_LENGTH; i++) {
        printf("---+");
    }

    printf("\n");
}

// Print the board with colored letters.
void print_board(char board[MAX_GUESSES][WORD_LENGTH + 1], const char answer[WORD_LENGTH + 1]){
    printf("\n");
    for (int row = 0; row < MAX_GUESSES; row++) {
        print_line();
        printf("|");

        // Keep track of which letters in the answer
        // have already been matched.
        int used[WORD_LENGTH] = {0};

        // Find letters that are in the correct position.
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (col >= (int)strlen(board[row])) {
                continue;
            }
            if (board[row][col] == answer[col]) {
                used[col] = 1;
            }
        }

        // Print each letter with the correct color.
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (col >= (int)strlen(board[row])) {
                printf("   |");
                continue;
            }
            char letter = board[row][col];

            // GREEN: correct letter and correct position.
            if (letter == answer[col]) {
                printf(" %s%c%s |", GREEN, letter, RESET);
                continue;
            }

            // Look for an unused matching letter somewhere
            // else in the answer.
            int yellow_position = -1;
            for (int i = 0; i < WORD_LENGTH; i++) {
                if (!used[i] && letter == answer[i]) {
                    yellow_position = i;
                    break;
                }
            }
            // YELLOW: letter exists somewhere else.
            if (yellow_position != -1) {
                used[yellow_position] = 1;
                printf(" %s%c%s |", YELLOW, letter, RESET);
            }
            else {
                // GRAY: letter does not exist.
                printf(" %s%c%s |", GRAY, letter, RESET);
            }
        }
        printf("\n");
    }
    print_line();
}


// Print one keyboard key with its current color.
static void print_key(char letter, char status) {
    const char *color = RESET;
    if (status == 'g') {
        color = GREEN;
    }
    else if (status == 'y') {
        color = YELLOW;
    }
    else if (status == 'x') {
        color = GRAY;
    }
    printf("%s[%c]%s ", color, letter, RESET);
}


// Display the keyboard.
void print_keyboard(const char keyboard[26]) {
    const char *rows[] = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};
    printf("\n\n");
    // Print each row of the keyboard.
    for (int row = 0; row < 3; row++) {
        // Indent lower rows.
        if (row == 1) {
            printf("   ");
        }
        else if (row == 2) {
            printf("      ");
        }

        for (int i = 0; rows[row][i] != '\0'; i++) {
            char letter = rows[row][i];
            print_key(letter, keyboard[letter - 'a']);
        }
        printf("\n");
    }
}


// Update keyboard colors after a valid guess.
void update_keyboard(const char guess[WORD_LENGTH + 1], const char answer[WORD_LENGTH + 1], char keyboard[26]) {
    int used[WORD_LENGTH] = {0};
    char feedback[WORD_LENGTH] = {0};

    // Find green letters first.
    for (int i = 0; i < WORD_LENGTH; i++) {

        if (guess[i] == answer[i]) {

            feedback[i] = 'g';
            used[i] = 1;
        }
    }

    // Find yellow and gray letters.
    for (int i = 0; i < WORD_LENGTH; i++) {

        if (feedback[i] == 'g') {
            continue;
        }

        feedback[i] = 'x';

        // Look for an unused matching letter.
        for (int j = 0; j < WORD_LENGTH; j++) {

            if (!used[j] && guess[i] == answer[j]) {

                feedback[i] = 'y';
                used[j] = 1;

                break;
            }
        }
    }

    // Update keyboard, keeping the best color achieved.
    for (int i = 0; i < WORD_LENGTH; i++) {

        int index = guess[i] - 'a';

        char new_status = feedback[i];
        char old_status = keyboard[index];

        if (new_status == 'g' ||
            (new_status == 'y' && old_status != 'g') ||
            (new_status == 'x' && old_status == 0)) {

            keyboard[index] = new_status;
        }
    }
}


// Print the board while the player is typing.
void print_typing_board(char board[MAX_GUESSES][WORD_LENGTH + 1], const char current_guess[WORD_LENGTH + 1], int current_row, int cursor, const char answer[WORD_LENGTH + 1], const char *message, const char keyboard[26]) {
    printf("\033[H\033[J");
    printf("=============================\n");
    printf("          C WORDLE\n");
    printf("=============================\n\n");
    printf("Guess the %d-letter word!\n", WORD_LENGTH);
    printf("%sGreen%s  = correct position\n", GREEN, RESET);
    printf("%sYellow%s = correct letter\n", YELLOW, RESET);
    printf("%sGray%s   = not in the word\n", GRAY, RESET);
    printf("Press ESC to quit.\n\n");

    for (int row = 0; row < MAX_GUESSES; row++) {
        print_line();
        printf("|");

        // Track which answer letters have already been matched.
        int used[WORD_LENGTH] = {0};

        // Mark green letters as already matched.
        if (row != current_row) {
            for (int col = 0; col < WORD_LENGTH; col++) {
                if (board[row][col] != '\0' && board[row][col] == answer[col]) {
                    used[col] = 1;
                }
            }
        }

        // Print each box.
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (row == current_row) {
                // Highlight selected box.
                if (col == cursor) {
                    if (current_guess[col] != '\0') {
                        printf(" \033[4;36m%c\033[0m |", current_guess[col]);
                    }
                    else {
                        printf(" \033[4;36m \033[0m |");
                    }
                }
                else if (current_guess[col] != '\0') {
                    printf(" %c |", current_guess[col]);
                }
                else {
                    printf("   |");
                }
            }
            else if (board[row][col] == '\0') {
                printf("   |");
            }
            else {
                char letter = board[row][col];

                // Green: correct position.
                if (letter == answer[col]) {
                    printf(" %s%c%s |", GREEN, letter, RESET);
                }
                else {

                    // Look for an unmatched letter.
                    int yellow_position = -1;
                    for (int i = 0; i < WORD_LENGTH; i++) {
                        if (!used[i] && letter == answer[i]) {
                            yellow_position = i;
                            break;
                        }
                    }
                    if (yellow_position != -1) {
                        used[yellow_position] = 1;
                        printf(" %s%c%s |", YELLOW, letter, RESET);
                    }
                    else {
                        printf(" %s%c%s |", GRAY, letter, RESET);
                    }
                }
            }
        }
        printf("\n");
    }
    print_line();

    // Display keyboard.
    print_keyboard(keyboard);

    // Display error message.
    if (message != NULL && message[0] != '\0') {
        printf("\n%s\n", message);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#include "wordle.h"

#define MAX_WORDS 10000

// ANSI color escape sequences
#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define GRAY   "\033[90m"
#define RESET  "\033[0m"

// Draw the top/bottom of the board.
void print_line(void) {
    printf("+");
    for (int i = 0; i < WORD_LENGTH; i++) printf("---+");
    printf("\n");
}
// Print an empty board at the beginning of the game.
void print_empty_board(char board[MAX_GUESSES][WORD_LENGTH + 1]) {
    printf("\n");
    for (int row = 0; row < MAX_GUESSES; row++) {
        print_line();
        printf("|");
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (board[row][col] == '\0') {
                printf("   |");
            }
            else {
                printf(" %c |", board[row][col]);
            }
        }
        printf("\n");
    }
    print_line();
}

// Print the board with colored letters.
void print_board(char board[MAX_GUESSES][WORD_LENGTH + 1], char answer[WORD_LENGTH + 1]) {
    printf("\n");
    for (int row = 0; row < MAX_GUESSES; row++) {
        print_line();
        printf("|");

        // Keep track of which letters in the answer have already been matched.
        int used[WORD_LENGTH] = {0};

        // First pass: Find letters that are in the correct position.
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (col >= (int)strlen(board[row])) continue;
            if (board[row][col] == answer[col]) used[col] = 1;
        }

        // Second pass: Print each letter with the correct color.
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (col >= (int)strlen(board[row])) {
                printf("   |");
                continue;
            }
            char letter = board[row][col];

            // GREEN: Correct letter and correct position.
            if (letter == answer[col]) {
                printf(" %s%c%s |", GREEN, letter, RESET);
                continue;
            }

            // Look for an unused matching letter somewhere else in the answer.
            int yellow_position = -1;
            for (int i = 0; i < WORD_LENGTH; i++) {
                if (!used[i] && letter == answer[i]) {
                    yellow_position = i;
                    break;
                }
            }

            // YELLOW: Letter exists somewhere else and has not already been matched.
            if (yellow_position != -1) {
                used[yellow_position] = 1;
                printf(" %s%c%s |", YELLOW, letter, RESET);
            }
            else {
                // GRAY: No unused matching letter exists.
                printf(" %s%c%s |", GRAY, letter, RESET);
            }
        }
        printf("\n");
    }
    print_line();
}

// Print one keyboard key with its current color.
void print_key(char letter, char status) {
    const char *color = RESET;
    if (status == 'g') color = GREEN;
    else if (status == 'y') color = YELLOW;
    else if (status == 'x') color = GRAY;
    printf("%s[%c]%s ", color, letter, RESET);
}

// Display the keyboard.
void print_keyboard(char keyboard[26]) {
    const char *rows[] = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};
    printf("\n\n");

    // Print each row of the keyboard.
    for (int row = 0; row < 3; row++) {

        // Indent the lower rows to resemble a keyboard.
        if (row == 1) printf("   ");
        else if (row == 2) printf("      ");

        for (int i = 0; rows[row][i] != '\0'; i++) {
            char letter = rows[row][i];

            // Print the letter with its current color based on previous guesses.
            print_key(letter, keyboard[letter - 'a']);
        }
        printf("\n");
    }
}

// Update keyboard colors after a valid guess.
void update_keyboard(char guess[WORD_LENGTH + 1], char answer[WORD_LENGTH + 1], char keyboard[26]) {
    int used[WORD_LENGTH] = {0};
    char feedback[WORD_LENGTH] = {0};

    // First pass: identify green letters.
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == answer[i]) {
            feedback[i] = 'g';
            used[i] = 1;
        }
    }

    // Second pass: identify yellow and gray letters.
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (feedback[i] == 'g') continue;
        feedback[i] = 'x';

        // Look for an unused matching letter in the answer.
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!used[j] && guess[i] == answer[j]) {
                feedback[i] = 'y';
                used[j] = 1;
                break;
            }
        }
    }

    // Update the keyboard, preserving the best color achieved.
    for (int i = 0; i < WORD_LENGTH; i++) {
        int index = guess[i] - 'a';
        char new_status = feedback[i];
        char old_status = keyboard[index];
        if (new_status == 'g' || (new_status == 'y' && old_status != 'g') || (new_status == 'x' && old_status == 0)) keyboard[index] = new_status;
    }
}
// Print the board while the player is typing.
void print_typing_board(char board[MAX_GUESSES][WORD_LENGTH + 1], char current_guess[WORD_LENGTH + 1], int current_row, char answer[WORD_LENGTH + 1], const char *message, char keyboard[26]) {
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

        // First pass: mark green letters as already matched.
        if (row != current_row) {
            for (int col = 0; col < WORD_LENGTH; col++) {
                if (board[row][col] != '\0' &&
                    board[row][col] == answer[col]) {
                    used[col] = 1;
                }
            }
        }

        // Second pass: print each cell with its appropriate color.
        for (int col = 0; col < WORD_LENGTH; col++) {
            if (row == current_row) {

                // Display the letters currently being typed.
                if (current_guess[col] != '\0') printf(" %c |", current_guess[col]);
                else printf("   |");

            } else if (board[row][col] == '\0') {

                printf("   |");

            } else {

                char letter = board[row][col];
                // Green: correct letter in the correct position.
                if (letter == answer[col]) {
                    printf(" %s%c%s |", GREEN, letter, RESET);
                } else {
                    // Find an unmatched occurrence in the answer.
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
                    } else {
                        printf(" %s%c%s |", GRAY, letter, RESET);
                    }
                }
            }
        }
        printf("\n");
    }
    print_line();

    // Display the keyboard beneath the board.
    print_keyboard(keyboard);

    // Display any error message.
    if (message != NULL && message[0] != '\0') {
        printf("\n%s\n", message);
    }
}

// Get a guess one character at a time.
void get_guess(char guess[WORD_LENGTH + 1], char board[MAX_GUESSES][WORD_LENGTH + 1], int current_row, char answer[WORD_LENGTH + 1], const char *message, char keyboard[26]) {

    int position = 0;
    guess[0] = '\0';

    // Loop until the user submits a valid guess.
    while (1) {
        print_typing_board(board, guess, current_row, answer, message, keyboard);
        char key = _getch();

        // ESC = quit
        if (key == 27) {
            printf("\n\nGame exited.\n");
            exit(0);
        }

        // ENTER = submit when the guess is full
        if (key == '\r') {
            if (position == WORD_LENGTH) break;
            continue;
        }

        // BACKSPACE = remove the last letter
        if (key == '\b') {
            if (position > 0) {
                position--;
                guess[position] = '\0';
            }
            continue;
        }

        // Accept letters only
        if (isalpha((unsigned char)key) && position < WORD_LENGTH) {
            guess[position] = (char)tolower((unsigned char)key);
            position++;
            guess[position] = '\0';
        }
    }
}

// Load words from words.txt.
int load_words(char words[MAX_WORDS][WORD_LENGTH + 1]) {
    // Open the words.txt file for reading.
    FILE *file = fopen("words.txt", "r");
    if (file == NULL) {
        printf("Could not open words.txt\n");
        return 0;
    }

    int count = 0;
    char word[100];
    while (count < MAX_WORDS && fscanf(file, "%99s", word) == 1) {

        // Ignore words that are not the correct length.
        if (strlen(word) != WORD_LENGTH) continue;

        // Convert the word to lowercase.
        for (int i = 0; i < WORD_LENGTH; i++) word[i] = (char)tolower((unsigned char)word[i]);
        strcpy(words[count], word);
        count++;
    }
    fclose(file);
    return count;
}

// Check if a guess exists in words.txt.
int is_valid_word(char guess[WORD_LENGTH + 1], char words[MAX_WORDS][WORD_LENGTH + 1], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(guess, words[i]) == 0) return 1;
    }
    return 0;
}

// Play the Wordle game.
void play_game(void) {
    char words[MAX_WORDS][WORD_LENGTH + 1];

    int word_count = load_words(words);
    if (word_count == 0) return;

    // Seed the random number generator.
    srand((unsigned int)time(NULL));

    char answer[WORD_LENGTH + 1];

    // Randomly select a word from the list as the answer.
    strcpy(answer, words[rand() % word_count]);

    char board[MAX_GUESSES][WORD_LENGTH + 1] = {0};
    char keyboard[26] = {0};
    char message[100] = "";

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++) {
        char guess[WORD_LENGTH + 1] = {0};
        get_guess(guess, board, attempt, answer, message, keyboard);

        // Clear the previous message after submission.
        message[0] = '\0';

        // Reject guesses not in words.txt.
        if (!is_valid_word(guess, words, word_count)) {
            strcpy(message, "\033[31m That word is not in the word list. \033[0m");
            attempt--;
            continue;
        }

        // Save the valid guess.
        strcpy(board[attempt], guess);

        // Update keyboard colors based on this submitted guess.
        update_keyboard(guess, answer, keyboard);

        // Check for a win.
        if (strcmp(guess, answer) == 0) {
            print_board(board, answer);
            print_keyboard(keyboard);
            printf("\n%sYou got it!%s\n\n", GREEN, RESET);
            return;
        }
    }

    // All guesses used.
    print_board(board, answer);
    print_keyboard(keyboard);
    printf("\nThe word was: %s\n", answer);
    printf("Better luck next time!\n\n");
}
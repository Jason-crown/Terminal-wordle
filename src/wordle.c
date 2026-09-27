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
            if (col >= (int)strlen(board[row])) {
                continue;
            }

            if (board[row][col] == answer[col]) {
                used[col] = 1;
            }
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

            for (int i = 0; i < WORD_LENGTH; i++)
            {
                if (!used[i] && letter == answer[i])
                {
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

// Print the board while the player is typing.
void print_typing_board(
    char board[MAX_GUESSES][WORD_LENGTH + 1],
    char current_guess[WORD_LENGTH + 1],
    int current_row,
    char answer[WORD_LENGTH + 1],
    const char *message)
{
    printf("\033[H\033[J");

    printf("=============================\n");
    printf("          C WORDLE\n");
    printf("=============================\n");

    printf("\n");
    printf("Guess the %d-letter word!\n", WORD_LENGTH);
    printf("%sGreen%s  = correct position\n", GREEN, RESET);
    printf("%sYellow%s = correct letter\n", YELLOW, RESET);
    printf("%sGray%s   = not in the word\n", GRAY, RESET);
    printf("Press ESC to quit.\n\n");

    for (int row = 0; row < MAX_GUESSES; row++) {
        print_line();
        printf("|");

        for (int col = 0; col < WORD_LENGTH; col++) {
            if (row == current_row) {
                if (current_guess[col] != '\0') {
                    printf(" %c |", current_guess[col]);
                } else {
                    printf("   |");
                }
            } else if (board[row][col] != '\0') {
                char letter = board[row][col];

                // Determine the color using the answer.
                int color = 0; // 0 = gray, 1 = yellow, 2 = green

                if (letter == answer[col]) {
                    color = 2;
                } else {
                    for (int i = 0; i < WORD_LENGTH; i++) {
                        if (letter == answer[i]) {
                            color = 1;
                            break;
                        }
                    }
                }

                if (color == 2) {
                    printf(" %s%c%s |", GREEN, letter, RESET);
                } else if (color == 1) {
                    printf(" %s%c%s |", YELLOW, letter, RESET);
                } else {
                    printf(" %s%c%s |", GRAY, letter, RESET);
                }
            } else {
                printf("   |");
            }
        }

        printf("\n");
    }

    print_line();

    // Display any message beneath the board.
    if (message != NULL && message[0] != '\0') {
        printf("\n%s\n", message);
    }
}

// Get a guess one character at a time.
void get_guess(
    char guess[WORD_LENGTH + 1],
    char board[MAX_GUESSES][WORD_LENGTH + 1],
    int current_row,
    char answer[WORD_LENGTH + 1],
    const char *message)
{
    int position = 0;
    guess[0] = '\0';

    while (1) {
        print_typing_board(
            board,
            guess,
            current_row,
            answer,
            message
        );

        char key = _getch();

        // ESC = quit
        if (key == 27) {
            printf("\n\nGame exited.\n");
            exit(0);
        }

        // ENTER = submit when the guess is full
        if (key == '\r') {
            if (position == WORD_LENGTH) {
                break;
            }
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
            guess[position] =
                (char)tolower((unsigned char)key);

            position++;
            guess[position] = '\0';
        }
    }
}

// Load words from words.txt.
int load_words(char words[MAX_WORDS][WORD_LENGTH + 1]) {
    FILE *file = fopen("words.txt", "r");

    if (file == NULL) {
        printf("Could not open words.txt\n");
        return 0;
    }

    int count = 0;
    char word[100];

    while (count < MAX_WORDS && fscanf(file, "%99s", word) == 1) {
        // Ignore words that are not the correct length.
        if (strlen(word) != WORD_LENGTH) {
            continue;
        }

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
void play_game(void)
{
    char words[MAX_WORDS][WORD_LENGTH + 1];
    int word_count = load_words(words);

    if (word_count == 0) {
        return;
    }

    srand((unsigned int)time(NULL));

    char answer[WORD_LENGTH + 1];
    strcpy(answer, words[rand() % word_count]);

    char board[MAX_GUESSES][WORD_LENGTH + 1] = {0};
    char message[100] = "";

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++) {
        char guess[WORD_LENGTH + 1] = {0};

        get_guess(
            guess,
            board,
            attempt,
            answer,
            message
        );

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

        // Check for a win.
        if (strcmp(guess, answer) == 0) {
            print_board(board, answer);
            printf("\n%sYou got it!%s\n\n", GREEN, RESET);
            return;
        }
    }

    // All guesses used.
    print_board(board, answer);
    printf("\nThe word was: %s\n", answer);
    printf("Better luck next time!\n\n");
}
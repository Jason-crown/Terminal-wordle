#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "wordle.h"

#define MAX_WORDS 100

// ANSI color escape sequences
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
        for (int i = 0; i < WORD_LENGTH; i++) {
            word[i] = (char)tolower((unsigned char)word[i]);
        }
        strcpy(words[count], word);
        count++;
    }

    fclose(file);

    return count;
}


// Play the Wordle game.
void play_game(void) {
    char words[MAX_WORDS][WORD_LENGTH + 1];

    int word_count = load_words(words);

    if (word_count == 0) {
        return;
    }

    srand((unsigned int)time(NULL));

    // Pick a random answer.
    char answer[WORD_LENGTH + 1];

    strcpy(answer, words[rand() % word_count]);

    // The game board.
    char board[MAX_GUESSES][WORD_LENGTH + 1] = {0};

    printf("\n");
    printf("=============================\n");
    printf("          C WORDLE\n");
    printf("=============================\n");

    printf("\n");
    printf("Guess the %d-letter word!\n", WORD_LENGTH);
    printf("%sGreen%s  = correct position\n", GREEN, RESET);
    printf("%sYellow%s = correct letter\n", YELLOW, RESET);
    printf("%sGray%s   = not in the word\n", GRAY, RESET);

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++) {
        print_board(board, answer);

        printf("\nGuess %d/%d: ",
               attempt + 1,
               MAX_GUESSES);

        char guess[WORD_LENGTH + 1];
        char input[100];

        scanf("%99s", input);

        if (strlen(input) != WORD_LENGTH) {
            printf("Please enter exactly %d letters.\n", WORD_LENGTH);
            attempt--;
            continue;
        }

        // Convert input to lowercase.
        for (int i = 0; i < WORD_LENGTH; i++) {
            input[i] =
                (char)tolower((unsigned char)input[i]);
        }

        // Copy the input into guess.
        strcpy(guess, input);

        // Store the guess on the board.
        strcpy(board[attempt], guess);

        // Check if the player won.
        if (strcmp(guess, answer) == 0) {
            print_board(board, answer);

            printf("\n%sYou got it!%s\n\n",
                   GREEN,
                   RESET);

            return;
        }
    }

    // Player used all six guesses.
    print_board(board, answer);

    printf("\nThe word was: %s\n", answer);
    printf("Better luck next time!\n\n");
}
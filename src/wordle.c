#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wordle.h"
#include "board.h"
#include "input.h"
#include "words.h"

void play_game(void)
{
    char words[MAX_WORDS][WORD_LENGTH + 1];
    int word_count = load_words(words);

    if (word_count == 0) return;

    // Random number generator.
    srand((unsigned int)time(NULL));

    char answer[WORD_LENGTH + 1];
    strcpy(answer, words[rand() % word_count]);

    char board[MAX_GUESSES][WORD_LENGTH + 1] = {0};
    char keyboard[26] = {0};
    char message[100] = "";

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++) {

        char guess[WORD_LENGTH + 1] = {0};
        get_guess(guess, board, attempt, answer, message, keyboard);
        message[0] = '\0';

        // Check if the guess is in words.txt.
        if (!is_valid_word(guess, words, word_count)) {
            strcpy(message, "\033[31mThat word is not in the word list.\033[0m");

            // Invalid guesses do not use an attempt.
            attempt--;
            continue;
        }

        // Save the valid guess.
        strcpy(board[attempt], guess);

        // Update keyboard colors.
        update_keyboard(guess, answer, keyboard);

        // Check for a win.
        if (strcmp(guess, answer) == 0) {
            print_board(board, answer);
            print_keyboard(keyboard);
            printf("\n%sYou got it!%s\n\n", GREEN, RESET);
            return;
        }
    }

    // Player is out of guesses.
    print_board(board, answer);
    print_keyboard(keyboard);

    printf("\nThe word was: %s\n", answer);
    printf("Better luck next time!\n\n");
}
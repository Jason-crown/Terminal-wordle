#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

#include "input.h"
#include "board.h"

void get_guess(char guess[WORD_LENGTH + 1], char board[MAX_GUESSES][WORD_LENGTH + 1], int current_row, const char answer[WORD_LENGTH + 1], const char *message, const char keyboard[26]) {
    int cursor = 0;
    int length = 0;
    guess[0] = '\0';

    while (1) {
        print_typing_board(board, guess, current_row, cursor, answer, message, keyboard);
        int key = _getch();

        // ESC = quit
        if (key == 27) {
            printf("\n\nGame exited.\n");
            exit(0);
        }

        // Arrow keys return a prefix + scan code.
        if (key == 0 || key == 224) {
            key = _getch();
            // LEFT ARROW
            if (key == 75 && cursor > 0) {
                cursor--;
            }
            // RIGHT ARROW
            else if (key == 77 && cursor < length) {
                cursor++;
            }
            continue;
        }

        // ENTER = submit answer
        if (key == '\r') {
            if (length == WORD_LENGTH) {
                break;
            }
            continue;
        }

        // BACKSPACE = remove character
        if (key == '\b') {

            if (cursor > 0) {

                // Shift remaining letters left.
                for (int i = cursor - 1; i < length - 1; i++) {
                    guess[i] = guess[i + 1];
                }
                length--;
                cursor--;
                guess[length] = '\0';
            }
            continue;
        }

        // Accept only letters.
        if (isalpha((unsigned char)key)) {

            if (cursor < WORD_LENGTH) {

                // Convert to lowercase.
                char letter = (char)tolower((unsigned char)key);

                // Replace an existing letter or add a new one.
                guess[cursor] = letter;

                if (cursor == length) length++;

                cursor++;

                guess[length] = '\0';
            }
        }
    }
}
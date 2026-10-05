#ifndef INPUT_H
#define INPUT_H

#include "wordle.h"

void get_guess(char guess[WORD_LENGTH + 1], char board[MAX_GUESSES][WORD_LENGTH + 1], int current_row, const char answer[WORD_LENGTH + 1], const char *message, const char keyboard[26]);

#endif
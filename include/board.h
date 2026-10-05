#ifndef BOARD_H
#define BOARD_H

#include "wordle.h"

void print_line(void);
void print_board(char board[MAX_GUESSES][WORD_LENGTH + 1], const char answer[WORD_LENGTH + 1]);
void print_keyboard(const char keyboard[26]);
void update_keyboard(const char guess[WORD_LENGTH + 1], const char answer[WORD_LENGTH + 1], char keyboard[26]);
void print_typing_board(char board[MAX_GUESSES][WORD_LENGTH + 1], const char current_guess[WORD_LENGTH + 1], int current_row, int cursor, const char answer[WORD_LENGTH + 1], const char *message, const char keyboard[26]);

#endif
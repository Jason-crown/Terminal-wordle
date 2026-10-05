#ifndef WORDLE_H
#define WORDLE_H

#define WORD_LENGTH 5
#define MAX_GUESSES 6
#define MAX_WORDS 10000

#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define GRAY   "\033[90m"
#define RESET  "\033[0m"

void play_game(void);

#endif
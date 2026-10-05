#ifndef WORDS_H
#define WORDS_H

#include "wordle.h"

int load_words(char words[MAX_WORDS][WORD_LENGTH + 1]);

int is_valid_word(const char guess[WORD_LENGTH + 1],
                  char words[MAX_WORDS][WORD_LENGTH + 1],
                  int word_count);

#endif
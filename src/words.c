#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "words.h"

int load_words(char words[MAX_WORDS][WORD_LENGTH + 1])
{
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

int is_valid_word(const char guess[WORD_LENGTH + 1],
                  char words[MAX_WORDS][WORD_LENGTH + 1],
                  int word_count)
{
    for (int i = 0; i < word_count; i++) {
        if (strcmp(guess, words[i]) == 0) {
            return 1;
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "wordle.h"

#define MAX_WORDS 100

/* ANSI color escape sequences */
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define GRAY   "\033[90m"
#define RESET  "\033[0m"


void print_line(void)
{
    printf("+---+---+---+---+---+\n");
}

void print_empty_board(char board[MAX_GUESSES][WORD_LENGTH + 1])
{
    printf("\n");

    for (int row = 0; row < MAX_GUESSES; row++)
    {
        print_line();

        printf("|");

        for (int col = 0; col < WORD_LENGTH; col++)
        {
            if (board[row][col] == '\0')
            {
                printf("   |");
            }
            else
            {
                printf(" %c |", board[row][col]);
            }
        }

        printf("\n");
    }

    print_line();
}

void play_game(void)
{
    char words[MAX_WORDS][WORD_LENGTH + 1];
    int word_count = 0;

    FILE *file = fopen("words.txt", "r");

    if (file == NULL)
    {
        printf("Could not open words.txt\n");
        return;
    }

    while (word_count < MAX_WORDS &&
           fscanf(file, "%5s", words[word_count]) == 1)
    {
        word_count++;
    }

    fclose(file);

    if (word_count == 0)
    {
        printf("No words found.\n");
        return;
    }

    srand((unsigned int)time(NULL));

    char answer[WORD_LENGTH + 1];
    strcpy(answer, words[rand() % word_count]);

    char guess[WORD_LENGTH + 1];

    printf("=== C WORDLE ===\n");
    printf("Guess the 5-letter word!\n");
    printf("G = correct position\n");
    printf("Y = correct letter, wrong position\n");
    printf("- = letter not in word\n\n");

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++)
    {
        printf("Guess %d/%d: ", attempt + 1, MAX_GUESSES);

        scanf("%5s", guess);

        if (strlen(guess) != WORD_LENGTH)
        {
            printf("Please enter exactly 5 letters.\n");
            attempt--;
            continue;
        }

        int correct = 0;

        printf("Result: ");

        for (int i = 0; i < WORD_LENGTH; i++)
        {
            if (guess[i] == answer[i])
            {
                printf("G ");
                correct++;
            }
            else
            {
                int found = 0;

                for (int j = 0; j < WORD_LENGTH; j++)
                {
                    if (guess[i] == answer[j])
                    {
                        found = 1;
                        break;
                    }
                }

                if (found)
                    printf("Y ");
                else
                    printf("- ");
            }
        }

        printf("\n");

        if (correct == WORD_LENGTH)
        {
            printf("You won!\n");
            return;
        }
    }

    printf("You lost! The word was: %s\n", answer);
}
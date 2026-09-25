#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "wordle.h"

#define MAX_WORDS 100

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

void print_board(char board[MAX_GUESSES][WORD_LENGTH + 1],
                 char answer[WORD_LENGTH + 1],
                 int rows)
{
    printf("\n");

    for (int row = 0; row < MAX_GUESSES; row++)
    {
        print_line();

        printf("|");

        for (int col = 0; col < WORD_LENGTH; col++)
        {
            if (col >= strlen(board[row]))
            {
                printf("   |");
                continue;
            }

            char letter = board[row][col];

            if (letter == answer[col])
            {
                printf(" %s%c%s |", GREEN, letter, RESET);
            }
            else
            {
                int found = 0;

                for (int i = 0; i < WORD_LENGTH; i++)
                {
                    if (letter == answer[i])
                    {
                        found = 1;
                        break;
                    }
                }

                if (found)
                {
                    printf(" %s%c%s |", YELLOW, letter, RESET);
                }
                else
                {
                    printf(" %s%c%s |", GRAY, letter, RESET);
                }
            }
        }

        printf("\n");
    }

    print_line();
}

int load_words(char words[MAX_WORDS][WORD_LENGTH + 1])
{
    FILE *file = fopen("words.txt", "r");

    if (file == NULL)
    {
        printf("Could not open words.txt\n");
        return 0;
    }

    int count = 0;

    while (count < MAX_WORDS &&
           fscanf(file, "%5s", words[count]) == 1)
    {
        for (int i = 0; i < WORD_LENGTH; i++)
        {
            words[count][i] =
                (char)tolower((unsigned char)words[count][i]);
        }

        count++;
    }

    fclose(file);

    return count;
}

void play_game(void)
{
    char words[MAX_WORDS][WORD_LENGTH + 1];

    int word_count = load_words(words);

    if (word_count == 0)
    {
        return;
    }

    srand((unsigned int)time(NULL));

    char answer[WORD_LENGTH + 1];

    strcpy(answer, words[rand() % word_count]);

    char board[MAX_GUESSES][WORD_LENGTH + 1] = {0};

    printf("\n");
    printf("=============================\n");
    printf("          C WORDLE\n");
    printf("=============================\n");

    printf("\n");
    printf("Guess the 5-letter word!\n");
    printf("%sGreen%s  = correct position\n", GREEN, RESET);
    printf("%sYellow%s = correct letter\n", YELLOW, RESET);
    printf("%sGray%s   = not in the word\n", GRAY, RESET);

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++)
    {
        print_board(board, answer, attempt);

        printf("\nGuess %d/%d: ",
               attempt + 1,
               MAX_GUESSES);

        char guess[WORD_LENGTH + 1];

        scanf("%5s", guess);

        for (int i = 0; i < WORD_LENGTH; i++)
        {
            guess[i] =
                (char)tolower((unsigned char)guess[i]);
        }

        if (strlen(guess) != WORD_LENGTH)
        {
            printf("Please enter exactly 5 letters.\n");

            attempt--;
            continue;
        }

        strcpy(board[attempt], guess);

        if (strcmp(guess, answer) == 0)
        {
            print_board(board, answer, attempt + 1);

            printf("\n%sYou got it!%s\n\n",
                   GREEN,
                   RESET);

            return;
        }
    }

    print_board(board, answer, MAX_GUESSES);

    printf("\nThe word was: %s\n", answer);
    printf("Better luck next time!\n\n");
}
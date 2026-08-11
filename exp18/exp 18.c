#include <stdio.h>
#include <string.h>

#define MAX 20

char trailing[3][MAX];
int count[3] = {0, 0, 0};

/* Add a symbol to TRAILING set if it is not already present */
void addTrailing(int index, char symbol)
{
    int i;

    for (i = 0; i < count[index]; i++)
    {
        if (trailing[index][i] == symbol)
            return;
    }

    trailing[index][count[index]++] = symbol;
}

/* Compute TRAILING sets for the given grammar */
void computeTrailing()
{
    /*
        Grammar:
        E -> E + T | T
        T -> T * F | F
        F -> ( E ) | id
    */

    /* F -> ( E ) */
    addTrailing(2, ')');

    /* F -> id */
    addTrailing(2, 'i');   // 'i' represents id

    /* T -> T * F */
    addTrailing(1, '*');

    /* T -> F */
    addTrailing(1, ')');
    addTrailing(1, 'i');

    /* E -> E + T */
    addTrailing(0, '+');

    /* E -> T */
    addTrailing(0, '*');
    addTrailing(0, ')');
    addTrailing(0, 'i');
}

/* Display TRAILING sets */
void displayTrailing()
{
    int i;

    printf("\nTRAILING Sets:\n");

    printf("TRAILING(E) = { ");
    for (i = 0; i < count[0]; i++)
    {
        if (trailing[0][i] == 'i')
            printf("id ");
        else
            printf("%c ", trailing[0][i]);
    }
    printf("}\n");

    printf("TRAILING(T) = { ");
    for (i = 0; i < count[1]; i++)
    {
        if (trailing[1][i] == 'i')
            printf("id ");
        else
            printf("%c ", trailing[1][i]);
    }
    printf("}\n");

    printf("TRAILING(F) = { ");
    for (i = 0; i < count[2]; i++)
    {
        if (trailing[2][i] == 'i')
            printf("id ");
        else
            printf("%c ", trailing[2][i]);
    }
    printf("}\n");
}
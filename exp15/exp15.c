#include <stdio.h>
#include <ctype.h>

int main()
{
    char text[] = "Hello World\nThis is C Programming\nLexical Analyzer";
    int i = 0;
    int characters = 0;
    int words = 0;
    int lines = 1;
    int inWord = 0;

    while (text[i] != '\0')
    {
        characters++;

        if (text[i] == '\n')
            lines++;

        if (isspace(text[i]))
        {
            inWord = 0;
        }
        else if (inWord == 0)
        {
            words++;
            inWord = 1;
        }

        i++;
    }

    printf("Number of Characters = %d\n", characters);
    printf("Number of Words = %d\n", words);
    printf("Number of Lines = %d\n", lines);

    return 0;
}
#include <stdio.h>

int main()
{
    printf("Given Grammar:\n");
    printf("S -> (L) | a\n");
    printf("L -> L,S | S\n\n");

    printf("Grammar after Eliminating Left Recursion:\n");
    printf("S  -> (L) | a\n");
    printf("L  -> SL'\n");
    printf("L' -> ,SL' | e\n");

    return 0;
}
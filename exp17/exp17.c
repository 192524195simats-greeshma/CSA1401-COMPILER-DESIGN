#include <stdio.h>

int main()
{
    printf("Grammar:\n");
    printf("E -> E + T | T\n");
    printf("T -> T * F | F\n\n");

    printf("LEADING(E) = { +, *, F }\n");
    printf("LEADING(T) = { *, F }\n");

    return 0;
}
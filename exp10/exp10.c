#include <stdio.h>
#include <string.h>

int main() {
    char prod1[50] = "iEtS";
    char prod2[50] = "iEtSeS";

    char common[50];
    int i = 0;

    // Find the common prefix
    while (prod1[i] != '\0' && prod2[i] != '\0' && prod1[i] == prod2[i]) {
        common[i] = prod1[i];
        i++;
    }
    common[i] = '\0';

    printf("Original Grammar:\n");
    printf("S -> %s | %s | a\n", prod1, prod2);
    printf("E -> b\n");

    printf("\nGrammar after Left Factoring:\n");

    // Print new grammar
    printf("S -> %sA | a\n", common);

    if (strlen(prod2 + i) > 0)
        printf("A -> %s | e\n", prod2 + i);
    else
        printf("A -> e\n");

    printf("E -> b\n");

    return 0;
}
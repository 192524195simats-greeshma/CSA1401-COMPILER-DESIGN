#include <stdio.h>
#include <string.h>

int main() {
    char exp[50];
    char op[10];
    int i, j = 1;

    printf("Enter the expression (Example: a+b*c-d): ");
    scanf("%s", exp);

    strcpy(op, "*/+-");

    printf("\nThree Address Code:\n");

    while (strlen(exp) > 1) {
        for (i = 0; exp[i] != '\0'; i++) {

            if (strchr(op, exp[i])) {

                printf("t%d = %c %c %c\n", j,
                       exp[i - 1], exp[i], exp[i + 1]);

                exp[i - 1] = 't';
                exp[i] = '0' + j;

                int k = i + 1;
                while (exp[k + 1] != '\0') {
                    exp[k] = exp[k + 1];
                    k++;
                }
                exp[k] = '\0';

                j++;
                break;
            }
        }

        if (i == strlen(exp))
            break;
    }

    return 0;
}
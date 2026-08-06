#include <stdio.h>
#include <string.h>

char input[100];
int pos = 0;

void E();
void E1();
void T();
void T1();
void F();

void E() {
    T();
    E1();
}

void E1() {
    if (input[pos] == '+') {
        pos++;
        T();
        E1();
    }
}

void T() {
    F();
    T1();
}

void T1() {
    if (input[pos] == '*') {
        pos++;
        F();
        T1();
    }
}

void F() {
    if (input[pos] == '(') {
        pos++;
        E();
        if (input[pos] == ')')
            pos++;
        else
            printf("Missing ')'!\n");
    }
    else if (input[pos] == 'i' && input[pos + 1] == 'd') {
        pos += 2;
    }
    else {
        printf("Invalid Input!\n");
    }
}

int main() {
    printf("Enter the input string: ");
    scanf("%s", input);

    E();

    if (input[pos] == '\0')
        printf("String Accepted.\n");
    else
        printf("String Rejected.\n");

    return 0;
}
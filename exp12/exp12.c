#include <stdio.h>
#include <string.h>

char input[100];
int i = 0;

// Function declarations
void E();
void Eprime();
void T();
void Tprime();
void F();

// E ? T E'
void E() {
    T();
    Eprime();
}

// E' ? + T E' | e
void Eprime() {
    if (input[i] == '+') {
        i++;
        T();
        Eprime();
    }
}

// T ? F T'
void T() {
    F();
    Tprime();
}

// T' ? * F T' | e
void Tprime() {
    if (input[i] == '*') {
        i++;
        F();
        Tprime();
    }
}

// F ? (E) | id
void F() {
    if (input[i] == '(') {
        i++;
        E();
        if (input[i] == ')')
            i++;
        else {
            printf("Rejected: Missing ')'\n");
            return;
        }
    }
    else if (input[i] == 'i' && input[i + 1] == 'd') {
        i += 2;      // Consume "id"
    }
    else {
        printf("Rejected: Invalid Identifier\n");
    }
}

int main() {
    printf("Enter the input string: ");
    scanf("%s", input);

    E();

    if (input[i] == '\0')
        printf("String Accepted\n");
    else
        printf("String Rejected\n");

    return 0;
}
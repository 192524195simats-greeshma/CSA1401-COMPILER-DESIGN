#include <stdio.h>
#include <string.h>

#define MAX 100

struct Symbol {
    char name[30];
    char type[20];
    int size;
};

struct Symbol table[MAX];
int count = 0;

// Insert a symbol
void insert() {
    printf("Enter Symbol Name: ");
    scanf("%s", table[count].name);

    printf("Enter Data Type: ");
    scanf("%s", table[count].type);

    printf("Enter Size: ");
    scanf("%d", &table[count].size);

    count++;
    printf("Symbol inserted successfully!\n");
}

// Display symbol table
void display() {
    int i;

    if (count == 0) {
        printf("Symbol Table is Empty!\n");
        return;
    }

    printf("\n-------------------------------------------------\n");
    printf("Name\t\tType\t\tSize\n");
    printf("-------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%s\t\t%s\t\t%d\n",
               table[i].name,
               table[i].type,
               table[i].size);
    }

    printf("-------------------------------------------------\n");
}

// Search a symbol
void search() {
    char key[30];
    int i, found = 0;

    printf("Enter Symbol to Search: ");
    scanf("%s", key);

    for (i = 0; i < count; i++) {
        if (strcmp(table[i].name, key) == 0) {
            printf("Symbol Found!\n");
            printf("Name : %s\n", table[i].name);
            printf("Type : %s\n", table[i].type);
            printf("Size : %d\n", table[i].size);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Symbol Not Found!\n");
}

// Delete a symbol
void deleteSymbol() {
    char key[30];
    int i, j, found = 0;

    printf("Enter Symbol to Delete: ");
    scanf("%s", key);

    for (i = 0; i < count; i++) {
        if (strcmp(table[i].name, key) == 0) {
            found = 1;

            for (j = i; j < count - 1; j++) {
                table[j] = table[j + 1];
            }

            count--;
            printf("Symbol Deleted Successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Symbol Not Found!\n");
}

int main() {
    int choice;

    while (1) {
        printf("\n===== SYMBOL TABLE MENU =====\n");
        printf("1. Insert Symbol\n");
        printf("2. Display Symbol Table\n");
        printf("3. Search Symbol\n");
        printf("4. Delete Symbol\n");
        printf("5. Exit\n");

        printf("Enter your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insert();
                break;
            case 2:
                display();
                break;
            case 3:
                search();
                break;
            case 4:
                deleteSymbol();
                break;
            case 5:
                printf("Exiting Program...\n");
                return 0;
            default:
                printf("Invalid Choice!\n");
        }
    }

    return 0;

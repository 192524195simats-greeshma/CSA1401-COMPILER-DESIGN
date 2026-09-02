#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 200
#define MAX_TOKEN 50


/* ============================================================
   1. SYNTAX TREE
   ============================================================ */

typedef struct Node {
    char value[MAX_TOKEN];
    struct Node *left;
    struct Node *right;
} Node;


Node *createNode(const char *value, Node *left, Node *right)
{
    Node *node = (Node *)malloc(sizeof(Node));

    strcpy(node->value, value);
    node->left = left;
    node->right = right;

    return node;
}


/* ============================================================
   2. TOKENIZER
   ============================================================ */

char tokens[MAX][MAX_TOKEN];
int tokenCount = 0;
int pos = 0;


void tokenize(const char *expr)
{
    int i = 0;

    while (expr[i] != '\0') {

        if (isspace(expr[i])) {
            i++;
            continue;
        }

        /* Identifier */
        if (isalpha(expr[i]) || expr[i] == '_') {

            int j = 0;

            while (isalnum(expr[i]) || expr[i] == '_') {
                tokens[tokenCount][j++] = expr[i++];
            }

            tokens[tokenCount][j] = '\0';
            tokenCount++;
        }

        /* Number */
        else if (isdigit(expr[i]) || expr[i] == '.') {

            int j = 0;

            while (isdigit(expr[i]) || expr[i] == '.') {
                tokens[tokenCount][j++] = expr[i++];
            }

            tokens[tokenCount][j] = '\0';
            tokenCount++;
        }

        /* Operator / Parenthesis */
        else {

            tokens[tokenCount][0] = expr[i];
            tokens[tokenCount][1] = '\0';

            tokenCount++;
            i++;
        }
    }
}


char *currentToken()
{
    if (pos < tokenCount)
        return tokens[pos];

    return NULL;
}


char *consume()
{
    if (pos < tokenCount)
        return tokens[pos++];

    return NULL;
}


/* ============================================================
   3. RECURSIVE DESCENT PARSER
   ============================================================ */

Node *parseExpression();
Node *parseTerm();
Node *parsePower();
Node *parseFactor();


Node *parseFactor()
{
    char *token = currentToken();

    if (token == NULL)
        return NULL;

    /* Parenthesized expression */
    if (strcmp(token, "(") == 0) {

        consume();

        Node *node = parseExpression();

        if (currentToken() != NULL &&
            strcmp(currentToken(), ")") == 0) {
            consume();
        }

        return node;
    }

    consume();

    return createNode(token, NULL, NULL);
}


Node *parsePower()
{
    Node *left = parseFactor();

    if (currentToken() != NULL &&
        strcmp(currentToken(), "^") == 0) {

        consume();

        Node *right = parsePower();

        left = createNode("^", left, right);
    }

    return left;
}


Node *parseTerm()
{
    Node *left = parsePower();

    while (currentToken() != NULL &&
          (strcmp(currentToken(), "*") == 0 ||
           strcmp(currentToken(), "/") == 0)) {

        char op[MAX_TOKEN];

        strcpy(op, consume());

        Node *right = parsePower();

        left = createNode(op, left, right);
    }

    return left;
}


Node *parseExpression()
{
    Node *left = parseTerm();

    while (currentToken() != NULL &&
          (strcmp(currentToken(), "+") == 0 ||
           strcmp(currentToken(), "-") == 0)) {

        char op[MAX_TOKEN];

        strcpy(op, consume());

        Node *right = parseTerm();

        left = createNode(op, left, right);
    }

    return left;
}


Node *parseAssignment()
{
    char *identifier = consume();

    if (currentToken() == NULL ||
        strcmp(currentToken(), "=") != 0) {

        printf("Syntax Error: Expected '='\n");
        exit(1);
    }

    consume();

    Node *expression = parseExpression();

    return createNode(
        "=",
        createNode(identifier, NULL, NULL),
        expression
    );
}


/* ============================================================
   4. PRINT SYNTAX TREE
   ============================================================ */

void printTree(Node *root, int level, char *prefix)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("    ");

    printf("%s%s\n", prefix, root->value);

    if (root->left != NULL)
        printTree(root->left, level + 1, "L: ");

    if (root->right != NULL)
        printTree(root->right, level + 1, "R: ");
}


/* ============================================================
   5. THREE ADDRESS CODE
   ============================================================ */

typedef struct {
    char op[MAX_TOKEN];
    char arg1[MAX_TOKEN];
    char arg2[MAX_TOKEN];
    char result[MAX_TOKEN];
} TAC;

TAC tac[MAX];
int tacCount = 0;
int tempCount = 0;


void newTemp(char *temp)
{
    tempCount++;
    sprintf(temp, "t%d", tempCount);
}


char *generateTAC(Node *node)
{
    static char result[MAX_TOKEN];

    /* Leaf */
    if (node->left == NULL &&
        node->right == NULL) {

        strcpy(result, node->value);
        return result;
    }

    /* Assignment */
    if (strcmp(node->value, "=") == 0) {

        char right[MAX_TOKEN];

        strcpy(
            right,
            generateTAC(node->right)
        );

        strcpy(tac[tacCount].op, "=");
        strcpy(tac[tacCount].arg1, right);
        strcpy(tac[tacCount].arg2, "-");
        strcpy(tac[tacCount].result,
               node->left->value);

        tacCount++;

        strcpy(result, node->left->value);

        return result;
    }

    char left[MAX_TOKEN];
    char right[MAX_TOKEN];
    char temp[MAX_TOKEN];

    strcpy(left, generateTAC(node->left));
    strcpy(right, generateTAC(node->right));

    newTemp(temp);

    strcpy(tac[tacCount].op, node->value);
    strcpy(tac[tacCount].arg1, left);
    strcpy(tac[tacCount].arg2, right);
    strcpy(tac[tacCount].result, temp);

    tacCount++;

    strcpy(result, temp);

    return result;
}


/* ============================================================
   6. PRINT TAC
   ============================================================ */

void printTAC(TAC code[], int count)
{
    for (int i = 0; i < count; i++) {

        if (strcmp(code[i].op, "=") == 0) {

            printf("%s = %s\n",
                   code[i].result,
                   code[i].arg1);
        }
        else {

            printf("%s = %s %s %s\n",
                   code[i].result,
                   code[i].arg1,
                   code[i].op,
                   code[i].arg2);
        }
    }
}


/* ============================================================
   7. HELPER FUNCTIONS FOR OPTIMIZATION
   ============================================================ */

int isNumber(const char *s)
{
    if (s == NULL || s[0] == '\0')
        return 0;

    int dot = 0;

    for (int i = 0; s[i] != '\0'; i++) {

        if (s[i] == '.') {

            if (dot)
                return 0;

            dot = 1;
        }

        else if (!isdigit(s[i])) {
            return 0;
        }
    }

    return 1;
}


double calculate(char op, double a, double b)
{
    switch (op) {

        case '+':
            return a + b;

        case '-':
            return a - b;

        case '*':
            return a * b;

        case '/':
            return a / b;

        case '^': {
            double result = 1;

            for (int i = 0; i < (int)b; i++)
                result *= a;

            return result;
        }
    }

    return 0;
}


/* ============================================================
   8. CONSTANT FOLDING
   ============================================================ */

int constantFolding(TAC input[], int count,
                    TAC output[])
{
    int newCount = 0;
    int changes = 0;

    for (int i = 0; i < count; i++) {

        if (strlen(input[i].arg1) > 0 &&
            strlen(input[i].arg2) > 0 &&
            input[i].arg2[0] != '-' &&
            isNumber(input[i].arg1) &&
            isNumber(input[i].arg2) &&
            strlen(input[i].op) == 1 &&
            strchr("+-*/^", input[i].op[0])) {

            double a = atof(input[i].arg1);
            double b = atof(input[i].arg2);

            if (input[i].op[0] == '/' && b == 0) {
                output[newCount++] = input[i];
                continue;
            }

            double value =
                calculate(input[i].op[0], a, b);

            sprintf(output[newCount].op, "=");

            if (value == (int)value)
                sprintf(output[newCount].arg1,
                        "%d", (int)value);
            else
                sprintf(output[newCount].arg1,
                        "%.2f", value);

            strcpy(output[newCount].arg2, "-");
            strcpy(output[newCount].result,
                   input[i].result);

            newCount++;
            changes++;
        }

        else {
            output[newCount++] = input[i];
        }
    }

    return changes;
}


/* ============================================================
   9. COMMON SUBEXPRESSION ELIMINATION
   ============================================================ */

int commonSubexpressionElimination(
        TAC input[],
        int count,
        TAC output[])
{
    int newCount = 0;
    int changes = 0;

    for (int i = 0; i < count; i++) {

        int found = -1;

        if (strchr("+-*/^", input[i].op[0]) != NULL) {

            for (int j = 0; j < newCount; j++) {

                if (strcmp(input[i].op,
                           output[j].op) == 0 &&
                    strcmp(input[i].arg1,
                           output[j].arg1) == 0 &&
                    strcmp(input[i].arg2,
                           output[j].arg2) == 0) {

                    found = j;
                    break;
                }

                /* + and * are commutative */
                if ((input[i].op[0] == '+' ||
                     input[i].op[0] == '*') &&

                    strcmp(input[i].op,
                           output[j].op) == 0 &&

                    strcmp(input[i].arg1,
                           output[j].arg2) == 0 &&

                    strcmp(input[i].arg2,
                           output[j].arg1) == 0) {

                    found = j;
                    break;
                }
            }
        }

        if (found != -1) {

            TAC replacement;

            strcpy(replacement.op, "=");
            strcpy(replacement.arg1,
                   output[found].result);
            strcpy(replacement.arg2, "-");
            strcpy(replacement.result,
                   input[i].result);

            output[newCount++] = replacement;

            changes++;
        }

        else {
            output[newCount++] = input[i];
        }
    }

    return changes;
}


/* ============================================================
   10. DEAD CODE ELIMINATION
   ============================================================ */

int isUsedLater(
        TAC code[],
        int start,
        int count,
        const char *variable)
{
    for (int i = start; i < count; i++) {

        if (strcmp(code[i].arg1, variable) == 0 ||
            strcmp(code[i].arg2, variable) == 0) {

            return 1;
        }
    }

    return 0;
}


int deadCodeElimination(
        TAC input[],
        int count,
        TAC output[])
{
    int keep[MAX] = {0};
    int changes = 0;

    /*
       Start from finalValue.
       The final assignment is always required.
    */

    for (int i = count - 1; i >= 0; i--) {

        if (strcmp(input[i].result,
                   "finalValue") == 0) {

            keep[i] = 1;

            /* Find dependencies */
            for (int j = i - 1; j >= 0; j--) {

                if (strcmp(input[i].arg1,
                           input[j].result) == 0) {

                    keep[j] = 1;
                }

                if (strcmp(input[i].arg2,
                           input[j].result) == 0) {

                    keep[j] = 1;
                }
            }
        }
    }

    /*
       Repeat dependency analysis.
    */

    int changed = 1;

    while (changed) {

        changed = 0;

        for (int i = count - 1; i >= 0; i--) {

            if (keep[i]) {

                for (int j = i - 1; j >= 0; j--) {

                    if (strcmp(input[i].arg1,
                               input[j].result) == 0 &&
                        !keep[j]) {

                        keep[j] = 1;
                        changed = 1;
                    }

                    if (strcmp(input[i].arg2,
                               input[j].result) == 0 &&
                        !keep[j]) {

                        keep[j] = 1;
                        changed = 1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < count; i++) {

        if (keep[i]) {
            output[0 + changes] = input[i];
        }
        else {
            changes++;
        }
    }

    /*
       Rebuild correctly after counting removed instructions.
    */

    int outputCount = 0;

    for (int i = 0; i < count; i++) {

        if (keep[i]) {
            output[outputCount++] = input[i];
        }
    }

    return count - outputCount;
}


/* ============================================================
   11. QUADRUPLES
   ============================================================ */

void printQuadruples(TAC code[], int count)
{
    printf("\n");
    printf("============================================================\n");
    printf("QUADRUPLE REPRESENTATION\n");
    printf("============================================================\n");

    printf("%-6s %-10s %-15s %-15s %-15s\n",
           "No.",
           "Operator",
           "Arg1",
           "Arg2",
           "Result");

    printf("------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {

        printf("%-6d %-10s %-15s %-15s %-15s\n",
               i + 1,
               code[i].op,
               code[i].arg1,
               code[i].arg2,
               code[i].result);
    }
}


/* ============================================================
   12. TRIPLES
   ============================================================ */

void printTriples(TAC code[], int count)
{
    char tempNames[MAX][MAX_TOKEN];
    int tempIndexes[MAX];
    int tempCountLocal = 0;

    printf("\n");
    printf("============================================================\n");
    printf("TRIPLE REPRESENTATION\n");
    printf("============================================================\n");

    printf("%-6s %-10s %-15s %-15s\n",
           "No.",
           "Operator",
           "Arg1",
           "Arg2");

    printf("------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {

        char arg1[MAX_TOKEN];
        char arg2[MAX_TOKEN];

        strcpy(arg1, code[i].arg1);
        strcpy(arg2, code[i].arg2);

        for (int j = 0; j < tempCountLocal; j++) {

            if (strcmp(arg1, tempNames[j]) == 0) {
                sprintf(arg1, "(%d)", tempIndexes[j]);
            }

            if (strcmp(arg2, tempNames[j]) == 0) {
                sprintf(arg2, "(%d)", tempIndexes[j]);
            }
        }

        printf("%-6d %-10s %-15s %-15s\n",
               i,
               code[i].op,
               arg1,
               arg2);

        strcpy(tempNames[tempCountLocal],
               code[i].result);

        tempIndexes[tempCountLocal] = i;

        tempCountLocal++;
    }
}


/* ============================================================
   13. MAIN
   ============================================================ */

int main()
{
    char expression[] =
        "finalValue = ((principal * rate * time) / 100) "
        "+ (principal * (1 + rate/100)^time) - fees";

    Node *syntaxTree;

    TAC optimized1[MAX];
    TAC optimized2[MAX];
    TAC optimized3[MAX];

    int originalCount;
    int optimizedCount;

    int constantChanges;
    int cseChanges;
    int dceChanges;

    /* --------------------------------------------------------
       HEADER
       -------------------------------------------------------- */

    printf("======================================================================\n");
    printf("       INTERMEDIATE CODE GENERATION AND OPTIMIZATION\n");
    printf("======================================================================\n");

    printf("\nInput Expression:\n%s\n", expression);


    /* --------------------------------------------------------
       TOKENIZATION
       -------------------------------------------------------- */

    tokenize(expression);

    printf("\n======================================================================\n");
    printf("TOKENS\n");
    printf("======================================================================\n");

    for (int i = 0; i < tokenCount; i++) {
        printf("%s ", tokens[i]);
    }

    printf("\n");


    /* --------------------------------------------------------
       PARSING
       -------------------------------------------------------- */

    pos = 0;

    syntaxTree = parseAssignment();


    /* --------------------------------------------------------
       SYNTAX TREE
       -------------------------------------------------------- */

    printf("\n======================================================================\n");
    printf("SYNTAX TREE\n");
    printf("======================================================================\n");

    printTree(syntaxTree, 0, "");


    /* --------------------------------------------------------
       TAC
       -------------------------------------------------------- */

    tacCount = 0;
    tempCount = 0;

    generateTAC(syntaxTree);

    printf("\n======================================================================\n");
    printf("NON-OPTIMIZED THREE ADDRESS CODE\n");
    printf("======================================================================\n");

    printTAC(tac, tacCount);

    originalCount = tacCount;

    printf("\nNon-Optimized Instruction Count: %d\n",
           originalCount);


    /* --------------------------------------------------------
       CONSTANT FOLDING
       -------------------------------------------------------- */

    constantChanges =
        constantFolding(
            tac,
            tacCount,
            optimized1
        );


    /* --------------------------------------------------------
       COMMON SUBEXPRESSION ELIMINATION
       -------------------------------------------------------- */

    int count1 = tacCount;

    /*
       constantFolding currently preserves the number
       of instructions, so use count1.
    */

    cseChanges =
        commonSubexpressionElimination(
            optimized1,
            count1,
            optimized2
        );

    int count2 = count1 - cseChanges;


    /* --------------------------------------------------------
       DEAD CODE ELIMINATION
       -------------------------------------------------------- */

    dceChanges =
        deadCodeElimination(
            optimized2,
            count2,
            optimized3
        );

    optimizedCount = count2 - dceChanges;


    /* --------------------------------------------------------
       OPTIMIZED TAC
       -------------------------------------------------------- */

    printf("\n======================================================================\n");
    printf("OPTIMIZED THREE ADDRESS CODE\n");
    printf("======================================================================\n");

    printTAC(
        optimized3,
        optimizedCount
    );

    printf("\nOptimized Instruction Count: %d\n",
           optimizedCount);


    /* --------------------------------------------------------
       OPTIMIZATION SUMMARY
       -------------------------------------------------------- */

    printf("\n======================================================================\n");
    printf("OPTIMIZATION SUMMARY\n");
    printf("======================================================================\n");

    printf("Constant Folding                 : %d change(s)\n",
           constantChanges);

    printf("Common Subexpression Elimination : %d change(s)\n",
           cseChanges);

    printf("Dead Code Elimination            : %d change(s)\n",
           dceChanges);


    /* --------------------------------------------------------
       INSTRUCTION COMPARISON
       -------------------------------------------------------- */

    int reduction =
        originalCount - optimizedCount;

    double percentage = 0;

    if (originalCount > 0) {

        percentage =
            ((double)reduction /
             originalCount) * 100.0;
    }

    printf("\n======================================================================\n");
    printf("INSTRUCTION COUNT COMPARISON\n");
    printf("======================================================================\n");

    printf("Non-Optimized Instructions : %d\n",
           originalCount);

    printf("Optimized Instructions     : %d\n",
           optimizedCount);

    printf("Instructions Reduced       : %d\n",
           reduction);

    printf("Instruction Reduction      : %.2f%%\n",
           percentage);


    /* --------------------------------------------------------
       QUADRUPLES
       -------------------------------------------------------- */

    printQuadruples(
        optimized3,
        optimizedCount
    );


    /* --------------------------------------------------------
       TRIPLES
       -------------------------------------------------------- */

    printTriples(
        optimized3,
        optimizedCount
    );


    /* --------------------------------------------------------
       CONCLUSION
       -------------------------------------------------------- */

    printf("\n======================================================================\n");
    printf("CONCLUSION\n");
    printf("======================================================================\n");

    printf("The expression was successfully parsed into a syntax tree.\n");

    printf("Three-address code was generated from the syntax tree.\n");

    printf("The TAC was converted into quadruple and triple representations.\n");

    printf("Constant folding, common subexpression elimination, and\n");
    printf("dead code elimination were implemented.\n");

    printf("The optimized intermediate code reduces unnecessary operations\n");
    printf("when applicable and improves compiler efficiency.\n");

    printf("\nFinal Instruction Reduction: %.2f%%\n",
           percentage);

    return 0;
}
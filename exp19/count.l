%{
#include <stdio.h>

int chars = 0, lines = 0, words = 0;
%}

%%

[ \t]+              ;               /* Ignore spaces and tabs */
\n                  { lines++; }
"//".*              ;               /* Ignore single-line comments */
"/*"([^*]|\*+[^*/])*\*+"/"   ;      /* Ignore multi-line comments */

[a-zA-Z0-9_]+       { words++; chars += yyleng; }
.                   { chars++; }

%%

int main()
{
    yyin = fopen("sample.c", "r");

    if (yyin == NULL)
    {
        printf("Cannot open sample.c\n");
        return 1;
    }

    yylex();

    printf("Number of characters = %d\n", chars);
    printf("Number of lines = %d\n", lines);
    printf("Number of words = %d\n", words);

    fclose(yyin);
    return 0;
}

int yywrap()
{
    return 1;
}
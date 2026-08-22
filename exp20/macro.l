%{
#include <stdio.h>

int macros = 0;
int headers = 0;
%}

%%

^[ \t]*#[ \t]*define[ \t]+.*     { macros++; }
^[ \t]*#[ \t]*include[ \t]*[<"][^>"]*[>"]   { headers++; }
.|\n                              ;

%%

int main()
{
    yyin = fopen("sample.c", "r");

    if (yyin == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    yylex();

    printf("Number of Macros = %d\n", macros);
    printf("Number of Header Files = %d\n", headers);

    fclose(yyin);
    return 0;
}

int yywrap()
{
    return 1;
}
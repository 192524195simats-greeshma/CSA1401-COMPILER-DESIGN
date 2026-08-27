%{
#include <stdio.h>

int line_no = 1;
%}

%%

\n {
    printf("\n");
    line_no++;
}

[^\n]+ {
    printf("%d: %s", line_no, yytext);
}

%%

int yywrap()
{
    return 1;
}

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc < 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    yyin = fp;

    yylex();

    fclose(fp);

    return 0;
}
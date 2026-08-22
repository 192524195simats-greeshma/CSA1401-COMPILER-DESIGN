%{
#include <stdio.h>
%}

%%

[0-9]+(\.[0-9]+)?    { printf("Constant: %s\n", yytext); }
\"[^\"]*\"            ;   /* Ignore strings */
\'[^\']*\'            ;   /* Ignore character constants */
.                     ;

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
    fclose(yyin);
    return 0;
}

int yywrap()
{
    return 1;
}
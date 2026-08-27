%{
#include <stdio.h>
%}

%%

"="     { printf("Assignment Operator: =\n"); }
"+="    { printf("Assignment Operator: +=\n"); }
"-="    { printf("Assignment Operator: -=\n"); }
"*="    { printf("Assignment Operator: *=\n"); }
"/="    { printf("Assignment Operator: /=\n"); }

[ \t\n]+    ;       /* Ignore spaces, tabs and newlines */

.           ;       /* Ignore all other characters */

%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter C program:\n");
    yylex();
    return 0;
}
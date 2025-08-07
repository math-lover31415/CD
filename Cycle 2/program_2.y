%{
#include <stdio.h>
#define YYSTYPE char*
%}
%union {
    char* str;
}
%token IDENTIFIER
%token INVALID
%%
program: line
    | line program;

line: IDENTIFIER "\n" {printf("Valid\n");}
    | INVALID "\n" {printf("Invalid\n");}
%%
void yyerror(char *s){
    printf("Error: %s\n",s);
}
int main() {
    yyparse();
    return 0;
}
int yywrap() {
    return 1;
}
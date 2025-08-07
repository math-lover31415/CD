%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include <string.h>

extern int yylval;

%}
%option noyywrap
%%
[0-9]+ { yylval = atoi(yytext); return IDENTIFIER; }
[+\-*/()\n] { return yytext[0]; }
. { printf("Invalid character: %s\n", yytext); return INVALID; }
%%

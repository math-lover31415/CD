%{
#include "y.tab.h"
#include <string.h>
%}
%option noyywrap
%%
[0-9]+ { yylval.intval = atoi(yytext); return INTEGER; }
[a-zA-Z_][a-zA-Z0-9_]* {yylval.strval = strdup(yytext); return IDENTIFIER;}
[+\-*/();=] { return yytext[0]; }
. { /*Skip remaining characters */ }
%%

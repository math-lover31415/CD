%{
#include <stdlib.h>
#include "y.tab.h"
#include <string.h>
%}
%option noyywrap
%%
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext); return IDENTIFIER; }
\n { return '\n'; }
.* { yylval.str = strdup(yytext); return INVALID; }
%%

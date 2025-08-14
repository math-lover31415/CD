%{
#include "y.tab.h"
#include <string.h>
%}
%option noyywrap
%%
[0-9]+ { return INTEGER; }
for { return FOR; }
int|float|char { return TYPE; }
[a-zA-Z_][a-zA-Z0-9_]* {return IDENTIFIER;}
(==) { return RELATIONAL_OPERATOR; }
=|(\+=)|(-=)|(\*=)|(\/=)|(\|\|=)|(&&=)|(<<=)|(>>=)|(\^=)|(%=) { return ASSIGN; }
[+/%*\-] { return ARITHMETIC_OPERATOR; }
(<=)|(>=)|(!=)|[<>] { return RELATIONAL_OPERATOR; }
(\|\|)|(&&) { return LOGICAL_OPERATOR; }
(<<)|(>>)|[&|^] { return BITWISE_OPERATOR;}
\( { return LPAREN; }
\) { return RPAREN; }
\{ { return LCURLY; }
\} { return RCURLY; }
; { return SEMICOLON; }
, { return COMMA; }
! { return NOT; }
. { /*Skip remaining characters */ }
%%
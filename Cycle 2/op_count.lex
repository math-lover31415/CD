%{
    #include <stdio.h>
    int op_count = 0;
%}
%option noyywrap
%%
[+\-*/] {++op_count;}
\n {printf("Operator count = %d\n",op_count);op_count=0;}
. {/*Do nothing*/}
%%
int main(){
    yylex();
}
%{
#include <stdio.h>
#define YYSTYPE int
extern YYSTYPE yylval;
%}
%token IDENTIFIER
%token INVALID
%%
program: line
    | program line;

line: exp "\n" {printf("Result: %d\n",$1);}

exp: exp "+" term {$$ = $1+$3;}
    | exp "-" term {$$ = $1-$3;}
    | term {$$ = $1;};

term: term "*" factor {$$ = $1*$3;};
    | term "/" factor {$$ = $1/$3;};
    | factor {$$ = $1;};

factor: "(" exp ")" {$$ = $2;}
    | IDENTIFIER {$$ = $1;};
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
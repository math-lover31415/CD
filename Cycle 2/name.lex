name .*[pP][rR][aA][dD].*
%%
{name} printf("Invalid\n");
exit exit(0);
.* printf("Valid\n");
%%
int main()
{
    yylex();
}

int yywrap() {
    return 1;
}

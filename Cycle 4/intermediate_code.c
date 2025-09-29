#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char tempVar = 'A';

bool resolveOperator(char op, char input[]){
    int n = strlen(input);
    for (int i=0;i<n;++i){
        if (input[i]==op){
            if (i==0 || i==(n-1)){
                printf("Error\n");
                return false;
            }
            printf("%c\t%c\t%c\t%c\n",op,tempVar,input[i-1],input[i+1]);
            char temp[100];
            strcpy(temp,input+i+2);
            input[i-1] = tempVar++;
            strcpy(input+i,temp);
        }
    }
    return true;
}

void parse(char input[]){
    printf("Operator\tDestination\tOperand 1\tOperator 2\n");
    if (!resolveOperator('/',input)) return;
    if (!resolveOperator('*',input)) return;
    if (!resolveOperator('+',input)) return;
    if (!resolveOperator('-',input)) return;
    // Resolve = symbol
    if (strlen(input)==3 && input[1]=='='){
        printf("=\t%c\t%c\n",input[0],input[2]);
        return;
    }
    if (strlen(input)>1){
        printf("Error\n");
    }
}

int main(){
    char input[100];
    scanf("%s",input);
    parse(input);
    return 0;
}
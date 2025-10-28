#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct Stack{
    char symbol;
    struct Stack* next;
};

bool stackIsEmpty(struct Stack** indirect){
    return !(*indirect);
}

void stackPush(struct Stack** indirect, char c){
    struct Stack* newNode = malloc(sizeof(struct Stack));
    newNode->next = *indirect;
    newNode->symbol = c;
    *indirect = newNode;
}

int stackTop(struct Stack** indirect){
    if (*indirect) return (*indirect)->symbol;
    return '\0';
}

int stackPop(struct Stack** indirect){
    char out = '\0';
    if (*indirect){
        out = (*indirect)->symbol;;
        struct Stack* temp = *indirect;
        *indirect = (*indirect)->next;
        free(temp);
    }
    return out;
}

char tempVar = 'A';

int precedence(char c){
    if (c=='('){
        return 10;
    } else if (c=='+' || c=='-'){
        return 2;
    } else if (c=='*' || c=='/'){
        return 3;
    } else if (c=='^'){
        return 4;
    } else if (c=='='){
        return 1;
    }
    return -1;
}

void postFixHandle(char symbol, struct Stack** valStack){
    char op1 = ' ';
    char op2 = ' ';
    if (precedence(symbol)>0){
        op2 = stackPop(valStack);
        op1 = stackPop(valStack);
        if (symbol=='='){
            printf("%c\t%c\t%c\n",symbol,op1,op2);
        } else {
            printf("%c\t%c\t%c\t%c\n",symbol,tempVar,op1,op2);
        }
        symbol = tempVar++;
    }
    stackPush(valStack,symbol);
}

void infixHandle(char currentSymbol,struct Stack** opStack,struct Stack** valStack){
    int p = precedence(currentSymbol);
    if (p>0){ //operator
        while (!stackIsEmpty(opStack)){
            char topElem = stackTop(opStack);
            if (topElem=='(' || precedence(topElem)<p){
                break;
            }
            postFixHandle(topElem,valStack);
            stackPop(opStack);
        }
        stackPush(opStack,currentSymbol);
    } else if(currentSymbol==')'){
        while (!stackIsEmpty(opStack)){
            char topElem = stackTop(opStack);
            if (topElem=='('){
                stackPop(opStack);
                break;
            }
            postFixHandle(topElem,valStack);
            stackPop(opStack);
        }
    } else {
        postFixHandle(currentSymbol,valStack);
    }
}


void process(char expression[100]){
    int n = strlen(expression);
    struct Stack* opStack = NULL;
    struct Stack* valStack = NULL;
    for (int i=0;i<n;++i){
        infixHandle(expression[i],&opStack,&valStack);
    }
    while (!stackIsEmpty(&opStack)){
        postFixHandle(stackPop(&opStack),&valStack);
    }
    while (!stackIsEmpty(&valStack)){
        stackPop(&valStack);
    }
}

int main(){
    char expression[100];
    scanf("%s",expression);
    process(expression);
    return 0;
}
#include "shift_reduce_common.c"

char ** read_precedence(struct Grammar* g){
    int n = strlen(g->terminals)+1;
    char ** table = malloc(sizeof(char*)*n);
    for (int i=0;i<n;++i){
        table[i] = malloc(sizeof(char)*n);
        scanf("%s",table[i]);
    }
    return table;
}

void free_precedence(char** table, int n){
    for (int i=0;i<n;++i){
        free(table[i]);
    }
    free(table);
}

char findAction(struct Grammar* g,char**table, struct StackNode** outputStack,struct StackNode** inputStack){
    char lhs = stackTerminal(outputStack);
    char rhs = stackTerminal(inputStack);
    int l = find_index(g->terminals,lhs);
    int r = find_index(g->terminals,rhs);
    if (l<0) l = strlen(g->terminals);
    if (r<0) r = strlen(g->terminals);
    return table[l][r];
}



void parse(struct Grammar* g, char** table,char input[20]){
    struct StackNode * inputHead = NULL;
    struct StackNode * stackHead = NULL;

    struct StackNode** inputStack = &inputHead;
    struct StackNode** outputStack = &stackHead;
    int n = strlen(input);
    for (int i=n-1;i>=0;--i){
        stackPush(inputStack,input[i],true);
    }
    char action = '=';
    int max_iterations = 1000;
    while (max_iterations-->0 && action!='A'){
        action = findAction(g,table,outputStack,inputStack);
        
        // printf("Action: %c Input: ",action);
        // printState(inputStack);
        // printf(" Output: ");
        // printState(outputStack);
        // printf("\n");

        switch(action){
            case '>':
            case '=':
                //Same for left associative grammar
                reduce(outputStack,g);
                break;
            case '<':
                stackPush(outputStack,stackTopValue(inputStack),true);
                popStack(inputStack);
                break;
            case 'A': //Accept
                if (emptyStack(inputStack) && !emptyStack(outputStack)
                && !stackHead->next && stackTopValue(outputStack)==g->startState){
                    //Valid input
                    printf("String Accepted\n");
                } else {
                    printf("String rejected\n");
                }
                break;
        }
    }
    derivation_parse();
    freeStack(inputStack);
    freeStack(outputStack);
}

int main(){
    struct Grammar* g = read_grammar();
    char ** table = read_precedence(g);
    int n = strlen(g->terminals)+1;
    char input[20];
    scanf("%19s",input);
    if (validInput(g,input)){
        parse(g,table,input);
    } else {
        printf("Invalid input\n");
    }
    free_precedence(table,n);
    return 0;
}
#include "shift_reduce_common.c"



void parse(struct Grammar* g,char input[20]){
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
        //Try reduce
        if (reduce(outputStack,g)){
            continue;
        }

        //Try shift
        if (!emptyStack(inputStack)){
            stackPush(outputStack,stackTopValue(inputStack),true);
            popStack(inputStack);
            continue;
        }

        //Accept or reject if neither works
        if (emptyStack(inputStack) && !emptyStack(outputStack)
        && !stackHead->next && stackTopValue(outputStack)==g->startState){
            //Valid input
            printf("String Accepted\n");
        } else {
            printf("String rejected\n");
        }
        break;
    }
    while (!empty_derivation()){
        pop_derivation();
    }
    freeStack(inputStack);
    freeStack(outputStack);
}

int main(){
    struct Grammar* g = read_grammar();
    int n = strlen(g->terminals)+1;
    char input[20];
    scanf("%19s",input);
    if (validInput(g,input)){
    } else {
        printf("Invalid input\n");
    }
    return 0;
}
#include "stack.c"

bool match(struct StackNode** indirect, char s[]){
    int n = strlen(s);
    struct StackNode* current = *indirect;
    
    for (int i=n-1;i>=0;--i){
        if (!current){
            return false;
        }
        char lhs = current->symbol;
        char rhs = s[i];
        if (lhs!=rhs){
            return false;
        }

        current = current->next;
    }
    
    // Don't remove matched nodes here, that happens in reduce()
    
    return true;
}

bool shift(struct StackNode** inputStack,struct StackNode** outputStack){
    if (!emptyStack(inputStack)){
        stackPush(outputStack,stackTopValue(inputStack),true);
        popStack(inputStack);
        printf("Action: Shift Input: ");
        printState(inputStack);
        printf(" Output: ");
        printState(outputStack);
        printf("\n");
        return true;
    }
    return false;
}

bool reduce(struct StackNode** outputStack,struct StackNode** inputStack,struct Grammar* g){
    int np = g->production_num;
    bool res = false;
    for (int p=0;p<np;++p){
        char* expression = g->rules[p].expression;
        char symbol = g->rules[p].symbol;
        if (match(outputStack,expression)){
            int n = strlen(expression);
            while (n-->0){
                popStack(outputStack);
            }
            stackPush(outputStack,symbol,false);
            push_derivation(g->rules[p]);
            res = true;
            printf("Action: Reduce Input: ");
            printState(inputStack);
            printf(" Output: ");
            printState(outputStack);
            printf("\n");
        }
    }
    return res;
}

void derivation_parse(){
    printf("The RMD is as follows:\n");
    while (!empty_derivation()){
        struct ProductionRule r = top_derivation();
        printf("%c->%s\n",r.symbol,r.expression);
        pop_derivation();
    }
}
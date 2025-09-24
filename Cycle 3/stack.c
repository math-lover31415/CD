#include "grammar.c"

struct StackNode{
    char symbol;
    char firstTerminal;
    struct StackNode* next;
};


bool emptyStack(struct StackNode** indirect){
    if (*indirect) return false;
    return true;
}

void stackPush(struct StackNode** indirect,char symbol, bool terminal){
    char firstTerminal = '$';
    if (terminal){
        firstTerminal = symbol;
    } else if (*indirect){
        firstTerminal = (*indirect)->firstTerminal;
    }
    struct StackNode* st = malloc(sizeof(struct StackNode));
    st->symbol = symbol;
    st->firstTerminal = firstTerminal;
    st->next = *indirect;
    *indirect = st;
}

void popStack(struct StackNode** indirect){
    if (*indirect){
        struct StackNode* st = *indirect;
        *indirect = st->next;
        free(st);
    }
}

char stackTopValue(struct StackNode** indirect){
    if (*indirect){
        return (*indirect)->symbol;
    }
    return '$';
}

char stackTerminal(struct StackNode** indirect){
    if (!emptyStack(indirect)){
        return (*indirect)->firstTerminal;
    }
    return '$';
}
void freeStack(struct StackNode** indirect){
    while (!emptyStack(indirect)){
        popStack(indirect);
    }
}

void printState(struct StackNode** indirect){
    while (*indirect){
        printf("%c",(*indirect)->symbol);
        // printf("(%c,%c)",(*indirect)->symbol,(*indirect)->firstTerminal);
        indirect = &((*indirect)->next);
    }
    printf("$");
}
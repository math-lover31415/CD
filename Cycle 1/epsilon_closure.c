#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "nfa_ds.c"

void print_epsilon_closure(struct NFA* nfa,int state){
    bool* closure = find_epsilon_closure(nfa,state);

    printf("The epsilon closure of state %d is: {",state);
    bool flag = false;
    for (int i=0;i<nfa->stateNum;++i){
        if (!closure[i]) continue;
        if (flag){
            printf(",");
        }
        flag = true;
        printf("q%d",i);
    }
    printf("}\n");

    free(closure);
}

int main(){
    struct NFA* nfa = read_nfa();
    printNFA(nfa);
    if (!nfa) return 1;
    
    // epsilon closure
    for (int i=0;i<nfa->stateNum;++i){
        print_epsilon_closure(nfa,i);
    }

    freeNFA(nfa);
    return 0;
}
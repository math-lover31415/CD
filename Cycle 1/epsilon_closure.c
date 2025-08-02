#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "nfa_ds.c"


void dfs_closure(struct NFA* nfa,int state, bool visited[]){
    if (visited[state]) return;
    visited[state] = true;
    for (struct TransitionNode* current = (nfa->stateList[state]).transitionListHead;current;current = current->next){
        if (current->input=='e'){
            dfs_closure(nfa,current->target_state,visited);
        }
    }
}

bool* find_epsilon_closure(struct NFA* nfa, int state){
    int n = nfa->stateNum;
    int m = strlen(nfa->inputAlphabet);
    bool* closure = malloc(sizeof(bool)*n);
    for (int i=0;i<n;++i){
        closure[i] = false;
    }
    dfs_closure(nfa,state,closure);
    return closure;
}

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
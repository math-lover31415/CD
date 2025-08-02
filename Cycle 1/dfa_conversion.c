#include <stdio.h>
#include "enfa_functions.c"
#include "dfa_ds.c"

struct StateMappingNode {
    int nfa_value, dfa_value;
    int* transitions;
    struct StateMappingNode* next;
};

void freeStateMappingList(struct StateMappingNode* head){
    if (!head) return;
    freeStateMappingList(head->next);
    free(head->transitions);
    free(head);
}

void printStateMapping(struct StateMappingNode* head){
    for (struct StateMappingNode* current = head;current;current = current->next){
        printf("%d: ", current->dfa_value);
        for (int i=0,bm=current->nfa_value;bm>0;bm>>=1,++i){
            if (bm&1){
                printf("q%d",i);
            }
        }
        printf("\n");
    }
}

int stateCount = 0;

int transition(struct NFA* nfa, int state, int input){
    int out = 0;
    int copy = state;
    for (int bitCounter=0;state>0;++bitCounter,state >>= 1){
        if ((state&1)==0) continue;
        for (struct TransitionNode* current = nfa->stateList[bitCounter].transitionListHead;current;current = current->next){
            if (current->input!=nfa->inputAlphabet[input]) continue;
            out = out | (1<<(current->target_state));
        }
    }
    return out;
}

void recursiveConvert(struct NFA* nfa,struct StateMappingNode** head,int state){
    int m = strlen(nfa->inputAlphabet);

    struct StateMappingNode** indirect = head; 

    while (*indirect){
        if ((*indirect)->nfa_value==state) return;
        indirect = &((*indirect)->next);
    }
    *indirect = malloc(sizeof(struct StateMappingNode));
    (*indirect)->dfa_value = stateCount++;
    (*indirect)->nfa_value = state;
    (*indirect)->transitions = malloc(sizeof(int)*m);
    (*indirect)->next = NULL;

    for (int i=0;i<m;++i){
        int t = transition(nfa,state,i);
        recursiveConvert(nfa,head,t);
        (*indirect)->transitions[i] = t;
    }
}

int stateMapping(struct StateMappingNode* head,int nfa_state){
    while(head){
        if (head->nfa_value==nfa_state){
            return head->dfa_value;
        }
        head = head->next;
    }
    return -1;
}

struct DFA* dfa_conversion(struct NFA* enfa){
    struct NFA* nfa = epsilon_removal(enfa);
    
    int n = nfa->stateNum;
    int m = strlen(nfa->inputAlphabet);
    if (n>=32){
        printf("NFA with 32 or more states cannot be converted\n");
    }

    struct StateMappingNode* head = NULL;

    recursiveConvert(nfa,&head,1);

    char* inputAlphabet = malloc(strlen(nfa->inputAlphabet)*sizeof(char));
    strcpy(inputAlphabet,nfa->inputAlphabet);
    struct DFA* dfa = init_DFA(stateCount,inputAlphabet);
    printf("Mapping:\n");
    if (dfa){
        struct StateMappingNode* current = head;
        while (current){
            int s = current->dfa_value;
            for (int i=0;i<m;++i){
                int t = stateMapping(head,current->transitions[i]);
                dfa->transitionTable[s][i] = t;
            }
            current = current->next;
        }
    }

    freeStateMappingList(head);
    freeNFA(nfa);
    return dfa;
}

int main(){
    struct NFA* nfa = readNFA();
    if (!nfa) {
        printf("NFA creation failed\n");
    }
    printf("For the NFA:\n");
    printNFA(nfa);

    struct DFA* dfa = dfa_conversion(nfa);
    if (!dfa){
        printf("DFA conversion failed\n");
    }
    
    printf("\n\nFor the DFA:\n");
    printDFA(dfa);
    
    freeDFA(dfa);
    freeNFA(nfa);
}
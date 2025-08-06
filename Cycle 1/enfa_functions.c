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

struct NFA* epsilon_removal(struct NFA* enfa){
    int n = enfa->stateNum;
    int m = strlen(enfa->inputAlphabet);
    char* inputAlphabet = malloc(sizeof(char)*(m+1));
    if (!inputAlphabet){
        return NULL;
    }

    strcpy(inputAlphabet,enfa->inputAlphabet);
    struct NFA* outNFA = init_NFA(n,inputAlphabet);
    if (!outNFA){
        return NULL;
    }

    bool* closure_matrix[n]; //matrix[a][b] means that b is part of epsilon closure of a
    for (int i=0;i<n;++i){
        closure_matrix[i] = find_epsilon_closure(enfa,i);
    }

    for (int s=0;s<n;++s){
        for (int s1=0;s1<n;++s1){
            if (!closure_matrix[s][s1]) continue;
            for (struct TransitionNode* current = (enfa->stateList[s1]).transitionListHead;current;current = current->next){
                if (current->input=='e') continue;
                // addTransitionNFA(outNFA,s,current->target_state,current->input);
                int t = current->target_state;
                char c = current->input;
                for (int t1=0;t1<n;++t1){
                    if (!closure_matrix[t][t1]) continue;
                    //t1 is a state part of epsilon closure of t
                    addTransitionNFA(outNFA,s,t1,c);
                }
            }
        }
    }

    for (int i=0;i<n;++i){
        for (int j=0;j<n;++j){
            if (!enfa->stateList[j].finalState) continue;
            if (!closure_matrix[i][j]) continue;
            outNFA->stateList[i].finalState = true;
        }
    }

    for (int i=0;i<n;++i){
        free(closure_matrix[i]);
    }
    
    return outNFA;
}
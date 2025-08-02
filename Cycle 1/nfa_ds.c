#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct TransitionNode {
    int target_state;
    char input;
    struct TransitionNode* next;
};

struct State {
    int id;
    struct TransitionNode* transitionListHead;
};

struct NFA {
    int stateNum;
    char * inputAlphabet;
    struct State* stateList;
};

struct NFA* init_NFA(int n, char* inputAlphabet){
    struct NFA* out = malloc(sizeof(struct NFA));
    if (!out){
        return NULL; //failed allocation
    }

    out->stateNum = n;
    out->inputAlphabet = inputAlphabet;
    out->stateList = malloc(sizeof(struct State)*n);
    if (!out->stateList){
        free(out);
        return NULL;
    }

    for (int i=0;i<n;++i){
        out->stateList[i].id = i;
        out->stateList[i].transitionListHead = NULL;
    }

    return out;
}

void addTransitionNFA(struct NFA* n, int s, int t, char c){
    struct TransitionNode** head = &(n->stateList[s].transitionListHead);
    while (*head){
        if ((*head)->input==c && (*head)->target_state==t){
            return ; //avoid duplicates
        }
        head = &((*head)->next);
    }
    *head = malloc(sizeof(struct TransitionNode));
    if (!*head){
        return; // allocation failed
    }
    (*head)->target_state = t;
    (*head)->input = c;
    (*head)->next = NULL;
}

void freeStateNFA(struct State s){
    struct TransitionNode* head = s.transitionListHead;
    while (head){
        struct TransitionNode* next = head->next;
        free(head);
        head = next;
    }
}

void freeNFA(struct NFA* n){
    if (!n) return;
    for (int i=0;i<(n->stateNum);++i){
        freeStateNFA(n->stateList[i]);
    }

    free(n->inputAlphabet);
    free(n->stateList);
    free(n);
}

void printNFA(struct NFA* nfa){
    printf("The transition table is as follows:\n");
    int n = nfa->stateNum;
    int m = strlen(nfa->inputAlphabet);
    printf("\t");
    for (int i=0;i<m;++i){
        printf("%c\t",nfa->inputAlphabet[i]);
    }
    printf("epsilon\n");
    for (int i=0;i<n;++i){
        printf("q%d\t",i);
        struct State s = nfa->stateList[i];
        for (int j=0;j<=m;++j){
            char c = nfa->inputAlphabet[j];
            if (j==m){
                c = 'e';
            }
            for (struct TransitionNode *current=s.transitionListHead;current;current=current->next){
                if (current->input==c){
                    printf("q%d",current->target_state);
                }
            }
            printf("\t");
        }
        printf("\n");
    }
}

/**
 * @brief Reads an NFA (Non-deterministic Finite Automaton) definition from standard input.
 * @details
 * The function expects the following input format:
 * - Three integers: n (number of states), m (number of input characters), t (number of transitions).
 * - A string of m input characters.
 * - t transitions, each specified by two integers (source and destination states) and a character (input symbol or 'e' for epsilon).
 *   - Each state is of format qi where 0<=i<n
 * The function performs input validation, allocates memory for input characters, and initializes the NFA structure.
 * If any error occurs (invalid input, memory allocation failure, etc.), it prints an error message and returns NULL.
 *
 * @return Pointer to the initialized NFA structure, or NULL on failure.
 */
struct NFA* readNFA() {
    // read input
    int n, m, t;
    scanf("%d%d%d", &n, &m, &t);
    
    
    char* inputChars = malloc(sizeof(char)*(m+1));
    if (!inputChars) {
        printf("Failed to allocate memory for input characters\n");
        return NULL;
    }

    scanf("%s\n", inputChars);
    if (strlen(inputChars) != m) {
        free(inputChars);
        printf("Input characters length mismatch\n");
        return NULL;
    }

    struct NFA *nfa = init_NFA(n,inputChars);
    if (!nfa) {
        free(inputChars);
        printf("Failed to initialize NFA\n");
        return NULL;
    }

    for (int i = 0; i < t; ++i) {
        int a, b;
        char c;
        scanf("q%d q%d %c\n", &a, &b, &c);
        if (a < 0 || a >= n || b < 0 || b >= n) {
            printf("Invalid transition from %d to %d\n", a, b);
            freeNFA(nfa);
            return NULL;
        }
        bool validChar = false;
        for (int j = 0; j < m; ++j) {
            if (inputChars[j] == c) {
                validChar = true;
                break;
            }
        }
        if (!validChar && c != 'e') { // 'e' for epsilon transition
            printf("Invalid input character '%c' for transition from %d to %d\n", c, a, b);
            freeNFA(nfa);
            return NULL;
        }
        addTransitionNFA(nfa, a, b, c);
    }
    return nfa;
}
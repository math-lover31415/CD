#include <stdio.h>
#include "enfa_functions.c"

int main(){
    struct NFA* enfa = readNFA();
    if (!enfa) return 1;
    printNFA(enfa);
    
    // epsilon removal
    struct NFA* nfa = epsilon_removal(enfa);
    if (!nfa){
        printf("Epsilon removal failes\n");
        freeNFA(enfa);
        return 1;
    }
    printf("\n\nAfter epsilon removal...\n");
    printNFA(nfa);

    freeNFA(enfa);
    freeNFA(nfa);
    return 0;
}
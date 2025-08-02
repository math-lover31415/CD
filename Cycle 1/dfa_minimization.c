#include <stdio.h>
#include "dfa_ds.c"

int main(){
    struct DFA* dfa = readDFA();
    if (!dfa){
        printf("DFA initialization failed\n");
        return 1;
    }
    printDFA(dfa);

    struct DFA* minimizeddfa = dfsMinimization(dfa);
    if (!minimizeddfa){
        printf("DFA minimization failed\n");
        return 1;
    }
    printf("\n\nThe minimized dfa is:\n");
    printDFA(minimizeddfa);

    freeDFA(dfa);
    freeDFA(minimizeddfa);
}
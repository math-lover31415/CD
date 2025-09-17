#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct ProductionRule{
    char symbol;
    char expression[20];
};

struct Grammar{
    char startState;
    char* non_terminals;
    char* terminals;
    struct ProductionRule* rules;
    int production_num;
};

struct LMDStackNode {
    struct ProductionRule rule;
    struct LMDStackNode* next;
};

struct LMDStackNode* head = NULL;

void free_grammar(struct Grammar* g){
    if (!g) return;
    if (g->non_terminals) free(g->non_terminals);
    if (g->terminals) free(g->terminals);
    if (g->rules) free(g->rules);
    free(g);
}

bool str_contains(char str[],char c){
    int n = strlen(str);
    for (int i=0;i<n;++i){
        if (str[i]==c) return true;
    }
    return false;
}


void add_str(char str[], char c){
    int n = strlen(str);
    for (int i=0;i<n;++i){
        if (str[i]==c){
            return ;
        }
    }
    str[n] = c;
    str[n+1] = '\0';
}

bool validTerminal(struct Grammar* g, char c){
    int n = strlen(g->terminals);
    for (int i=0;i<n;++i){
        if (g->terminals[i]==c) return true;
    }
    return false;
}

bool validNonTerminal(struct Grammar* g, char c){
    int n = strlen(g->non_terminals);
    for (int i=0;i<n;++i){
        if (g->non_terminals[i]==c) return true;
    }
    return false;
}

bool validInput(struct Grammar* g, char input[]){
    int n = strlen(input);
    for (int i=0;i<n;++i){
        if (!validTerminal(g,input[i])){
            return true;
        }
    }
    return true;
}

bool validExpansion(struct Grammar* g, char input[]){
    int n = strlen(input);
    for (int i=0;i<n;++i){
        if (!validTerminal(g,input[i]) && !validNonTerminal(g,input[i])){
            return true;
        }
    }
    return true;
}

struct Grammar* read_grammar() {
    int num_non_terminal, num_terminal, num_production_rule;
    scanf("%d %d %d",&num_non_terminal,&num_terminal,&num_production_rule);

    struct Grammar* g = malloc(sizeof(struct Grammar));
    if (!g){
        printf("Coulnd't create grammar\n");
        return NULL;
    }
    scanf(" %c",&g->startState);
    if (g->startState==EOF){
        printf("Reached EOF when reading start state\n");
        free_grammar(g);
        return NULL;
    }
    g->production_num = num_production_rule;

    //Read non terminals
    g->non_terminals = malloc(sizeof(char)*num_non_terminal);
    if (!g->non_terminals){
        printf("Couldnt' allocate non terminals\n");
        free_grammar(g);
        return NULL;
    }
    for (int i=0;i<num_non_terminal;++i){
        char c;
        scanf(" %c",&c);
        if (c==EOF){
            printf("Reached EOF when reading non terminals\n");
            free_grammar(g);
            return NULL;
        }
        g->non_terminals[i] = c;
    }
    g->non_terminals[num_non_terminal] = '\0';

    //Read terminals
    g->terminals = malloc(sizeof(char)*num_terminal);
    for (int i=0;i<num_terminal;++i){
        char c;
        scanf(" %c",&c);
        if (c==EOF){
            printf("Reached EOF when reading terminals\n");
            free_grammar(g);
            return NULL;
        }
        g->terminals[i] = c;
    }
    g->terminals[num_terminal] = '\0';

    //Read Production Rules
    g->rules = malloc(sizeof(struct ProductionRule)*num_production_rule);
    if (!g){
        printf("Error reading production rules\n");
        free_grammar(g);
        return NULL;
    }
    for (int i=0;i<num_production_rule;++i){
        char rule[20];
        scanf("%s",rule);
        sscanf(rule,"%c->%s",&(g->rules[i].symbol),&g->rules[i].expression);
        if (!validNonTerminal(g,g->rules[i].symbol) || !validExpansion(g,g->rules[i].expression)){
            printf("Production rule invalid\n");
            free_grammar(g);
            return NULL;
        }
    }

    return g;
}

void push_lmd(struct ProductionRule r){
    struct LMDStackNode* n = malloc(sizeof(struct LMDStackNode));
    n->next = head;
    n->rule = r;
    head = n;
}

bool empty_lmd(){
    if (head) return false;
    return true;
}

void pop_lmd(){
    if (!head) return;
    struct LMDStackNode* n = head->next;
    free(head);
    head = n;
}

struct ProductionRule top_lmd(){
    return head->rule;
}


void print_delete_lmd(){
    if (empty_lmd()) return;
    struct ProductionRule p = top_lmd();
    pop_lmd();
    print_delete_lmd();
    printf("%c->%s\n",p.symbol,p.expression);
}
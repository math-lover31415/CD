#include "grammar.c"

int recursiveDescent(struct Grammar* g, char input[],int inputStart, char expanded[], int expandedStart){
    if (input[inputStart]=='\0'){
        return expanded[expandedStart]=='\0';
    }
    if (expanded[expandedStart]=='\0'){
        return false;
    }

    if (input[inputStart]==expanded[expandedStart]){
        return recursiveDescent(g, input, inputStart + 1, expanded, expandedStart + 1);
    }
    
    char current = expanded[expandedStart];
    for (int i=0;i<g->production_num;++i){
        if (current==g->rules[i].symbol){
            char expanded_copy[100];
            strcpy(expanded_copy,expanded);

            expanded[expandedStart] = '\0';
            strcat(expanded,g->rules[i].expression);
            strcat(expanded,expanded_copy+expandedStart+1);
            push_derivation(g->rules[i]);

            if (recursiveDescent(g,input,inputStart,expanded,expandedStart)) {
                return true;
            }

            pop_derivation();
            strcpy(expanded,expanded_copy);
        }
    }
    return false;
}

bool parse(struct Grammar* g,char input[]){
    char expanded[100];
    expanded[0] = g->startState;
    expanded[1] = '\0';
    return recursiveDescent(g,input,0,expanded,0);
}

int main(){
    struct Grammar* g = read_grammar();
    char input[20];
    scanf("%s",input);
    if (parse(g,input)){
        printf("String accepted\n");
    } else {
        printf("String rejected\n");
    }
    free(g);
    print_delete_derivation();
}
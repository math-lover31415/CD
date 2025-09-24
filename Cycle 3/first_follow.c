#include "grammar.c"

int n,m;
bool changed;

void update(int** matrix, int i, int j, bool new){
    if (!new || matrix[i][j]) return;
    matrix[i][j] = true;
    changed = true;
}

void update_set(int* st1, int* st2, int size){
    for (int i=0;i<size;++i){
        if (!st1[i] && st2[i]){
            st1[i] = true;
            changed = true;
        }
    }
}

void find_first(struct Grammar* g, int** first){
    changed = true;
    int production_num = g->production_num;
    while (changed){
        changed = false;
        for (int p=0;p<production_num;++p){
            int X = find_index(g->non_terminals,g->rules[p].symbol);
            char* expression = g->rules[p].expression;
            if (X<0){
                continue;
            }
            int k = strlen(expression);
            if (k==1 && expression[0]=='e'){
                update(first,X,m,true);
                continue;
            }
            
            bool nullable = true;
            for (int i=0;i<k;++i){
                int Y = find_index(g->non_terminals,expression[i]);
                if (Y<0){
                    // Terminal encountered
                    int t = find_index(g->terminals,expression[i]);
                    update(first,X,t,true);
                    nullable = false;
                    break;
                }
                
                // Add all symbols from FIRST(Y) to FIRST(X) except epsilon
                update_set(first[X], first[Y], m);
                
                // If Y doesn't have epsilon, stop
                if (!first[Y][m]){
                    nullable = false;
                    break;
                }
            }
            
            if (nullable){
                update(first,X,m,true);
            }
        }
    }
}

void find_follow(struct Grammar* g, int** first,int** follow){
    changed = true;
    int production_num = g->production_num;
    // Add $ to follow set of start symbol
    int start_idx = find_index(g->non_terminals, g->startState);
    if (start_idx >= 0) {
        follow[start_idx][m] = true;
    }
    while (changed){
        changed = false;
        for (int p=0;p<production_num;++p){
            int X = find_index(g->non_terminals,g->rules[p].symbol);
            char* expression = g->rules[p].expression;
            if (X<0){
                continue;
            }
            int k = strlen(expression);
            for (int i=0;i<k;++i){
                int Y = find_index(g->non_terminals,expression[i]);
                if (Y<0){
                    continue;
                }
                bool nullable = true;
                for (int j=i+1;j<k && nullable;++j){
                    int Z = find_index(g->non_terminals,expression[j]);
                    if (Z<0){
                        int t = find_index(g->terminals,expression[j]);
                        update(follow,Y,t,true);
                        nullable = false;
                        break;
                    }
                    if (!first[Z][m]){
                        nullable = false;
                    }
                    update_set(follow[Y],first[Z],m);
                }
                if (nullable){
                    update_set(follow[Y],follow[X],m+1); //Include m representing $
                }
            }
        }
    }
}

void find_first_follow(struct Grammar* g){
    n = strlen(g->non_terminals);
    m = strlen(g->terminals);
    g->terminals[m] = 'e';
    g->terminals[m+1] = '\0';

    int** first = malloc(sizeof(int*)*n);
    int** follow = malloc(sizeof(int*)*n);
    for (int i=0;i<n;++i){
        first[i] = malloc(sizeof(int)*(m+1));
        follow[i] = malloc(sizeof(int)*(m+1));
        for (int j=0;j<=m;++j){
            first[i][j] = 0;
            follow[i][j] = 0;
        }
    }

    find_first(g,first);
    find_follow(g,first,follow);

    for (int i=0;i<n;++i){
        printf("First(%c) = {",g->non_terminals[i]);
        bool flag = false;
        for (int j=0;j<=m;++j){
            if (!first[i][j]) continue;
            if (flag){
                printf(",");
            }
            flag = true;
            char c = 'e';
            if (j!=m){
                c = g->terminals[j];
            }
            printf("%c",c);
        }
        printf("}\n");

        printf("Follow(%c) = {",g->non_terminals[i]);
        flag = false;
        for (int j=0;j<=m;++j){
            if (!follow[i][j]) continue;
            if (flag){
                printf(",");
            }
            flag = true;
            char c = '$';
            if (j!=m){
                c = g->terminals[j];
            }
            printf("%c",c);
        }
        printf("}\n");
    }

    for (int i=0;i<n;++i){
        free(first[i]);
        free(follow[i]);
    }
    free(first);
    free(follow);
}

int main(){
    struct Grammar* g = read_grammar();
    find_first_follow(g);
    free(g);
    print_delete_lmd();
    return 0;
}
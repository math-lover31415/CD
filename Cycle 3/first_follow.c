#include "grammar.c"

int n,m;

bool update(int** matrix, int i, int j, bool new){
    if (!new || matrix[i][j]) return false;
    matrix[i][j] = true;
    return true;
}

int update_set(int* st1, int* st2, int size){
    bool changed = false;
    for (int i=0;i<size;++i){
        if (!st1[i] && st2[i]){
            st1[i] = true;
            changed = true;
        }
    }
    return changed;
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
        if (g->non_terminals[i]==g->startState){
            follow[i][m] = 1;
        }
    }

    bool changed = true;
    int production_num = g->production_num;
    int max_iterations = 100;
    while (changed && max_iterations-->0){
        changed = false;
        for (int p=0;p<production_num;++p){
            char symbol = g->rules[p].symbol;
            int symbol_index = find_index(g->non_terminals,symbol);
            if (symbol_index==-1){
                continue;
            }
            char* expression = g->rules[p].expression;
            int k = strlen(expression);
            if (k==1 && expression[0]=='e'){
                changed = update(first,symbol_index,m,true);
                continue;
            }
            bool nullable_i = true;
            for (int i=0;i<k;++i){
                int index = find_index(g->non_terminals,expression[i]);
                if (nullable_i){
                    int t_index = find_index(g->terminals,expression[i]);
                    if (t_index!=-1 && !first[symbol_index][t_index]){
                        first[symbol_index][t_index] = true;
                        changed = true;
                    }
                    if (index!=-1){
                        changed = changed || update_set(first[symbol_index],first[index], m+1);
                    }
                }
                if (index==-1 || first[index][m]){
                    nullable_i = false;
                }
                bool nullable_j = true;
                for (int j=i+1;j<k;++j){
                    int index_2 = find_index(g->non_terminals,expression[j]);
                    if (nullable_j){
                        int t_index = find_index(g->terminals,expression[j]);
                        if (t_index!=-1 && !first[index][t_index]){
                            follow[index][t_index] = true;
                            changed = true;
                        }
                        if (index!=-1 && index_2!=-1){
                            changed = changed || update_set(follow[index],follow[index_2], m+1);
                        }
                    }
                    if (index_2==-1 || first[index_2][m]){
                        nullable_j = false;
                        break;
                    }
                }
                if (nullable_j && index!=-1){
                    changed = changed || update_set(follow[index],follow[symbol_index], m+1);
                }
            }
            if (nullable_i){
                first[symbol_index][m] = true;
            }
        }
    }

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
#include "grammar.c"

int n,m;

void find_first(struct Grammar* g, int first[n][m],int nonTerm, int term){
    if (first[nonTerm][term]!=-1) return first[nonTerm][term];
    first[nonTerm][term] = -2;
    int res = 0;
    int uncertain = 0;
    for (int p=0;p<(g->production_num) && res<0;++p){
        if (g->rules[p].symbol!=g->non_terminals[nonTerm]){
            continue;
        }
        bool le = true;
        int m = strlen(g->rules[p].expression);
        for (int i=0;i<m && le && res==0;++i){
            char c = g->rules[p].expression[i];
            if (validTerminal(g,c)){
                le = false;
                if (c==g->terminals[term]){
                    res = 1;
                }
            } else {
                int nt;
                for (nt=0;nt<n;++nt){
                    if (g->)
                }
                find_first(g,)
            }
        }
    }
}

void find_first_follow(struct Grammar* g){
    n = strlen(g->non_terminals);
    m = strlen(g->terminals);
    g->terminals[m] = 'e';
    g->terminals[m+1] = '\0';
    int first[n][m+1], follow[n][m+1];
    for (int i=0;i,n;++i){
        for (int j=0;j<m;++j){
            first[i][j] = -1;
            follow[i][j] = -1;
        }
        follow[i][m] = -1;
    }
    for(int i=0;i<n;++i){
        if (g->non_terminals[i]==g->startState){
            follow[i][m] = 1;
        } else {
            follow[i][m] = 0;
        }
    }
    for (int j=0;j<m;++j){
        for (int i=0;i<n;++i){
            for (int k=0;k<(n*m);++k){
                find_first(g,first,i,j);
                find_follow(g,first,follow,i,j);
            }
            if (find_first(g,first,follow,i,j)==-2){
                first[i][j] = 0;
            }
            if (find_follow(g,first,follow,i,j)==-2){
                follow[i][j] = 0;
            }
        }
        find_follow(g,first,follow,i,m);
    }
}

int main(){
    struct Grammar* g = read_grammar();

    free(g);
    print_delete_lmd();
    return 0;
}
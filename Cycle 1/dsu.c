#include <stdlib.h>
#include <stdbool.h>

struct DSU{
    int total,size,*parent,*rank;
};
void freeDSU(struct DSU* dsu){
    if (!dsu) return;
    if (dsu->parent) free(dsu->parent);
    if (dsu->rank) free(dsu->rank);
    free(dsu);
}

struct DSU* initDSU(int n){
    struct DSU* dsu = malloc(sizeof(struct DSU));
    if (!dsu){
        printf("DSU initialization failed\n");
        return NULL;
    }
    dsu->total = n;
    dsu->size = n;
    dsu->parent = malloc(n*sizeof(int));
    dsu->rank = malloc(n*sizeof(int));
    if (!dsu->parent || !dsu->rank){
        freeDSU(dsu);
        printf("DSU initialization failed\n");
        return NULL;
    }
    for (int i=0;i<n;++i){
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
    return dsu;
}

int findDSU(struct DSU* dsu, int x){
    if (x==(dsu->parent[x])) return x;
    return x = findDSU(dsu,dsu->parent[x]);
}
bool mergeDSU(struct DSU* dsu, int x, int y){
    x = findDSU(dsu,x);
    y = findDSU(dsu,y);
    if (x==y) return false;

    if (dsu->rank[x]>dsu->rank[y]){
        int temp = x;
        x = y;
        y = temp;
    }
    dsu->parent[x] = y;
    if (dsu->rank[x]==dsu->rank[y]){
        ++(dsu->rank[y]);
        --(dsu->size);
    }

    return true;
}
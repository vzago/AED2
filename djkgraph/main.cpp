#include "dijkstra.h"

int main(){
    int n, i, x, y, weight;
    Graph* G;
    if (scanf("%d\n", &n) == 1) {
        G = graph_init(n);
        while (scanf("%d %d %d\n", &x, &y, &weight) != EOF) {
        //insertEdge(D, x, y, weight);
            insert_arc(G,x,y,weight);
        }
    }
    for(int i=0;i<n;i++){
        dijkstra(G,i);
    }
}   


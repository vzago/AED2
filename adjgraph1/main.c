#include <stdio.h>
#include "adjgraph.h"

int main (){
    int number_of_vertices;
    int v, w;
    scanf("%d",&number_of_vertices);
    Graph *G = graph_init(number_of_vertices);

    for(int i = 0; i <= number_of_vertices; i++){
        scanf("%d %d",&v,&w);
        insert_arc(G,v,w);
    }

    //print_graph(G);
    //destroy_graph(G);
    initDFS(G,0);
    initBFS(G,0);

}
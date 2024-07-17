#include <stdio.h>
#include <stdlib.h>
#include "adjgraph.h"


#define vertex int 

static link *fill_arr_zeros(int rows, int columns, int val){
    link *matrix = malloc(sizeof(link)*rows);
    
    for(vertex k = 0; k < rows; k++){
        matrix[k] = malloc(sizeof(Matrix)*columns);
    }
    
    for(vertex v = 0; v < rows; v++){
        
        for(vertex w = 0; w < columns; w++){
            matrix[v][w].w = val; //Val is the number which will be filled in the matrix
        }
    }
    
    return matrix;
}


Graph *graph_init(int n){
    Graph *G = malloc(sizeof(Graph));

    if(G == NULL){
        exit(1);
    }
    
    G->V = n;
    G->A = 0;
    G->adj = fill_arr_zeros(n,n,0);
    return G;
}

void insert_arc(Graph *G, vertex v, vertex w){
    
    if(G->adj[v][w].w == 0){
        G->adj[v][w].w = 1;
        G->adj[w][v].w = 1;
        G->A++;
    }
}

void remove_arc(Graph *G, vertex v, vertex w){
   
    if(G->adj[v][w].w == 1){
        G->adj[v][w].w = 0;
        G->adj[w][v].w = 0;
        G->A--;
    }
}

void destroy_graph(Graph *G){
    if(G != NULL){
        for(int i = 0; i < G->V; i++){
            free(G->adj[i]);
        }
    }
    free(G->adj);
    G->V = 0;
    G->A = 0;
    free(G);
    
}

void print_graph(Graph *G){
    printf("Total of vertices: %d, total of arcs: %d\n", G->V, G->A);
    
    for(vertex v = 0; v < G->V; v++){
        printf("Vertex %d, arcs: ", v);
        int first_arc = 1; 
        
        for(vertex w = 0; w < G->V; w++){
            if(G->adj[v][w].w == 1){
                if (!first_arc) { 
                    printf(", ");
                } else {
                    first_arc = 0; 
                }
                printf("%d", w);
            }
        }
        printf("\n");
    }
}

void clear_visited(Graph *G){
    for(int i = 0; i < G->V; i++){
        for(int j = 0; j < G->V; j++){
            G->adj[i][j].visited = false;
            G->adj[j][i].visited = false;
            
        }
    }
}

void DFS(Graph *G, vertex v){
    printf("visited %d\n",v);
    for(int i = 0; i < G->V;i++){
        if(G->adj[i][v].w == 1 ){
            G->adj[i][v].visited = true;
        }
    }
    for(int j = 0; j < G->V; j++){
        if(G->adj[v][j].w == 1 && G->adj[v][j].visited == false){
            G->adj[v][j].visited = true;
            DFS(G,j);
        }
    }

}

void initDFS(Graph *G, vertex v){
    clear_visited(G);
    printf("DFS\n");
    DFS(G,v);
}

void BFS(Graph *G, vertex v){
    int first_one = 0, last_one = 0;
    int *queue = malloc(sizeof(int) * G->V);
    queue[last_one] = v;
    last_one++;

    while(last_one != first_one){
        vertex current = queue[first_one];
        first_one++;
        printf("visited %d\n", current);
        for(int i = 0; i < G->V; i++){
            if(G->adj[current][i].w == 1 && G->adj[current][i].visited == false){
                G->adj[current][i].visited = true;
                
                for(int j = i; j < G->V; j++){
                    if(G->adj[j][current].w == 1 && G->adj[j][current].visited == false){
                        if(j!=queue[first_one]){
                            queue[last_one] = j;
                            last_one++;
                            G->adj[j][current].visited = true;
                        }
                    }
                }
            }
        }
    }
    free(queue);
}

void initBFS(Graph *G, vertex v){
    clear_visited(G);
    printf("BFS\n");
    BFS(G,v);
}
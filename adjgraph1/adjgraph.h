#ifndef GRAPH_H
#define GRAPH_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define vertex int

typedef struct Matrix *link;

typedef struct Matrix{
    vertex w;
    bool visited;
}Matrix;

typedef struct graph {  
    int V; //number of vertices
    int A; //number of arcs
    link *adj;
}Graph;

static link *fill_arr_zeros(int rows, int columns, int val);
Graph *graph_init(int n);
void insert_arc(Graph *G, vertex v, vertex w);
void remove_arc(Graph *G, vertex v, vertex w);
void print_graph(Graph *G);
void destroy_graph(Graph *G);
void clear_visited(Graph *G);
void DFS(Graph *G, vertex v);
void initDFS(Graph *G, vertex v);
void BFS (Graph *G, vertex v);
void initBFS(Graph *G, vertex v);






#endif
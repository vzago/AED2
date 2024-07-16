#ifndef GRAPH_H
#define GRAPH_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <queue>
#include <iostream>
using namespace std;

#define vertex int

typedef struct Matrix *link;

typedef struct Matrix{
    vertex w;
    bool visited;
    int weight;
}Matrix;

typedef struct graph {  
    int V; //number of vertices
    int A; //number of arcs
    link *adj;
}Graph;

Graph *graph_init(int n);
void insert_arc(Graph *G, vertex v, vertex w, int weight);
void remove_arc(Graph *G, vertex v, vertex w, int weight);
void print_graph(Graph *G);
void destroy_graph(Graph *G);
void clear_visited(Graph *G);
void DFS(Graph *G, vertex v);
void initDFS(Graph *G, vertex v);
void BFS (Graph *G, vertex v);
void initBFS(Graph *G, vertex v);
void dijkstra (Graph *G, vertex source);






#endif
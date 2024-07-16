    #include <stdio.h>
    #include <stdlib.h>
    #include "dijkstra.h"
    #include <limits.h>
    #include <queue>

    using namespace std;

    #define vertex int 

    static link *fill_arr_zeros(int rows, int columns, int val){
        link *matrix = new link[rows];
        
        for(vertex k = 0; k < rows; k++){
            matrix[k] = new Matrix[columns];
        }
        
        for(vertex v = 0; v < rows; v++){
            
            for(vertex w = 0; w < columns; w++){
                matrix[v][w].w = val; 
            }
        }
        
        return matrix;
    }

    Graph *graph_init(int n){
        Graph *G = new Graph;

        if(G == NULL){
            exit(1);
        }
        
        G->V = n;
        G->A = 0;
        G->adj = fill_arr_zeros(n,n,0);
        return G;
    }

    void insert_arc(Graph *G, vertex v, vertex w, int weight){
        
        if(G->adj[v][w].w == 0){
            G->adj[v][w].w = 1;
            //G->adj[w][v].w = 1;
            G->adj[v][w].weight = weight;
            //G->adj[v][w].visited = false;
            G->A++;
        }
    }

    void remove_arc(Graph *G, vertex v, vertex w, int weight){
    
        if(G->adj[v][w].w == 1){
            G->adj[v][w].w = 0;
            //G->adj[w][v].w = 0;
            G->adj[v][w].weight = 0;
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
        int *queue = new int[G->V];
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

static void print_path(vertex* parent, int n){
    if(parent[n]==-1){
        printf("%d",n);
        return;
    }
    print_path(parent,parent[n]);
    printf(" -> %d",n);

}

static void printDijkstra(int* dist, vertex* parent, int n,int node){
    printf("Node %d:\n",node);
    for(int i=0; i<n;i++){
        printf("Distance from %d to %d = ",node,i);
        
        if(dist[i] != __INT_MAX__ && i<n-1){
            printf("%d\n",dist[i]);
        }else if(dist[i] != __INT_MAX__){
            printf("%d\n",dist[i]);
        }else if(dist[i] == __INT_MAX__ && i<n-1){
            printf("infinity\n");
        }else{
            printf("infinity");
        }
        if(parent[i]<0){
            continue;
        }

        printf("Path: ");
        
        print_path(parent,i);
        printf("\n");

    }
    
    if(node<n-1){
        printf("\n");
    }
    
}


void dijkstra(Graph* G, vertex source) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    vector<int> dist(G->V, INT_MAX);
    vector<vertex> parent(G->V, -1);
    vector<bool> visited(G->V, false);

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < G->V; v++) {
            if (G->adj[u][v].weight == 0) continue;

            int alt = dist[u] + G->adj[u][v].weight;
            if (alt < dist[v]) {
                dist[v] = alt;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    printDijkstra(dist.data(), parent.data(), G->V, source);
}

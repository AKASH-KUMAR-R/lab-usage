#include <stdio.h> 
#define INF 999  // Represent infinity as 999
#define MAX_NODES 20  // Maximum number of nodes
int cost[MAX_NODES][MAX_NODES]; 
int dist[MAX_NODES]; 
int visited[MAX_NODES];  // To track visited nodes
int prev[MAX_NODES];  // To store the previous node for the shortest path
int src; 
void dijkstra(int num_nodes); 
void print_shortest_path(int node);
void initialize(int num_nodes);
int main() { 
    int num_nodes;
    printf("Enter number of nodes: ");
    scanf("%d", &num_nodes);
    printf("Enter the source node: ");
    scanf("%d", &src);
    printf("Enter the cost matrix (Use 999 for infinity):\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            scanf("%d", &cost[i][j]);
        }
    }
    dijkstra(num_nodes);
}
void initialize(int num_nodes) {
    for (int i = 0; i < num_nodes; i++) {
        dist[i] = INF;  // Set initial distances to infinity
        prev[i] = -1;   // No previous node at the start
        visited[i] = 0;  // Mark all nodes as unvisited
    }
    dist[src] = 0;  // Distance from the source to itself is 0
}

void dijkstra(int num_nodes) { 
    int min_node, min_dist;
    initialize(num_nodes);
    // Main loop of Dijkstra's algorithm
    for (int i = 0; i < num_nodes; i++) {
        min_dist = INF;
        min_node = -1;
        // Find the unvisited node with the smallest distance
        for (int j = 0; j < num_nodes; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                min_node = j;
            }
        }
        // Mark the selected node as visited
        visited[min_node] = 1;
        // Update distances to neighboring nodes
        for (int j = 0; j < num_nodes; j++) {
            if (!visited[j] && cost[min_node][j] < INF) {
                int new_dist = dist[min_node] + cost[min_node][j];
                if (new_dist < dist[j]) {
                    dist[j] = new_dist;
                    prev[j] = min_node;  // Update the previous node
                }
            }
        }
    }

    // After the algorithm, print the shortest paths
    for (int i = 0; i < num_nodes; i++) {
        if (i != src) {
            printf("\nShortest path to node %d: ", i);
            print_shortest_path(i);
            printf(" [distance = %d]", dist[i]);
            printf("\n");
        }
    }
}

void print_shortest_path(int node) {
    if (node == src) {
        printf("%d", src);  // Print source node
        return;
    } else if (prev[node] == -1) {
        printf("No path");
        return;
    } else {
        print_shortest_path(prev[node]);  // Recursively print the path
        printf(" -> %d", node);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 30  // Define the maximum number of nodes in the network

// Structure to represent the routing table of a node
struct RoutingTable {
    int distance[MAX_NODES];  // Array to store the distance to each destination node
    int nextHop[MAX_NODES];   // Array to store the next hop node for each destination
} routingTables[MAX_NODES];  // Array of routing tables for all nodes

// Matrix to store the direct distances between nodes
int adjacencyMatrix[MAX_NODES][MAX_NODES];

void main() {
    int n, i, j, k;

    // Ask the user to input the number of nodes
    printf("Enter the number of nodes: ");
    scanf("%d", &n);
    
    // Ask the user to input the adjacency matrix (direct distances between nodes)
    printf("Enter the adjacency matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &adjacencyMatrix[i][j]);  // Read the distance between nodes i and j
            routingTables[i].distance[j] = adjacencyMatrix[i][j];  // Initialize the distance in the routing table for node i to node j
            routingTables[i].nextHop[j] = j;  // Initially, set the next hop for node i to be node j (direct path)
        }
    }

    // Distance Vector Algorithm to update the routing tables
    for (i = 0; i < n; i++) {  
        for (j = 0; j < n; j++) {  // Iterate over each destination
            for (k = 0; k < n; k++) {  // Check if an intermediate node k gives a shorter path
                if (routingTables[i].distance[j] > adjacencyMatrix[i][k] + routingTables[k].distance[j]) {  // If a shorter path is found through node k
                    routingTables[i].distance[j] = adjacencyMatrix[i][k] + routingTables[k].distance[j];  // Update the distance for node i to node j via node k
                    routingTables[i].nextHop[j] = k;  // Update the next hop for node i to node j to node k
                }
            }
        }
    }

    // Print the routing tables for each node
    for (i = 0; i < n; i++) {  // Iterate over each node
        printf("Routing table for node %d:\n", i);
        printf("Destination\tCost\tNext Hop\n");  // Print headers for the routing table
        
        // Loop to print the routing information for each destination node
        for (j = 0; j < n; j++) {
            printf("%d\t\t%d\t\t%d\n", j, routingTables[i].distance[j], routingTables[i].nextHop[j]);  // Print destination, cost, and next hop
        }
        printf("\n");  // Add a newline between routing tables for different nodes
    }
}


    
  /*
Enter the number of nodes: 3
Enter the adjacency matrix:
0
2
5
2
0
3
5
3
0
Routing table for node 0:
Destination	Cost	Next Hop
0		0		0
1		2		1
2		5		2

Routing table for node 1:
Destination	Cost	Next Hop
0		2		0
1		0		1
2		3		2

Routing table for node 2:
Destination	Cost	Next Hop
0		5		0
1		3		1
2		0		2
*/
    

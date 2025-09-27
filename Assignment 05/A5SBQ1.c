#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 20
#define INF 1000000

typedef enum { OR_NODE, AND_NODE, LEAF_NODE } NodeType;

typedef struct {
    int childCount;
    int children[MAX_NODES];
    int costs[MAX_NODES];  // cost to solve each child/subproblem
} NodeChildren;

typedef struct {
    char name[10];
    NodeType type;
    NodeChildren childData;
    int heuristic;  // heuristic cost estimate
    int solved;    // boolean flag if node is solved
    int minCost;   // stores minimum cost found for node
} Node;

Node nodes[MAX_NODES];
int nodeCount = 0;

int findNodeIndex(const char *name) {
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(nodes[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Forward declaration
int aoStar(int nodeIndex);

// Solve AND node: sum of children's costs + their edge costs
int solveAndNode(int nodeIndex) {
    int costSum = 0;
    for (int i = 0; i < nodes[nodeIndex].childData.childCount; i++) {
        int child = nodes[nodeIndex].childData.children[i];
        int childCost = aoStar(child);
        if (childCost == INF) return INF;
        costSum += childCost + nodes[nodeIndex].childData.costs[i];
    }
    return costSum;
}

// Solve OR node: minimum of children's cost + edge costs
int solveOrNode(int nodeIndex) {
    int minCost = INF;
    for (int i = 0; i < nodes[nodeIndex].childData.childCount; i++) {
        int child = nodes[nodeIndex].childData.children[i];
        int childCost = aoStar(child);
        if (childCost == INF) continue;
        int totalCost = childCost + nodes[nodeIndex].childData.costs[i];
        if (totalCost < minCost)
            minCost = totalCost;
    }
    return minCost;
}

int aoStar(int nodeIndex) {
    if (nodes[nodeIndex].solved)
        return nodes[nodeIndex].minCost;

    if (nodes[nodeIndex].type == LEAF_NODE) {
        nodes[nodeIndex].minCost = nodes[nodeIndex].heuristic;
        nodes[nodeIndex].solved = 1;
        return nodes[nodeIndex].minCost;
    }

    int cost;
    if (nodes[nodeIndex].type == AND_NODE)
        cost = solveAndNode(nodeIndex);
    else // OR_NODE
        cost = solveOrNode(nodeIndex);

    if (cost == INF)
        return INF;

    if (cost < nodes[nodeIndex].heuristic)
        nodes[nodeIndex].minCost = cost;
    else
        nodes[nodeIndex].minCost = nodes[nodeIndex].heuristic;

    nodes[nodeIndex].solved = 1;
    return nodes[nodeIndex].minCost;
}

void addNode(const char* name, NodeType type, int heuristic) {
    strcpy(nodes[nodeCount].name, name);
    nodes[nodeCount].type = type;
    nodes[nodeCount].heuristic = heuristic;
    nodes[nodeCount].solved = 0;
    nodes[nodeCount].minCost = INF;
    nodes[nodeCount].childData.childCount = 0;
    nodeCount++;
}

void addChild(const char* parentName, const char* childName, int cost) {
    int p = findNodeIndex(parentName);
    int c = findNodeIndex(childName);
    if (p == -1 || c == -1) {
        printf("Error: Node not found\n");
        return;
    }
    int count = nodes[p].childData.childCount;
    nodes[p].childData.children[count] = c;
    nodes[p].childData.costs[count] = cost;
    nodes[p].childData.childCount++;
}

int main() {
    // Example graph (You must define your own graph here)
    // Add nodes: name, type, heuristic cost
    addNode("A", OR_NODE, 10);
    addNode("B", AND_NODE, 8);
    addNode("C", LEAF_NODE, 5);
    addNode("D", LEAF_NODE, 7);
    addNode("E", LEAF_NODE, 3);

    // Add edges: parent, child, cost to child
    addChild("A", "B", 1);
    addChild("A", "C", 2);
    addChild("B", "D", 2);
    addChild("B", "E", 3);

    int start = findNodeIndex("A");
    int minCost = aoStar(start);

    printf("Minimum cost solution from node %s: %d\n", nodes[start].name, minCost);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10
#define INF 1000000

// Map node characters 'A'..'J' to indices 0..9
int node_to_index(char node) {
    return node - 'A';
}

char index_to_node(int index) {
    return 'A' + index;
}

// Structure for priority queue node
typedef struct {
    int f;      // f = g + h
    int g;      // cost so far
    int node;   // current node index
    int parent; // parent node index (-1 for start)
} PQNode;

typedef struct {
    PQNode *data;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* create_pq(int capacity) {
    PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->data = (PQNode*)malloc(sizeof(PQNode) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(PQNode *a, PQNode *b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(PriorityQueue *pq, PQNode node) {
    if (pq->size == pq->capacity) {
        printf("Priority queue full!\n");
        return;
    }
    pq->data[pq->size] = node;
    int i = pq->size;
    pq->size++;

    // Bubble up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->data[parent].f <= pq->data[i].f)
            break;
        swap(&pq->data[parent], &pq->data[i]);
        i = parent;
    }
}

PQNode pop(PriorityQueue *pq) {
    PQNode top = pq->data[0];
    pq->size--;
    pq->data[0] = pq->data[pq->size];

    // Bubble down
    int i = 0;
    while (1) {
        int left = 2*i + 1;
        int right = 2*i + 2;
        int smallest = i;

        if (left < pq->size && pq->data[left].f < pq->data[smallest].f)
            smallest = left;
        if (right < pq->size && pq->data[right].f < pq->data[smallest].f)
            smallest = right;

        if (smallest == i)
            break;
        swap(&pq->data[i], &pq->data[smallest]);
        i = smallest;
    }
    return top;
}

int is_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

// Graph adjacency list and heuristics

typedef struct {
    int neighbor;
    int cost;
} Edge;

Edge graph[MAX_NODES][MAX_NODES];
int graph_size[MAX_NODES];  // number of edges for each node

int heuristic[MAX_NODES] = {
    14, // A
    12, // B
    11, // C
    6,  // D
    4,  // E
    5,  // F
    3,  // G
    2,  // H
    6,  // I
    0   // J
};

void add_edge(char from, char to, int cost) {
    int u = node_to_index(from);
    int v = node_to_index(to);
    int sz = graph_size[u];
    graph[u][sz].neighbor = v;
    graph[u][sz].cost = cost;
    graph_size[u]++;
}

void print_path(int parents[], int node) {
    if (node == -1)
        return;
    print_path(parents, parents[node]);
    printf("%c ", index_to_node(node));
}

int contains(int *arr, int size, int val) {
    for (int i = 0; i < size; i++)
        if (arr[i] == val)
            return 1;
    return 0;
}

int main() {
    // Initialize graph edges (directed graph)
    memset(graph_size, 0, sizeof(graph_size));

    add_edge('A', 'B', 4);
    add_edge('A', 'C', 3);
    add_edge('B', 'D', 5);
    add_edge('B', 'E', 12);
    add_edge('C', 'F', 7);
    add_edge('D', 'G', 2);
    add_edge('E', 'H', 6);
    add_edge('F', 'I', 4);
    add_edge('G', 'J', 3);
    add_edge('H', 'J', 2);
    add_edge('I', 'J', 6);

    int start = node_to_index('A');
    int goal = node_to_index('J');

    PriorityQueue *pq = create_pq(1000);
    int closed_set[MAX_NODES] = {0};
    int parents[MAX_NODES];
    int g_score[MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        parents[i] = -1;
        g_score[i] = INF;
    }

    // Push start node: f = g + h = 0 + heuristic[start]
    g_score[start] = 0;
    PQNode start_node = {heuristic[start], 0, start, -1};
    push(pq, start_node);

    int found = 0;

    while (!is_empty(pq)) {
        PQNode current = pop(pq);

        if (closed_set[current.node]) continue;
        closed_set[current.node] = 1;

        parents[current.node] = current.parent;

        if (current.node == goal) {
            found = 1;
            printf("Most cost-effective path from %c to %c:\n", index_to_node(start), index_to_node(goal));
            print_path(parents, goal);
            printf("\nTotal cost: %d\n", current.g);
            break;
        }

        int u = current.node;
        for (int i = 0; i < graph_size[u]; i++) {
            int v = graph[u][i].neighbor;
            int cost = graph[u][i].cost;

            if (closed_set[v]) continue;

            int tentative_g = g_score[u] + cost;
            if (tentative_g < g_score[v]) {
                g_score[v] = tentative_g;
                PQNode neighbor_node = {tentative_g + heuristic[v], tentative_g, v, u};
                push(pq, neighbor_node);
            }
        }
    }

    if (!found) {
        printf("No path found from %c to %c\n", index_to_node(start), index_to_node(goal));
    }

    free(pq->data);
    free(pq);

    return 0;
}


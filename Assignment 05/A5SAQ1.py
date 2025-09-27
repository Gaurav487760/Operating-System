from heapq import heappush, heappop

# Graph adjacency list with distances
graph = {
    'A': [('B', 4), ('C', 3)],
    'B': [('D', 5), ('E', 12)],
    'C': [('F', 7)],
    'D': [('G', 2)],
    'E': [('H', 6)],
    'F': [('I', 4)],
    'G': [('J', 3)],
    'H': [('J', 2)],
    'I': [('J', 6)],
    'J': []
}

# Heuristic values
heuristic = {
    'A': 14,
    'B': 12,
    'C': 11,
    'D': 6,
    'E': 4,
    'F': 5,
    'G': 3,
    'H': 2,
    'I': 6,
    'J': 0
}

def a_star_graph(start, goal):
    open_set = []
    heappush(open_set, (heuristic[start], 0, start, [start]))  # (f, g, node, path)
    closed_set = set()

    while open_set:
        f, g, current, path = heappop(open_set)

        if current == goal:
            return path, g

        if current in closed_set:
            continue
        closed_set.add(current)

        for neighbor, dist in graph[current]:
            if neighbor not in closed_set:
                g_new = g + dist
                f_new = g_new + heuristic[neighbor]
                heappush(open_set, (f_new, g_new, neighbor, path + [neighbor]))

    return None, float('inf')

if __name__ == "__main__":
    start_node = 'A'
    goal_node = 'J'
    path, cost = a_star_graph(start_node, goal_node)

    if path:
        print(f"Most cost-effective path from {start_node} to {goal_node}:")
        print(" -> ".join(path))
        print(f"Total cost: {cost}")
    else:
        print("No path found.")


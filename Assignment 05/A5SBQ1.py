def ao_star(node, graph, heuristic, solved):
    if node not in graph:
        # Leaf node
        solved[node] = True
        return heuristic[node]

    if solved.get(node, False):
        return heuristic[node]

    # For OR node: find child with min cost
    # For AND node: sum all children's cost

    # This is a placeholder; actual implementation depends on node type & graph structure.

    # Update heuristic[node] with minimum cost
    # Mark node as solved if solution found

    return heuristic[node]

# Example usage:

graph = {
    'A': [('B', 2), ('C', 3)],  # OR node or AND node depends on your input
    'B': [('D', 4)],
    'C': [],
    'D': []
}

heuristic = {
    'A': 10,
    'B': 6,
    'C': 7,
    'D': 5
}

solved = {}

cost = ao_star('A', graph, heuristic, solved)
print(f"Minimum cost solution: {cost}")


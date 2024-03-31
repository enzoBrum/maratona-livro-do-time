'''
Returns best path from "start" to "end" in a weighted graph.
If such path doesn't exist, returns -1.
Uses the "Graph" class.
"weights" list is 0 indexed (i.e. index 0 is not empty).
'''
class Graph:
    
    def __init__(self):
        self.graph = {}
        
    def add_unidirectional_edge(self, u, v):
        if u in self.graph:
            self.graph[u].add(v)
        else:
            self.graph[u] = {v}

    def add_bidirectional_edge(self, u, v):
        self.add_unidirectional_edge(u, v)
        self.add_unidirectional_edge(v, u)
        
    def get_next_vertices(self, u):
        if u in self.graph:
            return self.graph[u]
        else:
            return set()


def get_closest_node(results, unvisited):
    res = 0
    val = float("inf")
    
    for node in unvisited:
        if results[node] <= val:
            val = results[node]
            res = node
    return res


def dijkstra(graph, start, end, vertice_count, weights):
    unvisited = set()
    for i in range(1, vertice_count+1):
        unvisited.add(i)
    results = [float("inf") for _ in range(vertice_count + 1)]
    results[start] = 0

    while unvisited:
        node = get_closest_node(results, unvisited)
        unvisited.remove(node)
        if node == end:
            return results[node] if results[node] != float("inf") else -1
            break
        for next in graph.get_next_vertices(node):
            if next in unvisited:
                d = results[node] + weights[node][next]
                if d < results[next]:
                    results[next] = d

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

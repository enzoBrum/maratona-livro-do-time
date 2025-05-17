/*
  MST (Kruskal algorithm)
  Utilizes UnionFind and Edge structures. Edge only accepts integer edge weights.
  For maximum spanning tree, just turn the edge weights negative.
  Also works for minimum edge product.
*/


struct UnionFind {
    vector<int> parent, size;
    
    UnionFind(int n) {
        parent.reserve(n);
        size.assign(n, 1);
        
        for (int i = 0; i < n; i++)
            parent.push_back(i);
    }
    
    int find(int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find(parent[v]);
    }
    
    void unionSets(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (size[a] < size[b])
                swap(a, b);
            parent[b] = a; // subordinate b to a (smaller to bigger)
            size[a] += size[b];
        }
    }
};

struct Edge {
    int u, v, weight;
    
    Edge(int _u, int _v, int _weight) : u(_u), v(_v), weight(_weight) {}
    
    bool operator<(Edge &other) const {
        return weight < other.weight;
    }
};

struct Kruskal {
    int n;
    vector<Edge> edges;

    Kruskal(int n, vector<Edge> &edges) : n(n), edges(edges) {}

    vector<Edge> mst() {
        sort(edges.begin(), edges.end());
        UnionFind uf = UnionFind(n);
        vector<Edge> mst;
        mst.reserve(n-1);

        for (Edge e : edges) {
            if (uf.find(e.u) != uf.find(e.v)) {
                mst.push_back(e);
                uf.unionSets(e.u, e.v);
            }
            if (mst.size() == n-1)
                break;
        }
        return mst;
    }
};

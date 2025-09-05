// Also works for max spanning tree and min edge product

struct UnionFind {
    vector<int> parent, size;    
    UnionFind(int n) : parent(n), size(n, 1) {
        iota(all(parent), 0ll);
    }
    
    int find(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find(parent[v]);
    }
    
    void unionSets(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a; // subordinate b to a (smaller to bigger)
        size[a] += size[b];
    }
};

struct Edge {
    int u, v, w;
    bool operator<(Edge &o) const {
        return w < o.w;
    }
};

vector<Edge> kruskal(int n, vector<Edge> &edges) {
    sort(all(edges));
    UnionFind uf(n);
    vector<Edge> mst;

    for (Edge e : edges) {
        if (uf.find(e.u) != uf.find(e.v)) {
            mst.push_back(e);
            uf.unionSets(e.u, e.v);
        }
    }
    return mst;
}

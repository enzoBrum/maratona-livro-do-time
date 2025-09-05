// int n - amount of sets (0 indexed)
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

// Allows you to undo set unions - O(logn) union, O(1) per rollback.
// Store the current state (i.e., history.size(), and, later on, do rollback() while history.size() != state).
struct RollbackUnionFind {
    vector<int> parent, size;
    int components;
    stack<pair<int*, int>> history;
    
    RollbackUnionFind(int n=0) {
        parent.resize(n);
        size.assign(n, 1);
        iota(all(parent), 0);
        components = n;
    }
    
    int find(int v) {
        if (v == parent[v])
            return v;
        return find(parent[v]);
    }

    void change(int &x, int newVal) {
        history.push({&x, x}); // x's current state
        x = newVal;
    }

    void rollback() {
        auto[ptr, val] = history.top(); history.pop();
        *ptr = val;
    }
    
    void unionSets(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        change(parent[b], a);
        change(size[a], size[a]+size[b]);
        change(components, components-1);
    }
};

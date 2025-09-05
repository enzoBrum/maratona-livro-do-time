struct RollbackUnionFind {
    vector<int> parent, size, hasWasps, hasTraps, arch, ans;
    int components, k;
    stack<pair<int*, int>> history;
    stack<ii> unionsToPropagate;
    
    RollbackUnionFind() {}
    RollbackUnionFind(int n, int e, int a, int k, vector<int> &traps) : k(k) {
        parent.resize(n);
        iota(all(parent), 0);
        size.assign(n, 1);
        hasWasps.assign(n, 0);
        components = n;
        ans.assign(a, -1);

        arch.assign(n, 0);
        for (int i = 0; i < a; i++)
            arch[i] ^= 1ll<<i;

        hasTraps.assign(n, 0);
        for (int i : traps) {
            hasTraps[i] = 1;
            if (k <= 1)
                hasWasps[i] = 1;
        }
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
        
        if (hasTraps[b])
            change(hasTraps[a], 1);      
        if (hasTraps[a] && size[a] >= k)
            hasWasps[a] = 1; // notice this one doesn't go to rollbacks
        if (hasWasps[b])
            hasWasps[a] = 1;

        // also doesn't rollback
        arch[a] |= arch[b];
        unionsToPropagate.push({b, a});
    }
};

struct Edge {
    int x, y;
};

struct SegmentTree {
    struct Segment {
        int l, r;
        vector<Edge> edg;
    };

    vector<Segment> t;
    RollbackUnionFind uf;
    int e, n, a;

    void build(int v, int tl, int tr) {
        t[v] = {tl, tr, {}};
        if (tl != tr) {
            int mid = (tl + tr) / 2;
            build(2*v, tl, mid);
            build(2*v+1, mid+1, tr);
        }
    }

    void addEdge(int v, int l, int r, Edge const &e) {
        if (t[v].l > r || t[v].r < l)
            return;
        if (t[v].l >= l && t[v].r <= r) {
            t[v].edg.push_back(e);
            return;
        }
        addEdge(2*v, l, r, e);
        addEdge(2*v+1, l, r, e);
    }

    void dfs(int v) {
        int state = uf.history.size();
        int unionsState = uf.unionsToPropagate.size();

        for (Edge &e : t[v].edg)
            uf.unionSets(e.x, e.y);
        
        if (t[v].l == t[v].r) {
            // at leaf. Compute answer.
            for (int i = n-e; i < n; i++) {
                int parent = uf.find(i);
                if (!uf.hasWasps[parent]) {
                    int mask = uf.arch[parent];
                    for (int j = 0; j < a; j++)
                        if (mask & (1ll<<j))
                            if (uf.ans[j] == -1)
                                uf.ans[j] = t[v].l;
                }
            }
        }
        else {
            dfs(2*v); dfs(2*v+1);
        }

        // now manually update archeologist and wasps
        while (uf.unionsToPropagate.size() != unionsState) {
            auto[b, a] = uf.unionsToPropagate.top(); uf.unionsToPropagate.pop();
            uf.arch[b] = uf.arch[a];
            uf.hasWasps[b] = uf.hasWasps[a];
        }
        while (uf.history.size() != state) uf.rollback();
    }

    SegmentTree(int sz, int n, int e, int a, int k, vector<int> &traps) : e(e), n(n), a(a) {
        t.resize(4*sz);
        build(1, 0, sz-1);
        uf = RollbackUnionFind(n, e, a, k, traps);
    }

    void addEdge(int l, int r, Edge e) {
        addEdge(1, l, r, e);
    }

    void dfs() {
        dfs(1);
    }
};

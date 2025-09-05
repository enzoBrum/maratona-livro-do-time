struct LCA {
    int n, l;
    int timer = 0;
    vector<int> tin, tout, d;
    vector<vector<int>> up, upmaxi;

    LCA (vector<vector<ii>> &adj, int root) : n(adj.size()), l(ceil(log2(n))), tin(n), tout(n), d(n, -1) {
        up.assign(n, vector<int>(l + 1));
        upmaxi.assign(n, vector<int>(l + 1));
        dfs(root, root, 0, adj);
    }

    void dfs(int v, int p, int lastEdge, vector<vector<ii>> &adj) {
        tin[v] = ++timer;
        up[v][0] = p;
        upmaxi[v][0] = lastEdge;
        d[v] = d[p]+1; // for kthAncestor

        for (int i = 1; i <= l; ++i) {
            up[v][i] = up[up[v][i-1]][i-1];
            upmaxi[v][i] = max(upmaxi[v][i-1], upmaxi[up[v][i-1]][i-1]);
        }

        for (auto[w, u] : adj[v]) 
            if (u != p)
                dfs(u, v, w, adj);
        tout[v] = ++timer;
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }

    int lca(int u, int v) {
        if (isAncestor(u, v)) return u;
        if (isAncestor(v, u)) return v;
        for (int i = l; i >= 0; --i)
            if (!isAncestor(up[u][i], v))
                u = up[u][i];
        return up[u][0];
    }

    int maxEdgeToAncestor(int u, int anc) {
        if (u == anc) return 0;
        int maxi = 0;
        for (int i = l; i >= 0; --i)
            if (!isAncestor(up[u][i], anc)) {
                maxi = max(maxi, upmaxi[u][i]);
                u = up[u][i];
            }
        return max(maxi, upmaxi[u][0]);
    }

    int maxEdge(int u, int v) {
        if (isAncestor(u, v)) return maxEdgeToAncestor(v, u);
        if (isAncestor(v, u)) return maxEdgeToAncestor(u, v);
        int anc = lca(u, v);
        return max(maxEdgeToAncestor(u, anc), maxEdgeToAncestor(v, anc));
    }

    int kthAncestor(int u, int k) {
        if (k > d[u]) return -1;
        for (int i = l; i >= 0; --i)
            if (k >= (1ll<<i))
                u = up[u][i], k -= (1ll<<i);
        return u;
    }
};

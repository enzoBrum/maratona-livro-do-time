struct LCA {
    int n, l;
    vector<vector<int>> tree;
    int timer;
    vector<int> tin, tout;
    vector<vector<int>> up, upMaxEdge;
    map<ii, int> weights;

    LCA (vector<vector<int>> &_tree, map<ii, int> &_weights, int root) : tree(_tree), weights(_weights) {
        n = tree.size();
        tin.resize(n);
        tout.resize(n);
        timer = 0;
        l = ceil(log2(n));
        up.assign(n, vector<int>(l + 1));
        upMaxEdge.assign(n, vector<int>(l + 1));
        weights[{root, root}] = 0;
        dfs(root, root);
    }

    void dfs(int v, int p) {
        tin[v] = ++timer;
        up[v][0] = p;
        int weightVP = 0;
        if (weights.count({v, p}))
            weightVP = weights[{v, p}];
        else
            weightVP = weights[{p, v}];
        upMaxEdge[v][0] = weightVP;
        
        for (int i = 1; i <= l; ++i) {
            up[v][i] = up[up[v][i-1]][i-1]; // 2^i up from v is the same as 2^(i-1) up from (2^(i-1) up from v [its parent]))
            upMaxEdge[v][i] = max(upMaxEdge[v][i-1], upMaxEdge[up[v][i-1]][i-1]); // max from first half and second half
        }

        for (int u : tree[v]) 
            if (u != p)
                dfs(u, v);
        tout[v] = ++timer;
    }

    bool is_ancestor(int u, int v) {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }

    int lca(int u, int v) {
        if (is_ancestor(u, v))
            return u;
        if (is_ancestor(v, u))
            return v;
        for (int i = l; i >= 0; --i)
            if (!is_ancestor(up[u][i], v))
                u = up[u][i];
        return up[u][0];
    }

    int maxEdgeFromUToLca(int u, int lca) {
        if (u == lca) // if u is a direct parent of v, then the max edge is 0
            return 0;

        // paths are built like follows: if the dude being checked (up[u][i]) is
        // not ancestor of the lca, then u is lifted up. That is the moment where
        // you apply the max(...).
        // After the loop ends, u is not the lca, but up[u][0] is (its parent).
        // That's why you still have to do one more max at the end.
        int maxEdge = 0;
        for (int i = l; i >= 0; --i) {
            if (!is_ancestor(up[u][i], lca)) {
                maxEdge = max(maxEdge, upMaxEdge[u][i]);
                u = up[u][i];
            }
        }
        maxEdge = max(maxEdge, upMaxEdge[u][0]);
        return maxEdge;
    }

    int lcaMaxEdge(int u, int v) {
        int ancestor = lca(u, v);
        return max(maxEdgeFromUToLca(u, ancestor), maxEdgeFromUToLca(v, ancestor));
    }
};

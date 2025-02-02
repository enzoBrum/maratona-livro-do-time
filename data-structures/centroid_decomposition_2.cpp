class CentroidDecomposition {
    vector<vector<int>> adj;
    vector<int> subtreeSz, removed, cnt;
    int maxDepth;

    // Sets sizes of subtrees of the tree rooted at 'u' bounded by removed vertices.
    void setSubtreeSizes(int u, int p = -1) {
        subtreeSz[u] = 1;
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            setSubtreeSizes(v, u);
            subtreeSz[u] += subtreeSz[v];
        }
    }

    // Returns the centroid of the tree rooted at 'root' bounded by removed vertices.
    int getCentroid(int u, int p, int root) {
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            if (subtreeSz[v] * 2 > subtreeSz[root])
                return getCentroid(v, u, root);
        }
        return u;
    }

    // Recursively counts paths of length exactly k (and sets up cnt for other subtrees of the centroid).
    void count(int u, int p, bool querying, int depth=1) {
        if (depth > k) return;
        if (querying) totalPathsK += cnt[k-depth];
        else cnt[depth]++;
        maxDepth = max(maxDepth, depth);
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            count(v, u, querying, depth+1);
        }
    }

    // This is where you iterate over the subtree with centroid 'centroid', and this is the place
    // where you edit your code and make it do whatever you need it to in O(sz) time.
    void build(int u) {
        setSubtreeSizes(u);
        int centroid = getCentroid(u, u, u);
        removed[centroid] = 1;

        maxDepth = 0;
        for (int v : adj[centroid]) {
            if (removed[v]) continue;
            count(v, centroid, true);
            count(v, centroid, false);
        }
        fill(cnt.begin()+1, cnt.begin()+maxDepth+1, 0);

        for (int v : adj[centroid])
            if (!removed[v])
                build(v);
    }

public:
    int k;
    ll totalPathsK;
    CentroidDecomposition(vector<vector<int>> &adj, int k, int root=0) : adj(adj), k(k) {
        int n = adj.size();
        totalPathsK = 0;
        subtreeSz.resize(n);
        cnt.assign(n, 0);
        cnt[0] = 1;
        removed.assign(n, 0);
        build(root);
    }
};

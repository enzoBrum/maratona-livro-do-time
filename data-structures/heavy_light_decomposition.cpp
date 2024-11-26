// Bring your favorite LazySegmentTree implementation here.
// The one used here should allow 'setting' and 'unsetting' a range.

class HLD {
    vector<int> sz, h, pos, p, at;
    LazySegmentTree st;
    int timer = 0;

    void dfsSz(int v, int parent, vector<vector<int>> &adj) {
        sz[v] = 1;
        p[v] = parent;
        for (int &u : adj[v]) { // pass by &reference is important!!
            if (u == parent) continue;
            dfsSz(u, v, adj);
            sz[v] += sz[u];
            // The first neighbour in the adj will be the heavy path
            if (sz[u] > sz[adj[v][0]] || adj[v][0] == parent)
                swap(u, adj[v][0]);
        }
    }

    void dfsHLD(int v, int parent, vector<vector<int>> &adj) {
        pos[v] = timer;
        at[timer++] = v;
        for (int u : adj[v]) {
            if (u == parent) continue;
            // if u is the heavy path
            if (u == adj[v][0]) h[u] = h[v]; // continue heavy path from above
            else h[u] = u; // starts its own heavy path
            dfsHLD(u, v, adj);
        }
    }
    
public:
    HLD(vector<vector<int>> &adj) {
        int n = adj.size();
        sz.resize(n);
        h.resize(n);
        pos.resize(n);
        p.resize(n);
        at.resize(n);
        dfsSz(0, -1, adj);
        dfsHLD(0, -1, adj);
        p[0] = -1; // DANGEROUS BUT NEEDED
        vector<int> emptyVector(n, 0);
        st = LazySegmentTree(emptyVector);
    }

    // Now, the subtree of v is on the range [pos[v], pos[v]+sz[v]-1] on the segment tree.
    // The path from v to the head of its heavy chain (h[v]) is [pos[h[v]], pos[v]] (because the
    // heavy path was the first to be visited on the dfs). This gives extreme querying power
    // when building a segment tree from it.

    int querySubtree(int u) {
        return st.query(pos[u], pos[u] + sz[u] - 1);
    }

    void updateSubtree(int u, int delta) {
        st.update(pos[u], pos[u] + sz[u] - 1, delta);
    }

    void updatePath(int a, int b, int delta) {
        if (pos[a] < pos[b]) swap(a, b);
		if (h[a] == h[b]) {
            st.update(pos[b], pos[a], delta);
            return;
        }
		st.update(pos[h[a]], pos[a], delta);
        updatePath(p[h[a]], b, delta);
    }

    void turnOnPathAbove(int u) {
        if (u == -1) return;
        // if the current heavy path is full, keep going. Else, binary search.
        int shouldBeOff = pos[u] - pos[h[u]] + 1;
        if (querySubtree(h[u]) + shouldBeOff >= sz[h[u]]) {
            st.update(pos[h[u]], pos[u], 1);
            turnOnPathAbove(p[h[u]]);
            return;
        }

        int low = pos[h[u]];
        int high = pos[u] + 1;
 
        while (low < high) {
            int mid = (low + high) / 2;
            int setOn = st.query(mid, mid+sz[at[mid]]-1);
            int shouldBeOff = pos[u] - mid + 1;
            if (setOn + shouldBeOff == sz[at[mid]])
                high = mid;
            else low = mid+1;
        }
 
        st.update(low, pos[u], 1);
    }

    void turnOnOperation(int u) {
        if (st.query(pos[u], pos[u])) return;
        updateSubtree(u, 1);
        turnOnPathAbove(p[u]);
    }

    void turnOffOperation(int u) {
        updateSubtree(u, -1);
        updatePath(0, u, -1); // DANGEROUS: updating u twice (I think)
    }
};

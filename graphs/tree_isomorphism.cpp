// Encodes a ROOTED tree as a single integer - O(nlogn)
// For rooted tree isomorphism, compare these encodings
int treeEncoding(vector<vector<int>> &adj, int u, int p=-1) {
    static map<vector<int>, int> enc;
    static int curId = 0;
    vector<int> children;
    for (int next : adj[u])
        if (next != p)
            children.push_back(treeEncoding(adj, next, u));
    sort(all(children));
    if (!enc.count(children))
        return enc[children] = curId++;
    return enc[children];
}

// From here downwards, unrooted isomorphism, also O(nlogn)
vector<int> subtreeSz, centroids;

int setSubtreeSizes(vector<vector<int>> &adj, int u=0, int p=-1) {
    subtreeSz[u] = 1;
    for (int next : adj[u])
        if (next != p)
            subtreeSz[u] += setSubtreeSizes(adj, next, u);
    return subtreeSz[u];
}

void getCentroids(vector<vector<int>> &adj, int u=0, int p=-1) {
    for (int next : adj[u]) {
        if (next != p) {
            if (2*subtreeSz[next] > adj.size())
                return getCentroids(adj, next, u);
            if (2*subtreeSz[next] == adj.size() && adj.size()%2==0) {
                centroids.push_back(u);
                centroids.push_back(next);
                return;
            }
        }
    }
    centroids.push_back(u);
}

bool treeIsomorphic(vector<vector<int>> &adj1, vector<vector<int>> &adj2) {
    subtreeSz.resize(adj2.size());
    setSubtreeSizes(adj2);
    centroids.clear();
    getCentroids(adj2);
    int e = treeEncoding(adj2, centroids[0]);

    subtreeSz.resize(adj1.size());
    setSubtreeSizes(adj1);
    centroids.clear();
    getCentroids(adj1);

    for (int c : centroids)
        if (treeEncoding(adj1, c) == e)
            return true;
    return false;
}

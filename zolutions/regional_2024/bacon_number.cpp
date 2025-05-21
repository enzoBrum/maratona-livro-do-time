#include <bits/stdc++.h>

using namespace std;

//#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;

/*
Exercise:
Given a bipartite graph of 1e2 movies and 1e6 actors, print, for each of the 1e4 queries, a path of
ANY length that connects two given actors.

Idea:
Because the queries only ask for a path and not the shortest path, you can compact the graph into
any spanning (i.e., dfs) tree. Now, the big deal is, to find the path, you won't do a regular dfs
(because you wouldn't know which way to go, and would visit the whole 1e6 sized tree). Instead,
you root it and then compute LCA of that tree. Then, you will always know where to go: simply go
from cur to up[cur][0] until you reach the common ancestor.
Now, about the bounds of the answer. Since the graph is a bipartite graph (of actors and movies),
we know that, in at most 2*n steps, we visited all of the movies. Therefore, if two actors belong
to a same "movie tree", then, in at most 2*n steps, you will reach their lowest common ancestor.
That bounds the answer to at most 2*(2*n) = 4*n vertices per query.
*/

vector<vector<int>> tree;
void dfs(int u, vector<int> &vis, vector<vector<int>> &adj, int component) {
    vis[u] = component;
    for (int next : adj[u])
        if (!vis[next]) {
            tree[u].push_back(next);
            tree[next].push_back(u);
            dfs(next, vis, adj, component);
        }
}

struct LCA {
    int n, l;
    vector<vector<int>> &tree;
    int timer;
    vector<int> tin, tout;
    vector<vector<int>> up;

    LCA(vector<vector<int>> &tree, int root) : tree(tree) {
        n = tree.size();
        tin.assign(n, -1);
        tout.resize(n);
        l = ceil(log2(n));
        up.assign(n, vector<int>(l + 1));
        for (int i = 0; i < n; i++)
            if (tin[i] == -1) {
                timer = 0;
                dfs(i, i);
            }
    }

    void dfs(int v, int p) {
        tin[v] = ++timer;
        up[v][0] = p;
        
        for (int i = 1; i <= l; ++i)
            up[v][i] = up[up[v][i-1]][i-1];

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
};

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n+m);
    tree.assign(n+m, {});
    for (int i = 0; i < n; i++) {
        int actors;
        cin >> actors;
        while (actors--) {
            int a;
            cin >> a; a--;
            adj[i+m].push_back(a);
            adj[a].push_back(i+m);
        }
    }

    vector<int> visited(n+m, 0);
    int component = 1;
    for (int i = 0; i < n+m; i++)
        if (!visited[i])
            dfs(i, visited, adj, component++);
   
    LCA lca(tree, 0);

    int q;
    cin >> q;

    while (q--) {
        int x, y;
        cin >> x >> y;
        x--; y--;

        if (visited[x] != visited[y]) {
            cout << "-1\n";
            continue;
        }

        int ancestor = lca.lca(x, y);
        vector<int> fst, scd;
        int cur = x;
        while (cur != ancestor) {
            int i = cur;
            if (i >= m)
                i -= m; 
            fst.push_back(i);
            cur = lca.up[cur][0];
        }
        if (ancestor >= m) fst.push_back(ancestor-m);
        else fst.push_back(ancestor);
        cur = y;
        while (cur != ancestor) {
            int i = cur;
            if (i >= m)
                i -= m;
            scd.push_back(i);
            cur = lca.up[cur][0];
        }
        reverse(all(scd));

        cout << (fst.size() + scd.size())/2 + 1 << '\n';
        for (int i : fst)
            cout << i+1 << " ";
        for (int i : scd)
            cout << i+1 << " ";
        cout << '\n';
    }
    
    return 0;
}

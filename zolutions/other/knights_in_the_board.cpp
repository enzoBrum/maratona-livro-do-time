#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;

/*
Exercise:
You are given a n*n = 25*25 chess board with some knights on it. Print the smallest amount of knights
you have to remove so no knight attacks another.

Idea:
If you build the collision graph, you will end up with a bipartite graph. Now, what you want is to
remove the smallest amount of vertices such that the graph ends up with no edges. That definition
is the same as the min vertex cover problem (smallest set of vertices such that every edge has at 
least one endpoint on that set). By some theorem, the size of that set is exactly the maximum flow.
So simply build the graph and throw maxflow.
*/

vector<ii> cor = {
    {-1, -2},
    {1, -2},
    {2, -1},
    {2, 1},
    {1, 2},
    {-1, 2},
    {-2, 1},
    {-2, -1}
};

class FordFulkerson {
    bool __dfs(int u, int t) {
        visited[u] = 1;
        if (u == t) return true;
        for (int next : adj[u]) {
            if (!visited[next] && cap[u][next]) {
                if (__dfs(next, t)) {
                    p[next] = u;
                    return true;
                }
            }
        }
        return false;
    }

    bool dfs(int u, int t) {
        visited.assign(n, 0);
        p.assign(n, -1);
        return __dfs(u, t);
    }

public:
    vector<vector<int>> adj, cap;
    vector<int> p, visited;
    int n;

    FordFulkerson(vector<vector<int>> &adj, vector<vector<int>> &cap) : adj(adj), cap(cap) {
        n = adj.size();
    }

    int maxflow(int s, int t) {
        int maxflow = 0;
        while (dfs(s, t)) {
            int increment = INF;
            for (int u = t; p[u] >= 0; u = p[u])
                increment = min(increment, cap[p[u]][u]);

            for (int u = t; p[u] >= 0; u = p[u]) {
                cap[p[u]][u] -= increment;
                cap[u][p[u]] += increment;
            }
            maxflow += increment;
        }
        return maxflow;
    }
};

const int MAXN = 25;
int id[MAXN][MAXN];
ii coord[MAXN*MAXN];

signed main() {
    cin.tie(0)->sync_with_stdio(0);
    memset(id, -1, sizeof(id));

    int n, k;
    cin >> n >> k;

    const int SOURCE = n*n;
    const int SINK = n*n+1;
    const int V = n*n+2;
    vector<vector<int>> adj(V), cap(V, vector<int>(V));

    for (int i = 0; i < k; i++) {
        int r, c;
        cin >> r >> c;
        r--; c--;
        id[r][c] = i;
        coord[i] = {r, c};
    }

    for (int i = 0; i < k; i++) {
        auto[r, c] = coord[i];

        if ((r+c)&1) { // right side
            adj[i].push_back(SINK);
            adj[SINK].push_back(i);
            cap[i][SINK] = 1;
            continue;
        }

        // left side
        adj[i].push_back(SOURCE);
        adj[SOURCE].push_back(i);
        cap[SOURCE][i] = 1;

        for (auto[x, y] : cor) {
            int xx = r+x;
            int yy = c+y;
            if (xx >= 0 && xx < n && yy >= 0 && yy < n && id[xx][yy] != -1) {
                adj[i].push_back(id[xx][yy]);
                adj[id[xx][yy]].push_back(i);
                cap[i][id[xx][yy]] = 1;
            }
        }
    }

    FordFulkerson ff(adj, cap);
    cout << ff.maxflow(SOURCE, SINK) << '\n';
    
    return 0;
}

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

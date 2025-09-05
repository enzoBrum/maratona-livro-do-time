struct FordFulkerson {
    const long long FLOW_INF = 1e18;
    vector<vector<int>> adj, cap;
    vector<int> visited;
    int n, s, t;

    FordFulkerson(int n, int s, int t) : n(n), s(s), t(t) {
        visited.resize(n);
        adj.resize(n);
        cap.assign(n, vector<int>(n, 0));
    }

    void addEdge(int u, int v, int c) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        cap[u][v] += c;
    }

    int dfs(int u, int pushed) {
        visited[u] = 1;
        if (pushed == 0) return 0;
        if (u == t) return pushed;
        for (int next : adj[u]) {
            if (!visited[next] && cap[u][next]) {
                int tr = dfs(next, min(pushed, cap[u][next]));
                if (tr == 0) continue;
                cap[u][next] -= tr;
                cap[next][u] += tr;
                return tr;
            }
        }
        return false;
    }

    int maxflow() {
        int maxflow = 0;
        while (true) {
            fill(all(visited), 0ll);
            int f = dfs(s, FLOW_INF);
            if (!f) break;
            maxflow += f;
        }
        return maxflow;
    }
};

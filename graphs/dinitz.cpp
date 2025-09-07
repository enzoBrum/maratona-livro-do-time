/*
O(V^2 * E)
Several things make it run in O(sqrt(V) * E) (or similar), like unit networks or graphs
where either the indeg or outdeg of every vertex is 1.
*/

struct FlowEdge {
    int u, v;
    long long cap, flow = 0;
    FlowEdge(int u, int v, long long cap) : u(u), v(v), cap(cap) {}
};
 
struct Dinitz {
    const long long FLOW_INF = 1e18;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, s, t, m = 0;
    vector<int> level, ptr;
    queue<int> q;
 
    Dinitz(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }
 
    void addEdge(int u, int v, long long cap) {
        edges.emplace_back(u, v, cap);
        edges.emplace_back(v, u, 0);
        adj[u].push_back(m++);
        adj[v].push_back(m++);
    }
 
    bool bfs() {
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : adj[u]) {
                // if (edges[id].w > limit) continue;
                if (edges[id].cap == edges[id].flow) continue;
                if (level[edges[id].v] != -1) continue;
                level[edges[id].v] = level[u] + 1;
                q.push(edges[id].v);
            }
        }
        return level[t] != -1;
    }
 
    long long dfs(int u, long long pushed) {
        if (pushed == 0) return 0;
        if (u == t) return pushed;
        for (int& cid = ptr[u]; cid < adj[u].size(); cid++) {
            int id = adj[u][cid];
            // if (edges[id].w > limit) continue;
            int v = edges[id].v;
            if (level[u]+1 != level[v]) continue;
            long long tr = dfs(v, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0) continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }
 
    long long maxflow() {
        long long f = 0;
        while (true) {
            fill(all(level), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs()) break;
            fill(all(ptr), 0);
            while (long long pushed = dfs(s, FLOW_INF))
                f += pushed;
        }
        // for (auto& e : edges) e.flow = 0;
        return f;
    }
};

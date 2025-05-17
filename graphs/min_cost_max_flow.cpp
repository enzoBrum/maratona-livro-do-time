/*
Time complexity: O(FVE),
although it is expectable to be MUCH better than that.
O(F) is the very rare Ford-Fulkerson worst case.
O(VE) is the also very rare SPFA worst case.
*/
struct MinCostMaxFlow {
    bool spfa(int s, int t) {
        int n = adj.size();
        d.assign(n, INF);
        vector<bool> inqueue(n, false);
        queue<int> q;
        p.assign(n, -1);

        d[s] = 0;
        q.push(s);
        inqueue[s] = true;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            inqueue[v] = false;

            for (int to : adj[v]) {
                int len = cost[v][to];
                if (d[v] != INF && d[v] + len < d[to] && cap[v][to] > 0) {
                    d[to] = d[v] + len;
                    p[to] = v;
                    if (!inqueue[to]) {
                        q.push(to);
                        inqueue[to] = true;
                    }
                }
            }
        }

        return d[t] != INF;
    }

    vector<vector<int>> adj, cap, cost;
    vector<int> p, d;
    int n;

    MinCostMaxFlow(vector<vector<int>> &adj, vector<vector<int>> &cap, vector<vector<int>> &cost) : adj(adj), cap(cap), cost(cost) {
        n = adj.size();
    }

    // {flow, cost}
    ii maxflow(int s, int t) {
        int maxflow = 0;
        int cost = 0;

        while (spfa(s, t)) {
            int increment = INF;
            for (int u = t; p[u] >= 0; u = p[u])
                increment = min(increment, cap[p[u]][u]);

            for (int u = t; p[u] >= 0; u = p[u]) {
                cap[p[u]][u] -= increment;
                cap[u][p[u]] += increment;              
            }
            cost += increment * d[t];
            maxflow += increment;
        }
        return {maxflow, cost};
    }
};

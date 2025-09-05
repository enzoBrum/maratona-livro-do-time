/*
You have a directed graph where an edge u->v indicates that v is a requirement
of u (or that u needs v).
Each vertex has a profit a[i]. You want a subset of maximum profit such that no
condition of "u needs v" is violated (that is, if u is in the subset, then, v is
in the subset too).
Solution models a mincut of E edges and flow F up to sum(all(a)).
Time complexity: O(E * sum(all(a))
*/

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

    // returns the SOURCE side of the mincut
    vector<int> mincut(int s, int t) {
        while (dfs(s, t)) {
            int increment = INF;
            for (int u = t; p[u] >= 0; u = p[u])
                increment = min(increment, cap[p[u]][u]);

            for (int u = t; p[u] >= 0; u = p[u]) {
                cap[p[u]][u] -= increment;
                cap[u][p[u]] += increment;
            }
        }
        vector<int> cutleft;
        for (int i = 0; i < n; i++)
            if (visited[i] && i != s)
                cutleft.push_back(i);
        return cutleft;
    }
};

// a is the profit of each vertex
// adj has edges u->v where u NEEDS v
// returns vertices that belong to the optimal closure
#warning INF must be larger than the sum of the profits
vector<int> maxClosure(vector<int> &a, vector<vector<int>> &adj) {
    vector<int> ret;
    int n = adj.size();
    const int SOURCE = n;
    const int SINK = n+1;

    vector<vector<int>> flowadj(n+2), cap(n+2, vector<int>(n+2, 0));

    auto addEdge = [&](int u, int v, int w) {
        flowadj[u].push_back(v);
        flowadj[v].push_back(u);
        cap[u][v] = w;
    };

    for (int i = 0; i < n; i++) {
        if (a[i] >= 0) addEdge(SOURCE, i, a[i]);
        else addEdge(i, SINK, -a[i]);
        for (int next : adj[i])
            addEdge(i, next, INF);
    }

    FordFulkerson ff(flowadj, cap);
    return ff.mincut(SOURCE, SINK);
}

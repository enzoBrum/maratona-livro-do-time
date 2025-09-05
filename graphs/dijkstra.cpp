// {length, vertex}
vector<int> dijkstra(int s, vector<vector<ii>> &adj) {
    int n = adj.size();
    const int INF = 1e18;
    vector<int> d(n+1, INF);
    d[s] = 0;
    
    priority_queue<ii, vector<ii>, greater<ii>> pq;
    pq.push({0, s});
    
    while (!pq.empty()) {
        auto [dist, v] = pq.top(); pq.pop();
        if (dist != d[v]) continue;
        for (auto [len, next] : adj[v]) {
            int newDist = dist + len;
            if (newDist < d[next]) {
                d[next] = newDist;
                pq.push({newDist, next});
            }
        }
    }
    return d;
}

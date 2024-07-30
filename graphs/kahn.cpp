/*
  Kahn's algorithm
  O(V+E), aka linear
  vector<int> topological_order will have the topological order after calling kahn()
  if, after kahn's queue, any indegree is not 0, there is a loop (so kahn returns false)
  if there is no loop, kahn returns true
*/

vector<vector<int>> graph;
vector<int> topological_order;

bool kahn() {
    int n = graph.size();
    vector<int> indegree(n, 0);
    topological_order.clear();
    topological_order.reserve(n);

    for (int i = 0; i < n; i++)
        for (int next : graph[i])
            indegree[next]++;
    
    queue<int> q;
    for (int i = 0; i < n; i++)
        if (!indegree[i])
            q.push(i);
    
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topological_order.push_back(u);

        for (int next : graph[u]) {
            indegree[next]--;
            if (!indegree[next])
                q.push(next);
        }
    }

    // if any node has indeg > 0, then there is a cycle
    for (int i = 0; i < n; i++)
        if (indegree[i])
            return false; // cyclic
    return true; // acyclic
}

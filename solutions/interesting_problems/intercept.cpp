/**

Find cutpoints/bridges in a DAG created by dijkstra's algorithm.

As the DAG always points from S to T, we can just treated it as a undirected graph and use the standard algorithm.

*/


#include <bits/stdc++.h>

#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

#define int long long

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;

void dfs(int u, vector<vector<int>>& dag, vector<char>& visited) {
  visited[u] = true;
  for (auto& v : dag[u]) {
    if (visited[v])
      continue;
    dfs(v, dag, visited);
  }
}


void find_cutpoints(int u, int p, vector<char>& tvisited, vector<vector<int>>& adj, vector<int>& tin, vector<int>& low, vector<char>& visited, vector<bool>& cutpoint,  int& cnt) {
  tin[u] = low[u] = cnt++;
  visited[u] = true;

  for (auto& v : adj[u]) {
    if (v == p || !tvisited[v])
      continue;
    if (visited[v])
      low[u] = min(low[u], tin[v]);
    else {
      find_cutpoints(v, u, tvisited,adj, tin, low, visited, cutpoint, cnt);
      
      if (low[v] >= tin[u])
        cutpoint[u] = true;
      low[u] = min(low[u], low[v]);
    }
  }
}

tuple<vector<vector<int>>, vector<vector<int>>, vector<int>, vector<int>> dijkstra(vector<vector<pii>>& adj, int s, int t) {
  vector<int> d(adj.size(), 1e18);

  
  using iii = tuple<int, int, int>;
  vector<vector<int>> tdag(adj.size());
  vector<vector<int>> dag(adj.size());
  vector<int> tdegree(adj.size(), 0);
  priority_queue<iii, vector<iii>, greater<iii>> pq;
  
  pq.push({0, -1, s});
  d[s] = 0;
  vector<char> visited(adj.size(), 0);
  visited[s] = true;
  
  while (!pq.empty()) {
    auto[dd, parent, u] = pq.top(); pq.pop();

    if (dd != d[u])
      continue;

    if (parent != -1) {
      dag[parent].push_back(u);
      dag[u].push_back(parent);
      tdag[u].push_back(parent);
      tdegree[parent]++;
    }
    
    if (parent != -1 && visited[u])
      continue;
    visited[u] = true;
    
    for (auto&[w, v] : adj[u]) {
      int nd = d[u] + w;
      if (nd > d[v])
        continue;
      d[v] = nd, pq.push({nd, u, v});
    }
  }
  
  return {dag, tdag, tdegree, d};
}

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  
  int n, m;
  cin >> n >> m;

  vector<vector<pii>> adj(n);
  while (m--) {
    int u, v, w;
    cin >> u >> v >> w;
    adj[u].push_back({w, v});
  }
  
  int s, t;
  cin >> s >> t;
  auto result = dijkstra(adj, s, t);
  
  auto&[dag, tdag, tdegree, d] = result;
  
  vector<char> tvisited(n, 0);
  dfs(t, tdag, tvisited);
  
  vector<int> tin(dag.size()), low(dag.size());
  vector<bool> cutpoint(dag.size(), 0);
  
  vector<char> visited(dag.size(), 0);
  int cnt = 0;
  find_cutpoints(s, -1, tvisited, dag, tin, low, visited, cutpoint, cnt);
  
  vector<int> ans;
  ans.push_back(s);
  if (s != t)
    ans.push_back(t);
  for (int i = 0; i < n; ++i)
    if (cutpoint[i] && i != s && i != t)
      ans.push_back(i);
  sort(all(ans));
  for (int i = 0; i < ans.size(); ++i)
    cout << ans[i] << (i == ans.size() - 1 ? "\n" : " ");
}
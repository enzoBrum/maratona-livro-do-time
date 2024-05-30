#include <bits/stdc++.h>

#ifdef DEBUG
#define PRINT(s) std::cout << s << '\n';
#endif

using namespace std;
using pii = pair<int, int>;
using ull = unsigned long long;
using ll = long long;

// O(E log V)
// For finding minimum spanning trees
ull prim(vector<vector<pii>> &adj, vector<bool> &visited, int og,
         int &num_visited) {
  priority_queue<pii, vector<pii>, greater<pii>> pq;

  ull cost = 0;
  // vector<bool> visited(adj.size(), false);

  visited[og] = true;
  int n = adj.size();

  for (auto &[w, v] : adj[og])
    if (!visited[v])
      pq.push({w, v});

  while (!pq.empty() && num_visited != n - 1) {
    auto [w, u] = pq.top();
    pq.pop();

    if (visited[u])
      continue;

    visited[u] = true;
    num_visited++;
    cost += w;

    for (auto &[wv, v] : adj[u])
      if (!visited[v])
        pq.push({wv, v});
  }

  return cost;
}

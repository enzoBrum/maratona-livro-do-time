/*
 * Basically, checks if the graph can be colored with 2 colors.
 * If it's possible, the graph is bipartite.
 */

vector<vector<int>> AL;
bool bipartite() {
  int s = 0;
  queue<int> q;
  q.push(s);
  vi color(n, INF);
  color[s] = 0;
  bool isBipartite = true;            // add a Boolean flag
  while (!q.empty() && isBipartite) { // as with original BFS
    int u = q.front();
    q.pop();
    for (auto &v : AL[u]) {
      if (color[v] == INF) {
        color[v] = 1 - color[u]; // just record two colors
        q.push(v);
      } else if (color[v] == color[u]) { // u & v have same color
        isBipartite = false;             // a coloring conflict :(
        break;                           // optional speedup
      }
    }
  }
  return isBipartite;
}

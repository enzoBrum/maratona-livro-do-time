/**
 * Eulerian path --> path that traverses each edge in the graph exactly once.
 * Eulerian tour --> eulerian path that starts and finishes at the same vertex,
 * Eulerian graph --> graph with eulerian tour.
 * an undirected graph is eulerian if it's connected and all vertices have an
 * even degree. a directed graph is eulerian if it's connected and all vertices
 * have in_degree = out_degree.
 */

// hierholzer finds an eulerian path if it exists.
// can't say I understand everything here but whatever. I'm way too short of time for that
int N;
vector<vi> AL; // Directed graph
vi hierholzer(int s) {
  vi ans, idx(N, 0), st;
  st.push_back(s);
  while (!st.empty()) {
    int u = st.back();
    if (idx[u] < (int)AL[u].size()) { // still has neighbor
      st.push_back(AL[u][idx[u]]);
      ++idx[u];
    } else {
      ans.push_back(u);
      st.pop_back();
    }
  }
  reverse(ans.begin(), ans.end());
  return ans;
}

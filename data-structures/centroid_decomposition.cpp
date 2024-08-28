#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using pii = pair<int, int>;

// `closest_red`, query and update were used for solving xenia and the tree.
struct CentroidDecomposition {
  vector<vector<int>> tree;
  vector<int> subtrees_sz, closest_red;
  vector<vector<pii>> parents;
  vector<bool> removed;

  CentroidDecomposition(vector<vector<int>> adj) : tree{adj} {
    int n = tree.size();

    subtrees_sz.resize(n);
    removed.assign(n, false);
    closest_red.assign(n, 1e9);
    parents.resize(n);

    centroid_decomposition(0, -1);
  }

  void calculate_subtree_sizes(int u, int p = -1) {
    subtrees_sz[u] = 1;
    for (auto v : tree[u]) {
      if (v == p || removed[v])
        continue;
      calculate_subtree_sizes(v, u);
      subtrees_sz[u] += subtrees_sz[v];
    }
  }

  int find_centroid(int u, int p, int n) {
    for (auto v : tree[u]) {
      if (v == p || removed[v])
        continue;
      if (subtrees_sz[v] > n / 2)
        return find_centroid(v, u, n);
    }

    return u;
  }

  void calculate_distance_to_centroid(int u, int p, int centroid, int d) {
    for (auto v : tree[u]) {
      if (v == p || removed[v])
        continue;
      calculate_distance_to_centroid(v, u, centroid, d + 1);
    }
    parents[u].push_back({centroid, d});
  }

  void centroid_decomposition(int u, int p = -1) {
    calculate_subtree_sizes(u);
    int centroid = find_centroid(u, p, subtrees_sz[u]);

    for (auto v : tree[centroid]) {
      if (removed[v])
        continue;
      calculate_distance_to_centroid(v, centroid, centroid, 1);
    }

    removed[centroid] = true;

    for (auto v : tree[centroid]) {
      if (removed[v])
        continue;
      centroid_decomposition(v, u);
    }
  }

  int query(int u) {
    int ret = closest_red[u];
    for (auto &[p, pd] : parents[u])
      ret = min(ret, pd + closest_red[p]);

    return ret;
  }

  void update(int u) {
    closest_red[u] = 0;
    for (auto &[p, pdist] : parents[u])
      closest_red[p] = min(closest_red[p], pdist);
  }
};

#include <bits/stdc++.h>

#ifdef DEBUG
#define PRINT(s) std::cout << s << '\n';
#endif

using namespace std;
using ll = long long;

#include <bits/stdc++.h>

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;

struct SegmentTree {

  inline int left(int p) { return p * 2; }

  inline int right(int p) { return p * 2 + 1; }

  SegmentTree(vector<int> &vec) {
    n = vec.size();
    int sz = 4 * n;

    tree.assign(sz, 1e9);
    lazy.assign(sz, -1);

    build(1, 0, n - 1, vec);
  }

  inline int merge(int a, int b) { return min(a, b); }

  void build(int p, int l, int r, vector<int> &vec) {
    if (l == r) {
      tree[p] = vec[l];
      return;
    }

    int m = (l + r) / 2;
    build(left(p), l, m, vec);
    build(right(p), m + 1, r, vec);

    tree[p] = merge(tree[left(p)], tree[right(p)]);
  }

  void propagate(int p, int l, int r) {
    if (lazy[p] == -1)
      return;

    tree[p] = lazy[p];

    if (l != r)
      lazy[left(p)] = lazy[right(p)] = lazy[p];

    lazy[p] = -1;
  }

  int query(int i, int j) { return query(1, 0, n - 1, i, j); }

  void update(int i, int j, int v) { update(1, 0, n - 1, i, j, v); }

  vector<int> lazy, tree;
  int n;

private:
  int query(int p, int l, int r, int i, int j) {
    propagate(p, l, r);
    if (i > j) // valor impossível. merge() deve ignorá-lo
      return 1e9;
    if (l >= i && r <= j)
      return tree[p];

    int m = (l + r) / 2;
    return merge(query(left(p), l, m, i, min(m, j)),
                 query(right(p), m + 1, r, max(i, m + 1), j));
  }

  void update(int p, int l, int r, int i, int j, int v) {
    propagate(p, l, r);
    if (i > j)
      return;
    if (l >= i && r <= j) {
      tree[p] = v;
      lazy[p] = v;
      return;
    }

    int m = (l + r) / 2;
    update(left(p), l, m, i, min(j, m), v);
    update(right(p), m + 1, r, max(i, m + 1), j, v);
    tree[p] = merge(tree[left(p)], tree[right(p)]);
  }
};

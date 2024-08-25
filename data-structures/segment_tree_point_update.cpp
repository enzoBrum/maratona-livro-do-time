struct SegmentTree {

  inline int left(int p) { return p * 2; }

  inline int right(int p) { return p * 2 + 1; }

  SegmentTree(vector<ll> &vec) {
    n = vec.size();
    int sz = 4 * n;

    tree.assign(sz, {1e18, 1e18});

    build(1, 0, n - 1, vec);
  }

  inline pll merge(const pll& a, const pll& b) { 
    return {
      a.first + b.first,
      min(a.second, b.second + a.first)
    };
  }

  void build(int p, int l, int r, vector<ll> &vec) {
    if (l == r) {
      tree[p] = {vec[l], vec[l]};
      return;
    }

    int m = (l + r) / 2;
    build(left(p), l, m, vec);
    build(right(p), m + 1, r, vec);

    tree[p] = merge(tree[left(p)], tree[right(p)]);
  }

  pll query(int i, int j) { return query(1, 0, n - 1, i, j); }

  void update(int i, ll v) { update(1, 0, n - 1, i, v); }

  vector<pair<ll, ll>> tree;
  int n;

private:
  pll query(int p, int l, int r, int i, int j) {
    if (i > j) // valor impossível. merge() deve ignorá-lo
      return {0, 1e18};
    if (l >= i && r <= j)
      return tree[p];

    int m = (l + r) / 2;
    return merge(query(left(p), l, m, i, min(m, j)),
                 query(right(p), m + 1, r, max(i, m + 1), j));
  }

  void update(int p, int l, int r, int i, ll v) {
    if (l == r) {
      tree[p] = {v, v};
      return;
    }

    int m = (l + r) / 2;
    if (i <= m)
      update(left(p), l, m, i, v);
    else
      update(right(p), m + 1, r, i, v);
    tree[p] = merge(tree[left(p)], tree[right(p)]);
  }
};

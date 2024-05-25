#include <bits/stdc++.h>

#ifdef DEBUG
#define PRINT(s) std::cout << s << '\n';
#endif

using namespace std;
using ll = long long;

struct SegmentTree {

  inline int left(int p) { return p * 2; }

  inline int right(int p) { return p * 2 + 1; }

  SegmentTree(vector<int> &vec) {
    n = vec.size();
    int sz = 4 * n;

    tree.resize(sz, 1e9);
    build(1, 0, n - 1, vec);
  }

  inline int merge(int a, int b) {
    if (!a || !b)
      return 0;
    if (a * b < 0)
      return -1;
    return 1;
  }

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

  int query(int i, int j) {
    return query(1, 0, n - 1, i, j);
  }

  void update(int i, int v) { 
    if (v > 0)
      v = 1;
    else if (v < 0)
      v = -1;

    update(1, 0, n - 1, i, v); 
  }

  vector<int> tree;
  int n;

private:
  int query(int p, int l, int r, int i, int j) {
    if (i > j)
      return 1;
    if (l >= i && r <= j)
      return tree[p];

    int m = (l + r) / 2;
    return merge(query(left(p), l, m, i, min(m, j)),
                 query(right(p), m + 1, r, max(i, m + 1), j));
  }

  void update(int p, int l, int r, int i, int v) {
    if (l == r) {
      tree[p] = v;
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

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int n, k;
  while (cin >> n >> k) {
    vector<int> vec(n);
    for (auto &v : vec)
      cin >> v;

    SegmentTree seg(vec);

    while (k--) {
      char c;
      cin >> c;

      if (c == 'C') {
        int i, v;
        cin >> i >> v;
        i--;
        seg.update(i, v);
      } else {
        int i, j;
        cin >> i >> j;
        i--;
        j--;
        int v = seg.query(i, j);
        if (!v)
          cout << '0';
        else if (v > 0)
          cout << '+';
        else
          cout << '-';
      }
    }
    cout << '\n';
  }
}



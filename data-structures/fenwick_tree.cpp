#include <bits/stdc++.h>

using namespace std;

// 1-indexed FenwickTree
struct FenwickTree {
  FenwickTree(int n) { ft.assign(n + 1, 0); }

  FenwickTree(vector<int> &vec) {
    ft.assign(vec.size() + 1, 0);
    for (int i = 0; i < vec.size(); ++i)
      update(i + 1, vec[i]);
  }

  inline int ls_one(int x) { return x & (-x); }

  int query(int r) {
    int sum = 0;
    while (r) {
      sum += ft[r];
      r -= ls_one(r);
    }
    return sum;
  }

  int query(int l, int r) { return query(r) - query(l - 1); }

  void update(int i, int v) {
    while (i < ft.size()) {
      ft[i] += v;
      i += ls_one(i);
    }
  }

  // Finds smallest index i on FenwickTree such that query(1, i) >= rank.
  // I.e: smallest i for [1, i] >= k
  int select(long long k) { // O(log^2 m)
    int lo = 1, hi = ft.size() - 1;
    for (int i = 0; i < 30; ++i) {
      int mid = (lo + hi) / 2;
      (query(1, mid) < k) ? lo = mid : hi = mid;
    }
    return hi;
  }

  vector<int> ft;
};

// Range Update - Point query
struct RUPQ {
  FenwickTree ft;
  RUPQ(int m) : ft{m} {}

  void range_update(int ui, int uj, int v) {
    ft.update(ui, v);
    ft.update(uj + 1, -v);
  }

  int point_query(int i) { return ft.query(i); }
};

// Range Update - Range Query
struct RURQ {
  RUPQ rupq;
  FenwickTree ft;

  RURQ(int m) : ft{m}, rupq(m) {}

  void range_update(int ui, int uj, int v) {
    rupq.range_update(ui, uj, v);
    ft.update(ui, v * (ui - 1));
    ft.update(uj + 1, -v * uj);
  }

  int query(int j) { return rupq.point_query(j) * j - ft.query(j); }

  int query(int i, int j) { return query(j) - query(i - 1); }
};

#include <bits/stdc++.h>

#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;

struct FenwickTree {
  FenwickTree(int n) { ft.assign(n + 1, 0); }

  FenwickTree(vector<int> &vec) {
    ft.assign(vec.size() + 1, 0);
    for (int i = 0; i < vec.size(); ++i)
      update(i + 1, vec[i]);
  }

  inline int ls_one(int x) { return x & (-x); }

  int query(int r) {
    int ret = 0;
    while (r) {
      ret = max(ret, ft[r]);
      r -= ls_one(r);
    }
    return ret;
  }
  void update(int i, int v) {
    while (i < ft.size()) {
      ft[i] = max(ft[i], v);
      i += ls_one(i);
    }
  }

  vector<int> ft;
};


int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  
  int n;
  cin >> n;

  vector<int> va(n), vb(n);
  vector<pii> pairs(n);
  for (int i = 0; i < n; ++i)
    cin >> va[i], pairs[va[i]].first = i;
  for (int i = 0; i < n; ++i)
    cin >> vb[i], pairs[vb[i]].second = i;
  
  int ans = 0;
  FenwickTree ft(n);
  for (int i = 0; i < n; ++i) {
    int num = va[i];
    int ib = pairs[num].second;
    
    int cur =  ft.query(ib+1);
    ft.update(ib+1, cur + 1);

    ans = max(cur + 1, ans);
  }
  
  cout << ans << '\n';
}


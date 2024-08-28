#include <utility>
#include <vector>

// preprocessing: O(n log n)
// range minimum query  (minimum element in [L, R] interval): O(1)
struct SparseTable {
  vector<vector<int>> st;
  int k = 25;
  int n;

  SparseTable(const vector<int> &vec) {
    n = vec.size();
    st.assign(k + 1, vector<int>(n));
    st[0] = vec;

    for (int i = 1; i <= k; ++i)
      for (int j = 0; j + (1 << i) <= n; ++j)
        st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
  }

  int query(int l, int r) {
    int i = bit_width((unsigned long)(r - l + 1)) -
            1; // change to log2 and memoization if c++20 is not available.
    return min(st[i][l], st[i][r - (1 << i) + 1]);
  }
};

s
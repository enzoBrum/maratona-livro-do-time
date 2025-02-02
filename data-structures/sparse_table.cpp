// preprocessing: O(n log n)
// range minimum query  (minimum element in [L, R] interval): O(1)
struct SparseTable {
    vector<vector<int>> st;
    SparseTable(vector<int> &a) {
        int n = a.size();
        int k = log2(n);
        if ((1ll << k) < n) k++;
        st.assign(k+1, vector<int>(n, 0));
        copy(all(a), st[0].begin());

        for (int i = 1; i <= k; i++)
            for (int j = 0; j + (1 << i) <= n; j++)
                st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
    }

    int query(int l, int r) {
        int i = log2(r - l + 1);
        return min(st[i][l], st[i][r - (1 << i) + 1]);
    }
};

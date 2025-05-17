// O(n log n)
// vector d[l] stores the smallest element that a size "l" increasing subsequence ends with

// Alternative solution: compress the values of 'a' and then build a segment tree that queries
// and updates the lis that ends with value 'i'. Build it progressively from left to right.
// Then, the lis of any number a[i] is prefixMax(a[i]-1)+1 or simply 1.
// Useful when you have to extend the lis idea (like lis count of an array).
int lis(vector<int> const& a) {
    int n = a.size();
    const int INF = 1e9;
    vector<int> d(n+1, INF);
    d[0] = -INF;

    for (int i = 0; i < n; i++) {
        int l = upper_bound(d.begin(), d.end(), a[i]) - d.begin();
        if (d[l-1] < a[i] && a[i] < d[l])
            d[l] = a[i];
    }

    int ans = 0;
    for (int l = 0; l <= n; l++)
        if (d[l] < INF)
            ans = l;
    return ans;
}

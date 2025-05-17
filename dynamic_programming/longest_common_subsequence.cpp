// dont forget to memset this to -1
int dp[10001][10001];
int lcs(string &a, string &b, int i, int j) {
    if (i < 0 || j < 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    if (a[i] == b[j]) return dp[i][j] = 1 + lcs(a, b, i-1, j-1);
    return dp[i][j] = max(
        lcs(a, b, i, j-1),
        lcs(a, b, i-1, j)
    );
}

string lcsRecover() {
    int i = a.size() - 1;
    int j = b.size() - 1;
    lcs(i, j);
    string res = "";

    while (i >= 0 && j >= 0) {
        if (a[i] == b[j]) {
            res += a[i];
            i--; j--;
        }
        else if (i > 0 && dp[i][j] == dp[i-1][j]) i--;
        else j--;
    }

    reverse(res.begin(), res.end());
    return res;
}

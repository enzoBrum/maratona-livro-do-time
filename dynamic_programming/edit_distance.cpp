// memset this dude to -1
int dp[2001][2001];
string a, b;
int min(int a, int b, int c) { return min(a, min(b, c)); }
int editDistance(int i, int j) {
    if (i < 0) return j+1;
    if (j < 0) return i+1;
    if (dp[i][j] != -1) return dp[i][j];

    if (a[i] == b[j]) return dp[i][j] = editDistance(i-1, j-1);
    return dp[i][j] = 1 + min(
        editDistance(i, j-1), // insert b[j] onto a
        editDistance(i-1, j), // remove a[i]
        editDistance(i-1, j-1) // a[i] = b[j]
    );
}

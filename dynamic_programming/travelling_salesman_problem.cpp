/*
To start, call tsp(0, 1);
Time complexity: O(2^n n^2)
*/

const int MAXN = 20;
int dp[MAXN+1][1048756]; // 2^20
int d[MAXN+1][MAXN+1];
int n; // vertex count
int tsp(int pos, int mask) {
    if (mask == (1 << n) - 1) return d[pos][0]; // everyone visited, return to root
    if (dp[pos][mask] != -1) return dp[pos][mask];
    int best = INF;
    for (int next = 0; next < n; next++)
        if ((mask & (1 << next)) == 0)
            best = min(best, d[pos][next] + tsp(next, mask | (1 << next)));
    return dp[pos][mask] = best;
}

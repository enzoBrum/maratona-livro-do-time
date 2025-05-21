#include <bits/stdc++.h>

using namespace std;

//#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

/*
Exercise:
n guys hold colored flags. An operation swaps the flags a pair of dudes hold. In how many ways can
you make each one hold its own flag in exactly K operations?
n <= 30, k <= 50

Idea:
Consider a set of cycles made by who owns what flag. For example, person 1 owns flag 3, person
3 owns flag 4, person 4 owns flag 1, and so on. That will decompose an arrangement of people-flags
into several cycles with sum of sizes equal to n. The deal is that swapping 2 flags of the same cycle
splits it, and 2 flags on different cycles merge them.
Let's do a dp. The amount of states is the amount of integer partitions p(n) and remaining movements.
p(n) for n = 30 is 5604, and the maximum amount of movements is k = 50. For each state, compute the n^2
movements you can do (one for each swapped pair) and count in how many ways you can reach the ending
state (all of the cycles with size 1). This gives a time complexity of:
p(n) * k * n^2
= 5604 * 50 * 30 * 30
= 252180000
= 2e8
Alright, but there is one last problem: our state is a vector, and we need to ensure O(1) dp lookup (else
we bring in another O(n) to the loop because of dp checking). The only way to do that is hashing the vector
and updating it while it goes down the recursion.
*/

const int MOD = 1e9+7;

int mul(int a, int b) {
    return 1ll*a*b%MOD;
}

inline int add(int a, int b) {
    a += b;
    a -= (a>=MOD)*MOD;
    return a;
}

const int MAXN = 30;
int ppowers[MAXN+1];
const int p = 31;

inline int hashVector(vector<int> &vec) {
    long long v = 0;
    int idx = 0;
    for (int i : vec)
        v = add(v, mul(i+1, ppowers[idx++]));
    return v;
}

inline int addToHash(int hash, int sz, vector<int> &szFreq) {
    szFreq[sz]++;
    return add(hash, ppowers[sz]);
}

inline int removeFromHash(int hash, int sz, vector<int> &szFreq) {
    szFreq[sz]--;
    return add(hash, MOD-ppowers[sz]);
}

const int MAXK = 50;
map<int, int> dp[MAXK+1];
int n;

int dfs(int u, vector<int> &v, vector<vector<int>> &adj) {
    v[u] = 1;
    for (int next : adj[u])
        if (!v[next])
            return dfs(next, v, adj) + 1;
    return 1;
}

int countWays(vector<int> &szFreq, int k, int v) {
    if (dp[k].count(v)) return dp[k][v];
    if (k == 0) return dp[k][v] = (szFreq[1] == n);

    vector<int> groupSizes;

    for (int i = 1; i <= n; i++) {
        int f = szFreq[i];
        while (f--)
            groupSizes.push_back(i);
    }

    int ans = 0;
    for (int i = 0; i < groupSizes.size(); i++) {
        int szi = groupSizes[i];

        //  - Compute all the answers of swaps within the same group here.
        // every swap with itself will give cycles with size E [1, groupSizes[i]-1], so we iterate
        // over them and count how many times each size appears. 
        // swapping 2 indices with distance d will give one group of size 'd' and another with the
        // remaining quantity 's-d'. 
        // Now, about how many times sz1 appears: an index 'i' has another index j = i+d after it 
        // iff 'i' is not within the last 'd' elements. Therefore, out of all the 's' indices, only
        // s-d have distance d. Therefore, frequency of sz1 is groupSizes[i] - sz1 == sz2.

        for (int sz1 = 1; sz1 < groupSizes[i]; sz1++) {
            // splitting szi into (sz1, sz2)
            int sz2 = szi - sz1;
            v = removeFromHash(v, szi, szFreq);
            v = addToHash(v, sz1, szFreq);
            v = addToHash(v, sz2, szFreq);
            ans = add(ans, mul(countWays(szFreq, k-1, v), sz2)); // here is the multiplication by sz2
            v = addToHash(v, szi, szFreq);
            v = removeFromHash(v, sz1, szFreq);
            v = removeFromHash(v, sz2, szFreq);
        }

        //  - Compute all the answers of swaps with other groups here.
        // any pair of indices (i, j) we swap (i from current group, j from other) will end up in the
        // same cycle sizes. Therefore, there are szi*szj pairs that lead to that.

        for (int j = i+1; j < groupSizes.size(); j++) {
            // merging (szi, szj) onto newSize
            int szj = groupSizes[j];
            int newSize = szi+szj;
            v = addToHash(v, newSize, szFreq);
            v = removeFromHash(v, szi, szFreq);
            v = removeFromHash(v, szj, szFreq);
            int w = countWays(szFreq, k-1, v);
            v = removeFromHash(v, newSize, szFreq);
            v = addToHash(v, szi, szFreq);
            v = addToHash(v, szj, szFreq);
            ans = add(ans, mul(w, szi*szj));
        }
    }

    return dp[k][v] = ans;
}

signed main() {
    int k, t;
    scanf("%d %d %d", &n, &k, &t);

    ppowers[0] = 1;
    for (int i = 1; i <= MAXN; i++)
        ppowers[i] = mul(ppowers[i-1], p);

    vector<int> a(n);
    while (t--) {
        for (int i = 0; i < n; i++) { 
            scanf("%d", &a[i]);
            a[i]--;
        }
        vector<vector<int>> adj(n);
        for (int i = 0; i < n; i++)
            adj[i].push_back(a[i]);

        vector<int> szFreq(n+1, 0);
        vector<int> visited(n, 0);
        for (int i = 0; i < n; i++)
            if (!visited[i])
                szFreq[dfs(i, visited, adj)]++;
        printf("%d\n", countWays(szFreq, k, hashVector(szFreq)));
    }

    return 0;
}
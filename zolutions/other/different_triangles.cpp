#include <bits/stdc++.h>

using namespace std;

//#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;
const int MOD = 1e9+7;

/*
Exercise:
Count how many triples (a, b, c) with sum <= n there are such that
a <= b <= c
and (a, b, c) forms a triangle (that is, a+b > c).

Idea:
Build a function count(n, k) that returns in how many ways you can distribute at most 'n' 
pellets into 'k' piles, in increasing amount between piles (it works recursively by
incrementing all k piles with 1 pellet at once).
Then, if you set the answer as count(n, 3), you have all of the triangles, but you also 
counted wrongly the ones where the following did not hold:
a+b > c
That is, where the following happened:
a+b <= c
To compute the incorrect sums, we can iterate through every possible value of c [0, n]
and then count in how many ways I can distribute the remaining n-c pellets in 2 piles
'a' and 'b' such that their sum is at most c. That is exactly count(min(n-c, c), 2).
*/

inline int add(int a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}

const int MAXN = 1e6;
int dp[MAXN+1][4];
int count(int n, int k) {
    if (n < 0) return 0;
    if (k == 0 || n == 0) return 1;
    if (dp[n][k] != -1) return dp[n][k];
    return dp[n][k] = add(
        count(n-k, k), 
        count(n, k-1)
    );
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);
    memset(dp, -1, sizeof(dp));

    int n;
    cin >> n;

    int ans = count(n, 3);
    for (int c = 0; c <= n; c++)
        ans = add(ans, MOD-count(min(n-c, c), 2));
    cout << ans << '\n';
    
    return 0;
}

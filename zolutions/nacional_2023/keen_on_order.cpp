#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

/*
Exercise:
Given 'k' and a sequence 'a' of size 'n', determine whether every permutation of the numbers
from 1 to 'k' appears as a subsequence of 'a'.

Idea:
If k >= 25, there is always a permutation that does not exist. The following algorithm always works:
 - Take the number whose first appearance is as far ahead as possible. It is at least the 25th element.
 - Now, take another number whose first appearence AFTER the previous number is as far ahead as possible.
   It is necessarily at least 24 numbers after the previous one.
 - Continue that idea.
That algorithm always works because for such subsequence to exist, it would have size
25 + 24 + ... + 1 = 25(1+25)/2 = 325 > n = 300
How to solve for k <= 24?
Keep a DP of bitmasks. DP[mask] returns the largest index that any permutation of the numbers on the
mask can end at. The recursion is trivial: all the permutations of this mask end in some element of
the mask. Therefore, iterate through every element, and compute what would be the maximum length with
it as last element. That length is exactly the first appearance of 'i' after dp[mask without i].
To recover the permutation itself, we will keep a vector 'p' that tells what element I should append
to get maximum permutation ending.
*/

int dp[1<<24];
int p[1<<24];
int nex[300][300];

int maxSubsequenceEnding(int mask) {
    if (dp[mask] != -1)
        return dp[mask];

    if (__builtin_popcountll(mask) == 1) {
        int i = 0;
        while (!(mask & (1<<i)))
            i++;
        p[mask] = i;
        return dp[mask] = nex[i][0]; // first appearance of i
    }

    for (int i = 0; i < 24; i++) {
        if (!(mask & (1<<i))) continue;
        int next = maxSubsequenceEnding(mask ^ (1<<i));
        if (next == INF || nex[i][next] == INF) { // this is a losing subsequence
            p[mask] = i;
            return dp[mask] = INF;
        }
        if (nex[i][next] > dp[mask]) {
            dp[mask] = nex[i][next];
            p[mask] = i; // appending i leads to the rightmost ending subsequence
        }
    }

    return dp[mask];
}

vector<int> ans;
void buildAns(int mask) {
    if (mask == 0) return;
    int next = p[mask];
    buildAns(mask ^ (1<<next));
    ans.push_back(next);
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &i : a) {
        cin >> i;
        i--;
    }

    if (k >= 25) {
        set<int> notUsed, notPrinted;
        for (int i = 0; i < k; i++) {
            notUsed.insert(i);
            notPrinted.insert(i);
        }

        for (int i = 0; i < n; i++) {
            if (notUsed.count(a[i])) {
                notUsed.erase(a[i]);
                if (!notUsed.size()) {
                    cout << a[i]+1 << " ";
                    notPrinted.erase(a[i]);
                    for (int i : notPrinted)
                        notUsed.insert(i);
                }
            }
        }

        for (int i : notPrinted)
            cout << i+1 << " ";
        cout << '\n';
        return 0;
    }

    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 300; j++)
            nex[i][j] = INF;
    for (int i = n-1; i >= 0; i--)
        for (int j = i; j >= 0; j--)
            nex[a[i]][j] = i;

    // out of all the subsequences of this mask, which one ends the furthest away?
    memset(dp, -1, sizeof(dp));
    memset(p, -1, sizeof(p));

    int fullMask = (1<<k)-1;
    if (maxSubsequenceEnding(fullMask) != INF) cout << "*\n";
    else {
        buildAns(fullMask);
        for (int i : ans)
            cout << i+1 << " ";
        cout << '\n';
    }

    return 0;
}

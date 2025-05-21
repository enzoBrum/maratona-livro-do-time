#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;

/*
Exercise:
You are given n=1e5 blueprints of buildings. Those buildings have a single ground floor of height 'G'
and at least 1 (or millions, up to you) residential floors of height 'R'. You wanna know whether it is
possible to distribute those blueprints onto two nonempty sets such that it is possible to build all
buildings of both sets and that the sum of the heights of each set of buildings ends up equal.

Idea:
Say you partitioned the blueprints onto the nonempty sets A and B. 
This means you need the following to hold:

sum(i E A) Gi + sum(i E A) Ri * ki = sum(i E B) Gi + sum(i E B) Ri * ji
sum(i E A) Ri * ki - sum(i E B) Ri * ji = sum(i E B) Gi - sum(i E A) Gi
sum(i E A) Ri * ki + sum(i E B) Ri * (-ji) = sum(i E B) Gi - sum(i E A) Gi

Notice that the above expression is a linear combination of all Ri. This means that an answer
exists iff there is a linear combination of Ri that ends up summing any possible difference of
the G of the sets, given that there are 2 nonempty partitions of G with that difference.
Therefore, from "extended Bèzout", there is an answer iff
gcd(all Ri) | some difference of G
Also, it is clear that, if some possible difference is different than the sum of all G, there
is always 2 nonempty sets that give that difference. But if the difference is EQUAL to the sum
of all G, then, you need at least a Gi = 0 and any other G, that is:
Gi = 0 for some Gi
n >= 2

But a linear combination may include negative coefficients. What about that?
Suppose you want the coefficient Ri to be increased on the left. Then, you grab any Rj on the
right, and sum to both sides Ri*Rj*k, where k is any integer > 1. What will end up happening
is that you increased left's Ri coefficient by (Rj*k), while right's Rj coefficient by (Ri*k).
Therefore, you can increase any coefficient arbitrarily.

So now, the solution is to iterate over all possible differences of G, do the checks above, and 
see if gcd(all Ri) divides the current difference.

How to get those differences efficiently? The DP is like subset sum, but we know that the sum of Gi
is O(n), that is, there are at most O(sqrt(n)) different Gi.
With that in mind, we can make a map of frequencies of Gi, and, instead of putting them all in the
knapsack vector, we will COMPRESS them.

Say there are 14 items with weight 'w'. You can write them as:
[w, 2w, 4w, 7w]
and every sum that was possible with the 14 individual items is possible now with these powers
of 2. Therefore, if our frequency map had O(sqrt(n)) entries, each one with frequency O(n), then,
we can reduce that to O(sqrt(n) * log(n)) entries. Then, the other dimension of the DP is their sum
O(n), leading to a O(n * sqrt(n) * log(n)) solution.
*/

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    // ----------- READING INPUT ----------- //
    int n;
    cin >> n;

    int accGcd;
    int sumG = 0;
    bool nullG = false;
    map<int, int> freq;
    for (int i = 0; i < n; i++) {
        int g, r;
        cin >> g >> r;
        if (i == 0) accGcd = r;
        else accGcd = gcd(accGcd, r);
        freq[g]++;
        sumG += g;
        if (!g)
            nullG = true;
    }

    // ----------- COMPUTING COMPRESSED KNAPSACK VALUES ----------- //
    vector<int> cost;
    for (auto[weight, count] : freq) {
        int rem = count;
        for (int i = 1; i <= rem; i *= 2) {
            cost.push_back(weight*i);
            rem -= i;
        }
        if (rem) cost.push_back(weight*rem);
    }

    // ----------- RUNNING DP ----------- //
    // can achieve weight 'w' using first 'i' indices
    vector<vector<int>> dp(sumG+1, vector<int>(cost.size(), 0));
    for (int i = 0; i < cost.size(); i++)
        dp[0][i] = 1;
    dp[cost[0]][0] = 1;

    for (int w = 1; w <= sumG; w++) {
        for (int i = 1; i < cost.size(); i++) {
            if (dp[w][i-1])
                dp[w][i] = 1;
            else if (cost[i] <= w)
                dp[w][i] = dp[w-cost[i]][i-1];
        }
    }

    for (int w = 1; w < sumG; w++) {
        if (dp[w][cost.size()-1]) {
            int A = w;
            int B = sumG - A;
            int D = abs(A-B);
            if (D % accGcd == 0) {
                cout << "Y\n";
                return 0;
            }
        }
    }

    // last case: difference == sumG
    if (sumG % accGcd == 0 && nullG && n >= 2)
        cout << "Y\n";
    else
        cout << "N\n";
    return 0;
}

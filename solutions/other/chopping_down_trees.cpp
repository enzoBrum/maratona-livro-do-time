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
You will chop down M trees that are on a field with integer coordinates on [1, N]. You don't know their
coordinates yet. There are neighbour's trees on 0 and N+1. All of the trees have height H. Chopping down a
tree at position x will knock down trees up to x+H (inclusive), in the direction the tree at x was falling.
Given that the distributions of the trees on the field are equaly likely, compute the probability that you
can chop down all of your trees without knocking down your neighbour's trees.

Idea:
There are M trees. This means that there are M+1 space intervals in between your trees and neighbour's trees.
The big idea is that the job can be completed iff one of those spaces is > h. Ignoring other combinatorial
parts, we have to count in how many ways we can distribute N+1 units of space in M+1 intervals such that
each interval has size <= h (we are computing the complement). This can be done with FFT + binary power.
Build a vector of size h+1 where every element is 1 except h[0]. This represents that there is 1 way to
distribute 'i' units of space in 1 interval (except 0 units of space, because the trees must be spaced by at
least 1 unit). Now, if we compute h^k, we will know in how many ways we can distribute 'i' units of space
in 'k' intervals. The recursive idea is: we are given h^(k-1), the distributions of 'i' units of space in
'k-1' intervals. Now, to compute h^k we would, for each position 'i', iterate through every size 's' (s <= i)
the rightmost interval could have and append it to the right of 'k-1' intervals with sum i-s. That is
the definition of FFT (h[i] = sum p[s]*p[i-s] for all s, where p is the previous h vector, h^(k-1)).
So, we build 'h', compute h^(m+1) and use h[n+1] to do the rest of the exercise (which I won't explain here).
*/

const int MOD = 998244353;

inline int add(int a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}

inline int mul(int a, int b) {
    return a * b % MOD;
}

int pwr(int a, int b){
    int r = 1;
    while (b) {
        if (b & 1) r = mul(r, a);
        a = mul(a, a);
        b >>= 1;
    }
    return r;
}

int inv(int x) {
    return pwr(x, MOD-2);
}

void ntt(vector<int> &a, bool rev) {
    int n = a.size();
    vector<int> b = a;
    int g = 1;
    while (pwr(g, MOD / 2) == 1)
        g++;
    if (rev)
        g = inv(g);
    for (int step = n / 2; step; step /= 2) {
        int w = pwr(g, MOD / (n / step));
        int wn = 1;
        for (int i = 0; i < n / 2; i += step) {
            for (int j = 0; j < step; j++) {
                int u = a[2 * i + j];
                int v = mul(wn, a[2 * i + j + step]);
                b[i + j] = add(u, v);
                b[i + n / 2 + j] = add(u, MOD - v);
            }
            wn = mul(wn, w);
        }
        swap(a, b);
    }
    if (rev) {
        int n1 = inv(n);
        for (int &x : a)
            x = mul(x, n1);
    }
}

vector<int> multiply(vector<int> a, vector<int> b) {
    int n = 1;
    while (n < a.size() + b.size())
        n *= 2;
    a.resize(n); b.resize(n);
    ntt(a, false); ntt(b, false);

    for (int i = 0; i < n; i++)
        a[i] = mul(a[i], b[i]);

    ntt(a, true);
    return a;
}

// discards everything after lastIndex
vector<int> pwr(vector<int> a, int b, int lastIndex) {
    vector<int> r(a.size(), 0); r[0] = 1; // neutral polynomial
    while (b) {
        if (b & 1) {
            r = multiply(r, a);
            r.resize(lastIndex+1);
        }
        a = multiply(a, a);
        a.resize(lastIndex+1);
        b >>= 1;
    }
    return r;
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int tt;
    cin >> tt;
    while (tt--) {
        int n, m, h;
        cin >> n >> m >> h;

        vector<int> poly(h+1, 1);
        poly[0] = 0;
        poly = pwr(poly, m+1, n+1);

        int nf = 1;
        for (int i = 2; i <= n; i++)
            nf = mul(nf, i);
        int mf = 1;
        for (int i = 2; i <= m; i++)
            mf = mul(mf, i);
        int nmf = 1;
        for (int i = 2; i <= n-m; i++)
            nmf = mul(nmf, i);
        int combNM = mul(nf, mul(inv(mf), inv(nmf)));
        int icombNM = inv(combNM);
        int ans = mul(add(combNM, MOD - poly[n+1]), icombNM);
        cout << ans << '\n';
    }
    
    return 0;
}

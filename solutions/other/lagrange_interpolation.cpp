/*
About Lagrange interpolation:
You are given k+1 points. You want the coefficients of the kth degree polynomial
that goes through them. You can do that for arbitrary (x, y) in O(k²) time. This
is explained as follows:
Suppose the said polynomial is
P(x) = l_1*f(x1) + l_2*f(x2) + ... + l_k+1*f(x_(k+1))
If you compute P(x1), you'd like the following to happen:
l_1 = 1
l_2 = 0
l_3 = 0
...
l_(k+1) = 0
In general, for P(xi), you want l_i to be 1 if x's index is i and 0 if its not.
Those l_i's are actually polynomials, and their shape is the following:
l_i = (x-x1) * (x-x2) * (x-x3) * ... * (x-x_(i-1)) * (x-x_(i+1)) * ... * (x-x_(k+1))
DIVIDED BY
(x_i-x1) * (x_i-x2) * (xi-x3) * ... * (xi-x_(i-1)) * (xi-x_(i+1)) * ... * (xi-x_(k+1))
The numerator has everyone but (x-xi), and the denominator has everyone but (xi-xi).
It is pretty clear that if you throw xi onto l_i it will be 1, and 0 if you throw
any of the other (k+1-1) points.

So, it means that P is already the polynomial you were searching for. Evaluating it
takes O(k²), unless there is any property of the numbers you are working with such
that you can transition from l_i to l_(i+1) in sublinear time...
*/

#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define detachingFromC ios::sync_with_stdio(0);cin.tie(0);
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;

const int MOD = 1e9+7;

/*
1) Compute k+2 points to define the k+1th degree polynomial (the sum itself)
2) Compute the polynomial l1(n) in O(k) time
3) Transition from l_i to l_(i+1) in O(1) time for all i and evaluate the polynomial
*/

tuple<int, int, int> extendedGcd(int a, int b) {
    if (b == 0) return make_tuple(a, 1, 0);
    auto[q, w, e] = extendedGcd(b, a%b);
    return make_tuple(q, e, w-e*(a/b));
}

int multiplicativeInverse(int n, int mod) {
    // (n)x + (mod)y = 1 (aka their difference is 1) 
    n = (n % mod + mod) % mod;
    auto[g, x, y] = extendedGcd(n, mod);
    return (x % mod + mod) % mod;
}

int fastPow(int base, int e) {
    if (e == 0) return 1;
    if (e == 1) return base;
    int h = fastPow(base, e/2);
    if (e % 2 == 1) return h*h % MOD * base % MOD;
    return h*h % MOD;
}

signed main() {
    detachingFromC;

    int n, k;
    cin >> n >> k;

    // 1)
    vector<int> y(k+3, 0);
    y[1] = 1;
    for (int xi = 2; xi <= k+2; xi++)
        y[xi] = (y[xi-1] + fastPow(xi, k)) % MOD;

    if (n <= k+2) {
        cout << y[n] << endl;
        return 0;
    }

    // 2)
    int num = 1;
    int den = 1;
    for (int xi = 2; xi <= k+2; xi++) {
        num = num * (n - xi) % MOD;
        den = (den * (1 - xi) % MOD + MOD) % MOD;
    }

    // 3)
    int ans = num * multiplicativeInverse(den, MOD) % MOD * y[1] % MOD;
    for (int xi = 2; xi <= k+2; xi++) {
        // num: take out factor (x-x_i) and bring (x-x_(i-1))
        num = num * multiplicativeInverse(n - xi, MOD) % MOD;
        num = num * (n - (xi-1)) % MOD;
        // den: take out factor (x_(i-1) - x_last) and bring (xi - x1)
        den = den * multiplicativeInverse(xi-1 - (k+2), MOD) % MOD; // k+2 == x_last
        den = den * (xi-1) % MOD;
        // increment answer by num/den * yi
        ans += num * multiplicativeInverse(den, MOD) % MOD * y[xi] % MOD;
        ans %= MOD;
    }

    cout << ans << endl;
    return 0;
}
